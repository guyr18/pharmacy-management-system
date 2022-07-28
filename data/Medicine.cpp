#pragma once
#include "headers/Medicine.h"
#include "../globals/Utils.cpp"

// Default constructor; parameterless.
Medicine::Medicine() : _id{0}, _price{0.0}, _qty{0} {}

// Default constructor; initializes all member variables.
Medicine::Medicine(const unsigned int id, const std::string name, const std::string ob, const std::string ad, const std::string ed, const double p, const int q)
{

    _id = id;
    _name = name;
    _ownedBy = ob;
    _arrivalDate = ad;
    _expirationDate = ed;
    _price = p;
    _qty = q;

}

// Default destructor.
Medicine::~Medicine()
{

    _id = 0;
    _name = "";
    _ownedBy = "";
    _price = 0.0;
    _qty = 0;

}

// Print(level) prints a description of this class to the standard output terminal,
// according to restrictions specified by parameter level. These restrictions are
// imposed by the level constants that are statically embedded within this class and are as follows:
//
//  1. L_ALL -> All properties will be printed.
//  2. L_ALL_EXCL_ID -> All properties, excluding the _id field, will be printed.
void Medicine::print(const unsigned int level, bool printHeader) const
{

    std::string closingLine;

    // Print all attributes.
    if(level == L_ALL)
    {

        closingLine = "---------------------------------------------------------------------------------------------------------------------------------";

        if(printHeader)
        {

            std::cout << "---------------------------------------------------------------------------------------------------------------------------------" << std::endl;
            std::cout << "| ID           | Name          | Company          | Arrival Date         | Expire Date         | Price          | Qty           |" << std::endl;
            std::cout << closingLine << std::endl;

        }

        std::cout << "| " << _id;
        Utils::getInstance().printNSpaces(Utils::getInstance().abs(std::to_string(_id).size() - 13));
        std::cout << "| " << _name;
        Utils::getInstance().printNSpaces(Utils::getInstance().abs(_name.size() - 14));
        std::cout << "| ";
        std::cout << _ownedBy;
        Utils::getInstance().printNSpaces(Utils::getInstance().abs(_ownedBy.size() - 17));
        std::cout << "| ";
        std::cout << _arrivalDate;
        Utils::getInstance().printNSpaces(Utils::getInstance().abs(_arrivalDate.size() - 21));
        std::cout << "| ";
        std::cout << _expirationDate;
        Utils::getInstance().printNSpaces(Utils::getInstance().abs(_expirationDate.size() - 20));
        std::cout << "| ";
        std::cout << std::fixed << std::setprecision(2) << _price;
        Utils::getInstance().printNSpaces(Utils::getInstance().abs(std::to_string(_price).size() - 19));
        std::cout << "| ";
        std::cout << _qty;
        Utils::getInstance().printNSpaces(Utils::getInstance().abs(std::to_string(_qty).size() - 13));
        std::cout << " |" << std::endl;

    // Two cases currently; if it is not L_ALL, then it must be L_ALL_EXCL_ID.
    }
    else
    {

        closingLine = "------------------------------------------------------------------------------------------------------------------";

        if(printHeader)
        {

            std::cout << "------------------------------------------------------------------------------------------------------------------" << std::endl;
            std::cout << "| Name          | Company          | Arrival Date         | Expire Date         | Price          | Qty           |" << std::endl;
            std::cout << closingLine << std::endl;

        }

        std::cout << "| " << _name;
        Utils::getInstance().printNSpaces(Utils::getInstance().abs(_name.size() - 14));
        std::cout << "| ";
        std::cout << _ownedBy;
        Utils::getInstance().printNSpaces(Utils::getInstance().abs(_ownedBy.size() - 17));
        std::cout << "| ";
        std::cout << _arrivalDate;
        Utils::getInstance().printNSpaces(Utils::getInstance().abs(_arrivalDate.size() - 21));
        std::cout << "| ";
        std::cout << _expirationDate;
        Utils::getInstance().printNSpaces(Utils::getInstance().abs(_expirationDate.size() - 20));
        std::cout << "| ";
        std::cout << std::fixed << std::setprecision(2) << _price;
        Utils::getInstance().printNSpaces(Utils::getInstance().abs(std::to_string(_price).size() - 19));
        std::cout << "| ";
        std::cout << _qty;
        Utils::getInstance().printNSpaces(Utils::getInstance().abs(std::to_string(_qty).size() - 14));
        std::cout << "|" << std::endl;

    }
}