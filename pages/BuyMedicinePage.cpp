#pragma once
#include "../globals/Pages.cpp"
#include "headers/BuyMedicinePage.h"
#include "../data/MedicineManager.cpp"
#include "../data/Medicine.cpp"
#include "../globals/Utils.cpp"
#include "../globals/DBConfig.cpp"
#include <string>
#include <iostream>
#include <boost/container/vector.hpp>
#include <boost/container/map.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

typedef allocator<char, managed_shared_memory::segment_manager> CharAllocator;
typedef basic_string<char, std::char_traits<char>, CharAllocator> MyString;
typedef allocator<MyString, managed_shared_memory::segment_manager> StringAllocator;
typedef boost::interprocess::vector<MyString, StringAllocator> Vect;

using namespace boost::interprocess;

// Default constructor.
BuyMedicinePage::BuyMedicinePage() {}

// Default destructor.
BuyMedicinePage::~BuyMedicinePage() { }

// CheckSharedMemory() is run by a separate thread of execution.
// It checks the shared memory region for any data changes before
// executing the remainder of the .monitor() method.
void BuyMedicinePage::checkSharedMemory()
{

    boost::mutex myMutex;
    myMutex.lock();

    try
    {
        
        // Reference shared memory.
        managed_shared_memory* shm = DBConfig::getInstance().sharedMemoryObject.get();

        // Obtain the thread-safe vector that is stored in shared memory.
        Vect* m = shm->find<Vect>("propertyVector").first;

        // There exists new Medicine objects in shared memory that were added from a different process.
        if(m != nullptr && m->size() >= 7 && m->size() % 7 == 0)
        {

            int count = 0;
            unsigned int id = 0;
            std::string name = "";
            std::string ownedBy = "";
            std::string arrivalDate = "";
            std::string expirationDate = "";
            double price = 0.0;
            int qty = 0;

            // If the vector exists and there are multiple Medicine objects, update in-memory
            // data structure; i.e.: MedicineManager.
            for(auto itr = m->begin(); itr != m->end(); itr++, count++)
            {

                // This is the 7th item. Medicine objects store 7 variables, therefore we have enough
                // information now to potentially add this to our in-memory data structure.
                if((count + 1) >= 7 && (count + 1) % 7 == 0)
                {
                    
                    Medicine& target = MedicineManager::getInstance().getById(id);
                    qty = std::stoi(std::string(itr->begin(), itr->end()));

                    // If we repeatedly go through this process, it is possible that the item
                    // is already consumed and is in the in-memory data structure for this process.
                    // We don't want duplicates, so we want to verify that this Medicine object
                    // is unique before adding it.
                    if(target._id == 0)
                    {

                        // Build Medicine object from properties and add to MedicineManager vector.
                        const Medicine objMedicine{id, name, ownedBy, arrivalDate, expirationDate, price, qty};
                        MedicineManager::getInstance().add(objMedicine);

                        // Reset local variable properties.
                        id = 0;
                        name = "";
                        ownedBy = "";
                        arrivalDate = "";
                        expirationDate = "";
                        price = 0.0;
                        qty = 0;

                    // It is possible that it is not neccessarily unique, but some properties may have been updated. We will need to reflect
                    // this in the MedicineManager class.
                    }
                    else
                    {

                        if(target._name != name)
                        {

                            target._name = name;

                        }

                        if(target._ownedBy != ownedBy)
                        {

                            target._ownedBy = ownedBy;

                        }

                        if(target._arrivalDate != arrivalDate)

                        {

                            target._arrivalDate = arrivalDate;

                        }

                        if(target._expirationDate != expirationDate)
                        {

                            target._expirationDate = expirationDate;

                        }

                        if(target._price != price)
                        {

                            target._price = price;

                        }

                        if(target._qty != qty)
                        {

                            target._qty = qty;
                            
                        }
                    }

                    count = -1; // Set count to -1, because we know it will be incremented to zero at the end of this iteration.
                    itr++;
                    continue;

                }

                // We need to figure out which property this is. We can logically deduce this from the value of count.
                if(count == 0)
                {

                    id = std::stoi(std::string(itr->begin(), itr->end()));

                }
                else if(count == 1)
                {

                    name = std::string(itr->begin(), itr->end());

                }
                else if(count == 2)
                {

                    ownedBy = std::string(itr->begin(), itr->end());

                }
                else if(count == 3)
                {

                    arrivalDate = std::string(itr->begin(), itr->end());

                }
                else if(count == 4)
                {

                    expirationDate = std::string(itr->begin(), itr->end());

                }
                else if(count == 5)
                {

                    price = std::stod(std::string(itr->begin(), itr->end()));

                }
                else
                {

                    qty = std::stoi(std::string(itr->begin(), itr->end()));

                }
            }
        }

    // Check if any IDs are present in MedicineManager and deleteVector. If so, we
    // should remove them.
    Vect* deleteVector = shm->find<Vect>("deleteVector").first;

    for(auto dItr = deleteVector->begin(); dItr != deleteVector->end(); ++dItr)
    {

        const unsigned int intId = std::stoi(std::string(dItr->begin(), dItr->end()));
        Medicine& targetMed = MedicineManager::getInstance().getById(intId);
        
        if(targetMed._id != 0)
        {

            MedicineManager::getInstance().removeById(targetMed._id);

        }
    }
    
    }
    catch(const std::exception& e) 
    {

        std::cout << e.what() << std::endl;

    }

    myMutex.unlock();

}

