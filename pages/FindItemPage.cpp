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

// Default constructor.
FindItemPage::FindItemPage() {}

// Default destructor.
FindItemPage::~FindItemPage() { }


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

            idSearchHandler();

        // Search by name.
        }
        else
        {

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
    std::cout << std::endl << "There are currently " << numItems << " items in the database. Please enter an identifier between 1 and " << numItems << ": ";

    while(true)
    {

        std::string idEntered;
        std::getline(std::cin, idEntered);

        try
        {

            convId = std::stoi(idEntered);
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
            const int numPrefixes = prefixes.size();
            std::cout << std::endl << "Specify an item no. to view in more detail; between 1 and " << numPrefixes << "." << std::endl;
        
            while(true)
            {

                std::string itemno;
                int count = 0;
                std::getline(std::cin, itemno);
            
                try
                {
                
                    convItemNo = std::stoi(itemno);

                    if(convItemNo >= 1 && convItemNo <= numPrefixes)
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