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
ShowItemListPage::~ShowItemListPage() { }


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

    std::cout << "------------------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "| Name          | Company          | Arrival Date         | Expire Date         | Price          | Qty           |" << std::endl;
    std::cout << "------------------------------------------------------------------------------------------------------------------" << std::endl;

    MedicineManager& ref = MedicineManager::getInstance();
    boost::container::vector<Medicine> cache = ref.getData();

    for(const Medicine& m : cache)
    {

        std::cout << "| " << m._name;
        Utils::getInstance().printNSpaces(Utils::getInstance().abs(m._name.size() - 14));
        std::cout << "| ";
        std::cout << m._ownedBy;
        Utils::getInstance().printNSpaces(Utils::getInstance().abs(m._ownedBy.size() - 17));
        std::cout << "| ";
        std::cout << m._arrivalDate;
        Utils::getInstance().printNSpaces(Utils::getInstance().abs(m._arrivalDate.size() - 21));
        std::cout << "| ";
        std::cout << m._expirationDate;
        Utils::getInstance().printNSpaces(Utils::getInstance().abs(m._expirationDate.size() - 20));
        std::cout << "| ";
        std::cout << m._price;
        Utils::getInstance().printNSpaces(Utils::getInstance().abs(std::to_string(m._price).size() - 22));
        std::cout << "| ";
        std::cout << m._qty;
        Utils::getInstance().printNSpaces(Utils::getInstance().abs(std::to_string(m._qty).size() - 14));
        std::cout << "|" << std::endl;

    }

    std::cout << "------------------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << std::endl << "Press 'm' to return to the main menu..." << std::endl;
  
}