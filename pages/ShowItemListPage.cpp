#pragma once
#include "headers/ShowItemListPage.h"
#include "../globals/Pages.cpp"
#include "../data/Medicine.cpp"
#include "../data/MedicineManager.cpp"
#include "../globals/Utils.cpp"
#include <boost/container/vector.hpp>
#include <string>
#include <iostream>
#include <stdio.h>
#include <limits>

// Default constructor.
ShowItemListPage::ShowItemListPage() {}

// Default destructor.
ShowItemListPage::~ShowItemListPage() {}


// Monitor() monitors() user input.
void ShowItemListPage::monitor()
{

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
        m.print(Medicine::L_ALL_EXCL_ID, printHeader);
        count++;

    }

    if(cache.size() == 0)
    {

        std::cout << std::endl << "There are no products currently in the database." << std::endl;
        
    }
    else
    {

        std::cout << "------------------------------------------------------------------------------------------------------------------" << std::endl;

    }

    std::cout << std::endl << "Press 'm' to return to the main menu..." << std::endl;
  
}