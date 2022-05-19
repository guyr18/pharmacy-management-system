#pragma once
#include "headers/ShowItemListPage.h"
#include "../globals/Pages.cpp"
#include "../data/Medicine.cpp"
#include "../data/MedicineManager.cpp"
#include "../globals/Utils.cpp"
#include "../db/SQLConnection.cpp"
#include <boost/container/vector.hpp>
#include <string>
#include <iostream>
#include <stdio.h>
#include <limits>
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
ShowItemListPage::ShowItemListPage() {}

// Default destructor.
ShowItemListPage::~ShowItemListPage() {}

// CheckSharedMemory() is run by a separate thread of execution.
// It checks the shared memory region for any data changes before
// executing the remainder of the .monitor() method.
void ShowItemListPage::checkSharedMemory()
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
void ShowItemListPage::monitor()
{

    std::string input;

    // Start thread and block until shared memory changes
    // have been merged.
    boost::thread workerThread(&ShowItemListPage::checkSharedMemory, this);
    workerThread.join();
    log();

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

}   

// Log() will log the current item list.
void ShowItemListPage::log()
{

    std::cout << "Item List Center" << std::endl;
    MedicineManager& ref = MedicineManager::getInstance();
    boost::container::vector<Medicine> cache = ref.getData();
    int count = 0;

    for(const Medicine& m : cache)
    {

        bool printHeader = count == 0 ? true : false;
        m.print(Medicine::L_ALL, printHeader);
        count++;

    }

    if(cache.size() == 0)
    {

        std::cout << std::endl << "There are no products currently in the database." << std::endl;
        
    }
    else
    {

        std::cout << "---------------------------------------------------------------------------------------------------------------------------------" << std::endl;

    }

    std::cout << std::endl << "Press 'm' to return to the main menu..." << std::endl;
  
}