// HandleBuy(newQty) takes a vector of tuples (i, q) where i is the
// ID of the item being update and q is the new quantity. It is thread-safe.
void BuyMedicinePage::handleBuy(boost::container::vector<boost::tuple<int, int>>& newQty)
{

    boost::mutex myMutex; // Initialize mutex.
    myMutex.lock(); // Block until lock is acquired.
    SQLConnection& conn = *DBConfig::getInstance().connObj.get();
    boost::container::map<std::string, std::string> newQtyAsMap;
    conn.connect();

    for(const boost::tuple<int, int> tup : newQty)
    {

        std::string strId = std::to_string(tup.get<0>());
        std::string strQty = std::to_string(tup.get<1>());
        conn.insert("UPDATE public.medicines SET qty=" + strQty + " WHERE id=" + strId + ";");
        newQtyAsMap[strId] = strQty;

    }

    conn.disconnect();

    // Reference shared memory.
    managed_shared_memory* shm = DBConfig::getInstance().sharedMemoryObject.get();
    Vect* m = shm->find<Vect>("propertyVector").first;

    // We need to make all new quantities available in shared memory segment.
    const CharAllocator charAllocInst(shm->get_segment_manager());
    
    for(auto itr = m->begin(); itr != m->end(); itr += 7)
    {

        std::string ref(itr->begin(), itr->end());

        // The ID that was just added is also in shared memory. This means we do not
        // need to add all the properties for it. We can just update the quantity field.
        if(newQtyAsMap.contains(ref))
        {

            auto qtyItr = itr + 6;
            *qtyItr = MyString(newQtyAsMap[ref].begin(), newQtyAsMap[ref].end(), charAllocInst);

        // In this case, the ID is not in shared memory. We will need to add all properties for it.
        }
        else
        {

            Medicine& target = MedicineManager::getInstance().getById(std::stoi(std::string(itr->begin(), itr->end())));
            const std::string strId2 = std::to_string(target._id);
            const std::string strPrice = std::to_string(target._price);
            const std::string strQty = newQtyAsMap[ref];
            MyString myStringId(strId2.begin(), strId2.end(), charAllocInst);
            MyString myStringName(target._name.begin(), target._name.end(), charAllocInst);
            MyString myStringOwnedBy(target._ownedBy.begin(), target._ownedBy.end(), charAllocInst);
            MyString myStringAD(target._arrivalDate.begin(), target._arrivalDate.end(), charAllocInst);
            MyString myStringED(target._expirationDate.begin(), target._expirationDate.end(), charAllocInst);
            MyString myStringPrice(strPrice.begin(), strPrice.end(), charAllocInst);
            MyString myStringQty(strQty.begin(), strQty.end(), charAllocInst);
            m->push_back(myStringId);
            m->push_back(myStringName);
            m->push_back(myStringOwnedBy);
            m->push_back(myStringAD);
            m->push_back(myStringED);
            m->push_back(myStringPrice);
            m->push_back(myStringQty);

        }
    }

    // Shared memory segment is completely empty. Loop will not be entered. We can
    // handle adding these Medicine objects that were manipulated on this screen
    // here.
    if(m->size() == 0)
    {

        for(auto pair : newQtyAsMap)
        {

            Medicine& target = MedicineManager::getInstance().getById(std::stoi(pair.first));
            const std::string strId2 = std::to_string(target._id);
            const std::string strPrice = std::to_string(target._price);
            const std::string strQty = std::to_string(target._qty);
            MyString myStringId(strId2.begin(), strId2.end(), charAllocInst);
            MyString myStringName(target._name.begin(), target._name.end(), charAllocInst);
            MyString myStringOwnedBy(target._ownedBy.begin(), target._ownedBy.end(), charAllocInst);
            MyString myStringAD(target._arrivalDate.begin(), target._arrivalDate.end(), charAllocInst);
            MyString myStringED(target._expirationDate.begin(), target._expirationDate.end(), charAllocInst);
            MyString myStringPrice(strPrice.begin(), strPrice.end(), charAllocInst);
            MyString myStringQty(strQty.begin(), strQty.end(), charAllocInst);
            m->push_back(myStringId);
            m->push_back(myStringName);
            m->push_back(myStringOwnedBy);
            m->push_back(myStringAD);
            m->push_back(myStringED);
            m->push_back(myStringPrice);
            m->push_back(myStringQty);

        }
    }

    myMutex.unlock(); // Release lock.

}

