#pragma once
#include "../globals/Pages.cpp"
#include "../data/MedicineManager.cpp"
#include "../globals/Utils.cpp"
#include "../globals/DBConfig.cpp"
#include "headers/DeleteStockPage.h"
#include <string>
#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

using namespace boost::interprocess;
typedef allocator<char, managed_shared_memory::segment_manager> CharAllocator;
typedef basic_string<char, std::char_traits<char>, CharAllocator> MyString;
typedef allocator<MyString, managed_shared_memory::segment_manager> StringAllocator;
typedef boost::interprocess::vector<MyString, StringAllocator> Vect;

// Default constructor.
DeleteStockPage::DeleteStockPage() { }

// Default destructor.
DeleteStockPage::~DeleteStockPage() { }

// CheckSharedMemory() is run by a separate thread of execution.
// It checks the shared memory region for any data changes before
// executing the remainder of the .monitor() method.
void DeleteStockPage::checkSharedMemory()
{

    boost::mutex myMutex;
    myMutex.lock();

    try {

        // Reference shared memory.
        managed_shared_memory* shm = DBConfig::getInstance().sharedMemoryObject.get();

        // Obtain the thread-safe vector that is stored in shared memory.
        Vect* m = shm->find<Vect>("propertyVector").first;

        // Are their new properties in this vector?
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
    {}



    myMutex.unlock();

}

// Monitor() monitors() user input.
void DeleteStockPage::monitor()
{

    const size_t n = MedicineManager::getInstance().getData().size();
    bool emptyCache = n == 0;
    std::string input;
    bool inputReq = false;
    int intId = -1;
    boost::thread memoryThread{&DeleteStockPage::checkSharedMemory, this};
    memoryThread.join();
    log();
    std::cin.ignore();

    // Obtain product ID as input
    while(true && !emptyCache)
    {

        std::cout << std::endl << "Enter a Product ID to delete: ";
        std::getline(std::cin, input);
        bool isIdValid = Utils::getInstance().isStringInteger(input);

        if(isIdValid)
        {

            intId = std::stoi(input);

            if(intId >= 1 && (MedicineManager::getInstance().getById(intId)._id > 0))
            {

                break;

            } 
            else
            {

                std::cout << std::endl << "Invalid value specified for Product ID; ID out of bounds." << std::endl;

            }
        }
        else
        {

            std::cout << std::endl << "Invalid value specified for Product ID; please enter an integer value." << std::endl;
            continue;

        }

    }

    std::string confirmation;
    bool performDelete = false;

    // Ask user to confirm their decision.
    while(true && !emptyCache)
    {

        std::cout << std::endl << "Are you sure you would like to delete Product [ID=" << intId << "]? This CANNOT BE UNDONE. (y / n): ";
        std::getline(std::cin, confirmation);
            
        if(confirmation == "Y" || confirmation == "y")
        {

            performDelete = true;
            boost::thread memoryThread2{&DeleteStockPage::checkSharedMemory, this};
            boost::thread workerThread(&DeleteStockPage::handleDeleteRelation, this, std::ref(intId));
            memoryThread2.join();
            workerThread.join();
            break;

        }
        else if(confirmation == "N" || confirmation == "n")
        {

            break;

        }
        else
        {

            std::cout << "Invalid input; please use (Y/y/N/n)" << std::endl;

        }
    }

    if(emptyCache)
    {

        std::cout << std::endl << "There are no products currently in the database." << std::endl;
        std::cout << std::endl << "Press 'm' to return to the main menu.." << std::endl;

    }
    else
    {

        std::cout << std::endl << "Press 'm' to return to the main menu or 'd' to delete a different item..." << std::endl;

    }

    while(true && emptyCache)
    {
        

        std::cin >> input;

        if(input == "m")
        {

            break;

        }

        std::cout << std::endl << "Invalid key pressed; you pressed " << input << std::endl;

    }


    while(true && !emptyCache)
    {
        

        std::cin >> input;

        if(input == "m")
        {

            break;

        }

        if(input == "d")
        {

            inputReq = true;
            break;

        }

        std::cout << std::endl << "Invalid key pressed; you pressed " << input << std::endl;

    }

    if(inputReq)
    {

        monitor();

    }
    else
    {

        const size_t upd_size = MedicineManager::getInstance().getData().size();

        if(upd_size > 2)
        {

            MedicineManager::getInstance().heapSortById();

        }
        else if(upd_size == 2)
        {

            auto items = MedicineManager::getInstance().getData();

            if(items[0]._id > items[1]._id)
            {

                std::swap(items[0], items[1]);

            }
        }

        system("clear");
        Pages::getInstance().MAIN.log();
        
    }
}

// Log() will log medicine deletion details.
void DeleteStockPage::log()
{

    std::cout << "Delete Stock Center" << std::endl;
    bool emptyCache = MedicineManager::getInstance().getData().size() == 0;

    if(!emptyCache)
    {

        MedicineManager& ref = MedicineManager::getInstance();
        boost::container::vector<Medicine> cache = ref.getData();
        int count = 0;
    
        for(const Medicine& m : cache)
        {

            bool printHeader = count == 0 ? true : false;
            m.print(Medicine::L_ALL, printHeader);
            count++;

        }

        std::cout << "---------------------------------------------------------------------------------------------------------------------------------" << std::endl;

    }
}

// HandleDeleteRelation(id) deletes a relation from the appropriate database table
// given the primary key id.
void DeleteStockPage::handleDeleteRelation(const unsigned int& id)
{

    boost::mutex myMutex; // Initialize lock.
    myMutex.lock(); // Block until lock is acquired.
    SQLConnection& conn = *DBConfig::getInstance().connObj.get();
    const std::string strId = std::to_string(id);
    conn.connect();
    conn.fetch("DELETE FROM public.medicines WHERE id=" + strId);
    conn.disconnect();
    MedicineManager::getInstance().removeById(id);

    // Get shared memory reference.
    managed_shared_memory* shm = DBConfig::getInstance().sharedMemoryObject.get();
    Vect* m = shm->find<Vect>("deleteVector").first;
    bool idFound = false;

    // Search and see if the ID is already in the shared memory segment.
    for(auto itr = m->begin(); itr != m->end(); ++itr)
    {

        std::string ref(itr->begin(), itr->end());

        if(ref == strId)
        {

            idFound = true;
            break;

        }
    }

    // If the ID was not in the shared memory segment, add it so that
    // others will know.
    if(!idFound)
    {

        const CharAllocator charAllocInstance(shm->get_segment_manager());
        m->push_back(MyString(strId.begin(), strId.end(), charAllocInstance));

    }

    myMutex.unlock(); // Release Lock.

}