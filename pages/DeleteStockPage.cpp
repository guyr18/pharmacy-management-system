#pragma once
#include "../globals/Pages.cpp"
#include "../data/MedicineManager.cpp"
#include "../globals/Utils.cpp"
#include "../globals/DBConfig.cpp"
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

    const size_t n = MedicineManager::getInstance().getData().size();
    bool emptyCache = n == 0;
    std::string input;
    bool inputReq = false;
    int intId = -1;

    // Obtain product ID as input
    while(true && !emptyCache)
    {

        std::cout << "Enter a Product ID to delete: ";
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

                std::cout << "Invalid value specified for Product ID; ID out of bounds." << std::endl;

            }
        }
        else
        {

            std::cout << "Invalid value specified for Product ID; please enter an integer value." << std::endl;
            continue;

        }

    }

    std::string confirmation;
    bool performDelete = false;

    // Ask user to confirm their decision.
    while(true && !emptyCache)
    {

        std::cout << "Are you sure you would like to delete Product [ID=" << intId << "]? This CANNOT BE UNDONE. (y / n): ";
        std::getline(std::cin, confirmation);
            
        if(confirmation == "Y" || confirmation == "y")
        {

            performDelete = true;
            handleDeleteRelation(intId);
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
    }
}

// HandleDeleteRelation(id) deletes a relation from the appropriate database table
// given the primary key id.
void DeleteStockPage::handleDeleteRelation(const unsigned int& id)
{

    SQLConnection& conn = *DBConfig::getInstance().connObj.get();
    conn.connect();
    conn.fetch("DELETE * FROM public.medicines WHERE id=" + std::to_string(id));
    conn.disconnect();

}