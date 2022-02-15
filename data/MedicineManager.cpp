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

    for(const Medicine& obj : _data)
    {

        if(obj._id == id)
        {

            return true;

        }
    }

    return false;

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

    for(Medicine& m : _data)
    {

        if(m._id == id)
        {

            return m;

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

// GetData() returns the cached list of Medicine object(s), @see _data.
boost::container::vector<Medicine>& MedicineManager::getData()
{

    return _data;

}