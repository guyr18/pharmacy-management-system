#pragma once
#include "../globals/Pages.cpp"
#include "../data/MedicineManager.cpp"
#include "headers/DeleteStockPage.h"
#include <string>
#include <iostream>

// Default constructor.
DeleteStockPage::DeleteStockPage() { }

// Default destructor.
DeleteStockPage::~DeleteStockPage() { }

// Monitor() monitors() user input.
void DeleteStockPage::monitor()
{

    bool emptyCache = MedicineManager::getInstance().getData().size() == 0;
    std::string input;
    bool inputReq = false;

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

        std::cout << "Invalid key pressed; you pressed " << input << std::endl;

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

        std::cout << "Invalid key pressed; you pressed " << input << std::endl;

    }

    if(inputReq)
    {

        monitor();

    }
    else
    {

        system("clear");
        Pages::getInstance().MAIN.log();
        
    }
}

// Log() will log medicine deletion details.
void DeleteStockPage::log()
{

    std::cout << "Delete Stock Center" << std::endl;

}