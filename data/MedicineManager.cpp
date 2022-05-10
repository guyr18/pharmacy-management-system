#pragma once
#include "headers/MedicineManager.h"
#include <cmath>

// Default constructor.
MedicineManager::MedicineManager() {}

// Default destructor.
MedicineManager::~MedicineManager()
{

    _data.clear();

}

// GetInstance() returns a single copy of MedicineManager for the
// life-time of this application.
MedicineManager& MedicineManager::getInstance()
{

    static MedicineManager inst;
    return inst;

}

 // Add(m) adds a Medicine object, m to _data.
void MedicineManager::add(const Medicine& m)
{

    try
    {
        
        _data.push_back(m);

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;

    }
}

// Contains(id) returns true if a Medicine object with id, 'id'
// exists in _data. And otherwise, false.
bool MedicineManager::contains(const unsigned int id)
{

    return getById(id)._id == id;

}

// RemoveById(id) removes any Medicine object that contains id, 'id'.
void MedicineManager::removeById(const unsigned int id)
{

    for(unsigned int i = 0; i < _data.size(); i++)
    {

        if(_data[i]._id == id)
        {

            _data.erase(_data.begin() + i);
            break;

        }
    }
}


// GetById(id) returns the Medicine object with id, 'id'. If no match is
// found, @see DUMMY_MED is returned.
Medicine& MedicineManager::getById(const unsigned int id)
{

    unsigned int left = 0;
    unsigned int right = _data.size() - 1;

    while(left <= right)
    {

        int mid = left + (right - left) / 2;

        if(_data[mid]._id == id)
        {

            return _data[mid];

        }
        else if(_data[mid]._id < id)
        {

            left = mid + 1;

        }
        else
        {

            right = mid - 1;
            
        }
    }

    return DUMMY_MED;

}

// IsUniqueName(name) returns false if a Medicine object has the name property == 'name'.
// And otherwise, true.
bool MedicineManager::isUniqueName(const std::string name)
{

    for(const Medicine& m : _data)
    {

        if(m._name == name)
        {

            return false;

        }
    }

    return true;
    
}

// SyncItemProperty(id, field) locates the Medicine object with a
// unique identifier of id and sets it member variable field to 
// val.
void MedicineManager::syncItemProperty(const unsigned int id, const std::string field, const std::string val)
{

    Medicine& ref = this->getById(id);

    if(field == "name")
    {

        ref._name = val;

    }
    else if(field == "arrival_date")
    {

        ref._arrivalDate = val;

    }
    else if(field == "expire_date")
    {

        ref._expirationDate = val;

    }
    else if(field == "owned_by")
    {

        ref._ownedBy = val;

    }
    else
    {

        std::cerr << "Invalid field name specified." << std::endl;

    }
}

void MedicineManager::syncItemProperty(const unsigned int id, const std::string field, const double val)
{

    Medicine& ref = this->getById(id);

    if(field == "price")
    {

        ref._price = val;

    }
    else
    {

        std::cerr << "Invalid field name specified." << std::endl;
        
    }
}


void MedicineManager::syncItemProperty(const unsigned int id, const std::string field, const int val)
{


    Medicine& ref = this->getById(id);

    if(field == "qty")
    {

        ref._qty = val;

    }
    else
    {

        std::cerr << "Invalid field name specified." << std::endl;
        
    }
}

// BubbleSortById() sorts @see _data in ascending order using a bubble sort
// algorithm.
void MedicineManager::bubbleSortById()
{

    const size_t n = _data.size();

    for(int i = 0; i < n - 1; i++)
    {

        for(int j = 0; j < n - i - 1; j++)
        {

            if(_data[j]._id >= _data[j + 1]._id)
            {

                std::swap(_data[j], _data[j + 1]);

            }
        }
    }
}

// GetData() returns the cached list of Medicine object(s), @see _data.
boost::container::vector<Medicine>& MedicineManager::getData()
{

    return _data;

}