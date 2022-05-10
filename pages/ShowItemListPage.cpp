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

// Default constructor.
ShowItemListPage::ShowItemListPage() {}

// Default destructor.
ShowItemListPage::~ShowItemListPage() {}


void handleLoadData()
{

    boost::mutex myMutex;
    myMutex.lock();
    SQLConnection& conn = *DBConfig::getInstance().connObj.get();
    conn.connect();
    pqxx::result r = conn.fetch("SELECT * FROM medicines");
    MedicineManager::getInstance().clear();

    for(const pqxx::row& row : r)
    {

        Medicine m{row[0].as<unsigned int>(), row[1].as<std::string>(), row[2].as<std::string>(), row[3].as<std::string>(), row[4].as<std::string>(), row[5].as<double>(), row[6].as<int>()};
        MedicineManager::getInstance().add(m);

    }

    conn.disconnect();
    myMutex.unlock();
    
}

// Monitor() monitors() user input.
void ShowItemListPage::monitor()
{

    std::string input;
    boost::thread workerThread(handleLoadData);
    workerThread.join();

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