#ifndef MED_H
#define MED_H

#include <iostream>
#include <string>

class Medicine
{

    public:

        unsigned int _id; // integer identifier of this medicine.
        std::string _name; // Name of this medicine.
        std::string _ownedBy; // Company that owns this medicine.
        std::string _arrivalDate; // Arrival date of this medicine.
        std::string _expirationDate; // Expiration date of this medicine.
        double _price; // Cost of this medicine.
        unsigned int _qty; // Quantity of this medicine available.

        // Default constructor; initializes all member variables.

        Medicine();
        Medicine(const unsigned int id, const std::string name, const std::string ob, const std::string ad, const std::string ed, const double p, const int q);

        // Default destructor.
        ~Medicine();

};

#endif