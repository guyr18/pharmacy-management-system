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