// Monitor() monitors() user input.
void BuyMedicinePage::monitor()
{

    boost::container::vector<int> idVect;
    boost::container::vector<boost::tuple<int, int>> newQty;
    bool emptyCache = MedicineManager::getInstance().getData().size() == 0;
    std::string id;
    boost::thread memoryThread{&BuyMedicinePage::checkSharedMemory, this};
    memoryThread.join();
    log();
    std::cin.ignore();

    if(!emptyCache)
    {

    
    while(true)
    {

        std::cout << "Enter item ID (press q to accept selections): ";
        std::getline(std::cin, id);

        if(id == "q") { break; }

        // See if we can convert it to an integer without raising
        // an exception. If we can, it is a valid number and we
        // can add it to our selection vector. If it isn't, just
        // ignore it.
        try
        {
            
            int temp = std::stoi(id);
            Medicine& m = MedicineManager::getInstance().getById(temp);

            if(temp >= 1 && m._id > 0)
            {

                if(m._qty > 0)
                {

                    boost::tuple<int, int> tup = boost::make_tuple(temp, m._qty - 1);
                    idVect.push_back(temp);
                    newQty.push_back(tup);
                    m._qty = m._qty - 1;

                }
                else
                {

                    std::cout << "Item no. " << temp << " is not in stock." << std::endl;

                }
            }
            else
            {

                std::cout << "Invalid ID specified or item cannot be found." << std::endl;

            }
        }
        catch(const std::exception& e)
        { }

    }

    std::cout << std::endl <<  "The following IDs were selected: [";
    int i = 0;
    double totalPurchasePrice;

    for(auto itr = idVect.begin(); itr != idVect.end(); itr++, i++)
    {

        double tempPrice = MedicineManager::getInstance().getById(*itr)._price;
        totalPurchasePrice += tempPrice;

        if(i == idVect.size() - 1)
        {

            std::cout << *itr;

        }
        else
        {

            std::cout << *itr << ", ";

        }
    }

    std::cout << "]" << std::endl;
    std::cout << std::fixed << std::setprecision(2) << "Total Purchase Price: " << totalPurchasePrice << std::endl;
    boost::thread memoryThread2{&BuyMedicinePage::checkSharedMemory, this};
    boost::thread workerThread(&BuyMedicinePage::handleBuy, this, std::ref(newQty));
    memoryThread2.join();
    workerThread.join();

    }

    std::cout << std::endl << "Press 'm' to return to the main menu..." << std::endl;
    std::string input;

    while(true)
    {
        

        std::cin >> input;

        if(input == "m")
        {

            break;

        }

        std::cout << "Invalid key pressed; you pressed " << input << std::endl;

    }

    system("clear");
    Pages::getInstance().MAIN.log();
    return;
    
}

// Log() will log medicine transaction details.
void BuyMedicinePage::log()
{

    std::cout << "Medicine Transaction" << std::endl << std::endl;
    MedicineManager& ref = MedicineManager::getInstance();
    boost::container::vector<Medicine> cache = ref.getData();
    int count = 0;
    
    for(const Medicine& m : cache)
    {

        bool printHeader = count == 0 ? true : false;
        m.print(Medicine::L_ALL, printHeader);
        count++;

    }

    bool emptyCache = cache.size() == 0;

    if(emptyCache)
    {

        std::cout << "There are no products currently in the database." << std::endl;

    }
    else
    {
    
        std::cout << "---------------------------------------------------------------------------------------------------------------------------------" << std::endl;
    
    }
}