#pragma once
#include "../globals/Pages.cpp"
#include "headers/FindItemPage.h"
#include "../data/MedicineManager.cpp"
#include "../data/Medicine.cpp"
#include "../globals/Utils.cpp"
#include <string>
#include <iostream>
#include <boost/container/vector.hpp>
#include <boost/tuple/tuple.hpp>
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
FindItemPage::FindItemPage() {}

// Default destructor.
FindItemPage::~FindItemPage() { }

// CheckSharedMemory() is run by a separate thread of execution.
// It checks the shared memory region for any data changes before
// executing the remainder of the .monitor() method.
void FindItemPage::checkSharedMemory()
{

    boost::mutex myMutex;
    myMutex.lock();

    try
    {
        
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
void FindItemPage::monitor()
{

    int mode = -1;
    bool firstPhase = true;
    bool emptyCache = MedicineManager::getInstance().getData().size() == 0;
    std::string input;
    std::cin.ignore();

    while(firstPhase && !emptyCache)
    {

        std::cout << std::endl << "Press 1 to search by ID or 2 to search by name (or prefix); any value not 1 or 2 will assume 2." << std::endl;
        std::getline(std::cin, input);
        mode = input == "1" ? 1 : 2;
        std::string action = mode == 1 ? "ID search" : "name search";

        while(true)
        {

            std::string yesOrNo;
            std::cout << std::endl << "Confirm " << action << " (y/n)" << std::endl;
            std::getline(std::cin, yesOrNo);

            if(yesOrNo == "Y" || yesOrNo == "y")
            {

                firstPhase = false;
                break;

            }
            else if(yesOrNo == "N" || yesOrNo == "n")
            {

                break;

            }
            else
            {

                std::cout << "Invalid input; please use (Y/y/N/n)" << std::endl;
                
            }
        }
    }
    
    // No search should happen if the cache is empty.
    if(!emptyCache)
    {

        // Search by ID.
        if(mode == 1)
        {

            boost::thread memoryThread{&FindItemPage::checkSharedMemory, this};
            memoryThread.join();
            idSearchHandler();

        // Search by name.
        }
        else
        {

            boost::thread memoryThread{&FindItemPage::checkSharedMemory, this};
            memoryThread.join();
            nameSearchHandler();

        }
    }
    else
    {

        std::cout << std::endl << "There are no products currently in the database." << std::endl;

    }

    if(!emptyCache)
    {

        std::cout << std::endl << "Press 'm' to return to the main menu or press 'l' to look up another item." << std::endl;

    }
    else
    {

        std::cout << std::endl << "Press 'm' to return to the main menu.." << std::endl;

    }

    std::string input2;
    bool lookupReq = false;

    while(true && !emptyCache)
    {
        

        std::cin >> input2;

        if(input2 == "m")
        {

            break;

        }

        if(input2 == "l")
        {

            lookupReq = true;
            break;

        }

        std::cout << "Invalid key pressed; you pressed " << input2 << std::endl;

    }

    while(true && emptyCache)
    {
        

        std::cin >> input2;

        if(input2 == "m")
        {

            break;

        }

        std::cout << "Invalid key pressed; you pressed " << input2 << std::endl;

    }

    if(lookupReq)
    {

        monitor();

    }
    else
    {

        system("clear");
        Pages::getInstance().MAIN.log();

    }
}

// Log() will log medicine transaction details.
void FindItemPage::log()
{

    std::cout << "Item Search Center" << std::endl;

}

// IdSearchHandler() handles the algorithm related to id lookup.
void FindItemPage::idSearchHandler()
{


    int convId = -1;
    const size_t numItems = MedicineManager::getInstance().getData().size();
    unsigned int maxId = MedicineManager::getInstance().getData().at(numItems - 1)._id;
    std::cout << std::endl << "There are currently " << numItems << " items in the database. Please enter an identifier between 1 and " << maxId << ": ";

    while(true)
    {

        std::string idEntered;
        std::getline(std::cin, idEntered);

        try
        {

            convId = std::stoi(idEntered);

            if(convId < 1 || convId > maxId)
            {

                std::cout << "Invalid ID entered." << std::endl;
                continue;

            }

            break;

        }
        catch(const std::exception& e)
        {
                
            std::cout << "Invalid ID entered.";
            continue;

        }   
    }

    const Medicine& m = MedicineManager::getInstance().getById(convId);
    m.print(Medicine::L_ALL, true);
    std::cout << "---------------------------------------------------------------------------------------------------------------------------------" << std::endl;
  
}

// NameSearchHandler() handles the algorithm related to name lookup.
void FindItemPage::nameSearchHandler()
{

        boost::container::vector<Medicine> prefixes;
        std::string prefix;
        std::cout << std::endl << "Enter a name or prefix of a product to search for: ";
        std::getline(std::cin, prefix);

        for(const Medicine& m : MedicineManager::getInstance().getData())
        {

            const std::string& strName = m._name;
            int i = 0, j = 0;

            if(std::tolower(strName[0], std::locale()) != std::tolower(prefix[0], std::locale())) { continue; }

            for(const auto& ch : strName)
            {

                if(std::tolower(ch, std::locale()) == std::tolower(prefix[j], std::locale()))
                {
                    
                    i++, j++;

                }
            }

            if(i == prefix.size())
            {

                prefixes.push_back(m);

            }
        }

        if(prefixes.size() == 0)
        {

            std::cout << std::endl << "No items match this criteria." << std::endl;

        }
        else
        {

            int k = 1;

            for(const Medicine& m2 : prefixes)
            {

                std::cout << std::endl << "Item no. " << k << " [ID=" << m2._id << "] => " << m2._name << std::endl;
                k++;
                
            }

            int convItemNo = -1;
            const size_t numPrefixes = MedicineManager::getInstance().getData().size();
            unsigned int maxId = MedicineManager::getInstance().getData().at(numPrefixes - 1)._id;
            std::cout << std::endl << "Specify an item no. to view in more detail; between 1 and " << maxId << "." << std::endl;
        
            while(true)
            {

                std::string itemno;
                int count = 0;
                std::getline(std::cin, itemno);
            
                try
                {
                
                    convItemNo = std::stoi(itemno);

                    if(convItemNo >= 1 && convItemNo <= maxId)
                    {

                        const Medicine& m2 = MedicineManager::getInstance().getById(prefixes.at(convItemNo - 1)._id);
                        bool printHeader = count == 0 ? true : false;
                        m2.print(Medicine::L_ALL, printHeader);
                        std::cout << "---------------------------------------------------------------------------------------------------------------------------------" << std::endl;
                        count++;
                        break;

                    }
                
                    std::cout << "Invalid item no. specified; please try again between range [1, " << numPrefixes << "]: ";
                    continue;


                }
                catch(const std::exception& e)
                {
                

                    std::cout << "Invalid item no. specified; make sure you\'re entering a number." << std::endl;

                }
            }
        }
}