#pragma once
#include "../globals/Pages.cpp"
#include "headers/BuyMedicinePage.h"
#include "../data/MedicineManager.cpp"
#include "../data/Medicine.cpp"
#include "../globals/Utils.cpp"
#include "../globals/DBConfig.cpp"
#include <string>
#include <iostream>
#include <boost/container/vector.hpp>
#include <boost/tuple/tuple.hpp>

// Default constructor.
BuyMedicinePage::BuyMedicinePage() {}

// Default destructor.
BuyMedicinePage::~BuyMedicinePage() { }


// Monitor() monitors() user input.
void BuyMedicinePage::monitor()
{

    boost::container::vector<int> idVect;
    boost::container::vector<boost::tuple<int, int>> newQty;
    std::string id;
    std::cin.ignore();

    while(true)
    {

        std::cout << "Enter item ID (press q to accept selections): ";
        std::getline(std::cin, id);

        if(id == "q") { break; }

        // See if we can convert it to an integer without raising
        // an exception. If we can, it is a valid number and we
        // can add it to our selection vector. If it isn't, just
        // ignore it.
        try
        {
            
            int temp = std::stoi(id);

            if(temp >= 1 && temp <= MedicineManager::getInstance().getData().size())
            {

                Medicine& m = MedicineManager::getInstance().getById(temp);

                if(m._qty > 0)
                {

                    boost::tuple<int, int> tup = boost::make_tuple(temp, m._qty - 1);
                    idVect.push_back(temp);
                    newQty.push_back(tup);

                }
                else
                {

                    std::cout << "Item no. " << temp << " is not in stock." << std::endl;

                }
            }
            else
            {

                std::cout << "Invalid ID specified." << std::endl;

            }
        }
        catch(const std::exception& e)
        { }

    }

    std::cout << std::endl <<  "The following IDs were selected: [";
    int i = 0, totalPurchasePrice = 0;

    for(auto itr = idVect.begin(); itr != idVect.end(); itr++, i++)
    {

        double tempPrice = MedicineManager::getInstance().getById(*itr)._price;
        totalPurchasePrice += tempPrice;

        if(i == idVect.size() - 1)
        {

            std::cout << *itr;

        }
        else
        {

            std::cout << *itr << ", ";

        }
    }


    std::cout << "]" << std::endl;
    std::cout << "Total Purchase Price: " << totalPurchasePrice << std::endl;
    SQLConnection& conn = *DBConfig::getInstance().connObj.get();
    conn.connect();

    for(const boost::tuple<int, int> tup : newQty)
    {

        std::string strId = std::to_string(tup.get<0>());
        std::string strQty = std::to_string(tup.get<1>());
        conn.insert("UPDATE public.medicines SET qty=" + strQty + " WHERE id=" + strId + ";");

    }

    conn.disconnect();
    std::cout << std::endl << "Press 'm' to return to the main menu..." << std::endl;
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
    return;
    
}

// Log() will log medicine transaction details.
void BuyMedicinePage::log()
{

    std::cout << "Medicine Transaction" << std::endl << std::endl;
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