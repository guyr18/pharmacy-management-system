#pragma once
#include "../globals/Pages.cpp"
#include "BuyMedicinePage.cpp"
#include "../globals/DBConfig.cpp"
#include "headers/AddStockPage.h"
#include <boost/date_time.hpp>
#include <string>
#include <iostream>

// Default constructor.
AddStockPage::AddStockPage() { }

// Default destructor.
AddStockPage::~AddStockPage() { }


// Monitor() monitors() user input.
void AddStockPage::monitor()
{
    
    // ERROR CHECKING is NOT done for every field; Assumed certain level of competency when inserting records.
    // If an error is made, it can be corrected through the update function; main menu (5).

    // Declare input variables to send to input stream.
    std::string name, cmp, ad, ed, price, qty;

    std::cout << "Enter Name: ";
    std::cin.ignore();
    std::getline(std::cin, name);
    std::cout << "Enter Company: ";
    std::getline(std::cin, cmp);
    std::cout << "Enter Arrival Date (DD-MM-YYYY): ";
    std::getline(std::cin, ad);
    std::cout << "Enter Expiration Date (DD-MM-YYYY): ";
    std::getline(std::cin, ed);
    std::cout << "Enter Price: ";
    std::getline(std::cin, price);
    std::cout << "Enter Quantity: ";
    std::getline(std::cin, qty);

    double castedPrice = Utils::getInstance().isStringDouble(price) ? std::stof(price) : 0.0;
    int castedQty = Utils::getInstance().isStringInteger(qty) ? std::stoi(qty) : 0;
    std::string strCurDate = "";

    // If the arrival date or expiration date receives invalid input, default
    // each one, respectively to the current date in the following format:
    //
    //  DD-MM-YYYY.
    if(!Utils::getInstance().isStringDate(ad))
    {

        strCurDate = Utils::getInstance().getCurrentDateAsString();
        ad = strCurDate;

    }

    if(!Utils::getInstance().isStringDate(ed))
    {

        strCurDate = strCurDate.size() == 0 ? Utils::getInstance().getCurrentDateAsString() : strCurDate;
        ed = strCurDate;

    }

    Medicine m{static_cast<unsigned int>(MedicineManager::getInstance().getData().size() + 1), name, cmp, ad, ed, castedPrice, castedQty};

    // Make sure this isn't a duplicate entry, if it isn't we add it.
    if(MedicineManager::getInstance().isUniqueName(name))
    {

        MedicineManager::getInstance().add(m);

    }


    std::string q1 = "INSERT INTO public.medicines(id, name, \"ownedBy\", \"arrivalDate\", \"expireDate\", price, qty) VALUES (";
    std::string q2 = std::to_string(m._id) + ", \'" + m._name + "\', \'" + m._ownedBy + "\', \'" + m._arrivalDate + "\', \'" + m._expirationDate
                     + "\', " + std::to_string(castedPrice) + ", " + std::to_string(castedQty) + ");";
    std::string q3 = q1 + q2;

    SQLConnection& conn = *DBConfig::getInstance().connObj.get();
    conn.connect();
    conn.insert(q3);
    conn.disconnect();
    system("clear");
    Pages::getInstance().MAIN.log();
    return;

}

// Log() will log medicine transaction details.
void AddStockPage::log()
{

    std::cout << "Add Product To Stock" << std::endl << std::endl;

}