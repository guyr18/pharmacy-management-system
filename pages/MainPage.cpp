#pragma once
#include "../globals/Pages.cpp"
#include "BuyMedicinePage.cpp"
#include "AddStockPage.cpp"
#include "FindItemPage.cpp"
#include "UpdateStockPage.cpp"
#include "DeleteStockPage.cpp"
#include "headers/MainPage.h"
#include <string>
#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

// Default constructor.
MainPage::MainPage() { }

// Default destructor.
MainPage::~MainPage() { }

void handleDataLoad()
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

    size_t n = MedicineManager::getInstance().getData().size();

    if(n > 2)
    {

        MedicineManager::getInstance().bubbleSortById();

    }

    conn.disconnect();
    myMutex.unlock();

}

// Monitor() monitors() user input.
void MainPage::monitor()
{

    while(true)
    {

        std::string input;
        std::cin >> input;

        if(input == "1") // Buy Medicine.
        {
            
            boost::thread workerThread(handleDataLoad);
            workerThread.join();
            system("clear");
            Pages::getInstance().BMP.log();
            Pages::getInstance().BMP.monitor();

        }
        else if(input == "2") // Show Item List.
        {

            system("clear");
            Pages::getInstance().SILP.log();
            Pages::getInstance().SILP.monitor();


        }
        else if(input == "3") // Item database lookup.
        {


            boost::thread workerThread(handleDataLoad);
            workerThread.join();
            system("clear");
            Pages::getInstance().FIP.log();
            Pages::getInstance().FIP.monitor();

        }
        else if(input == "4") // Add product to database.
        {


            boost::thread workerThread(handleDataLoad);
            workerThread.join();
            system("clear");
            Pages::getInstance().ASP.log();
            Pages::getInstance().ASP.monitor();


        }
        else if(input == "5") // Update product attribute.
        {

            boost::thread workerThread(handleDataLoad);
            workerThread.join();
            system("clear");
            Pages::getInstance().USP.log();
            Pages::getInstance().USP.monitor();
            

        }
        else if(input == "6") // Delete product from database.
        {

            boost::thread workerThread(handleDataLoad);
            workerThread.join();
            system("clear");
            Pages::getInstance().DSP.log();
            Pages::getInstance().DSP.monitor();
           
        }
        else if(input == "7") // Exit application.
        {

            return;

        }
        else
        {

            continue;

        }
    }
}

// Log() will log the default commands (main menu) for this application.
void MainPage::log()
{

    std::cout << "1. Buy Medicine " << std::endl;
    std::cout << "2. Show Item List " << std::endl;
    std::cout << "3. Find Item From List " << std::endl;
    std::cout << "4. Add Item In Stock " << std::endl;
    std::cout << "5. Update Stock Item " << std::endl;
    std::cout << "6. Delete Stock Item " << std::endl;
    std::cout << "7. Exit " << std::endl;

}