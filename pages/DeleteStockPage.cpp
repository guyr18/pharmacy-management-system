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