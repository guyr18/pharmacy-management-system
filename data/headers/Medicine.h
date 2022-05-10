#ifndef MED_H
#define MED_H

#include <iostream>
#include <string>
#include "../../interfaces/IPrintable.h"

class Medicine : public IPrintable
{

    public:

        // LEVEL CONSTANTS for .print()
        const static unsigned int L_ALL = 0; // Print all properties.
        const static unsigned int L_ALL_EXCL_ID = 1; // Print all properties except the ID.

        unsigned int _id; // integer identifier of this medicine.
        std::string _name; // Name of this medicine.
        std::string _ownedBy; // Company that owns this medicine.
        std::string _arrivalDate; // Arrival date of this medicine.
        std::string _expirationDate; // Expiration date of this medicine.
        double _price; // Cost of this medicine.
        unsigned int _qty; // Quantity of this medicine available.

        // Default parameterless constructor.
        Medicine();

        // Default constructor; initializes all member variables.
        Medicine(const unsigned int id, const std::string name, const std::string ob, const std::string ad, const std::string ed, const double p, const int q);

        // Default destructor.
        ~Medicine();

        // Print(level) prints a description of this class to the standard output terminal,
        // according to restrictions specified by parameter level. These restrictions are
        // imposed by the level constants that are statically embedded within this class and are as follows:
        //
        //  1. L_ALL -> All properties will be printed.
        //  2. L_ALL_EXCL_ID -> All properties, excluding the _id field, will be printed.
        void print(const unsigned int level, const bool printHeader) const;

};

#endif