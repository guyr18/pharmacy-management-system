#pragma once
#include "headers/Medicine.h"

// Default constructor; parameterless.
Medicine::Medicine() : _id{0} {}

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