#ifndef MED_MGR_H
#define MED_MGR_H

#include "Medicine.h"
#include <boost/container/vector.hpp>

class MedicineManager
{

    private:


        boost::container::vector<Medicine> _data; // List of Medicine objects.
        Medicine DUMMY_MED;

        // MedicineManager object is a singleton; suppress constructors and assignments.
        MedicineManager();
        MedicineManager(const MedicineManager& rhs) = delete;
        MedicineManager(const MedicineManager&& rhs) = delete;
        MedicineManager& operator=(const MedicineManager& rhs) = delete;
        MedicineManager& operator=(const MedicineManager&& rhs) = delete;

    public:

        // Default destructor.
        ~MedicineManager();

        // GetInstance() returns a single copy of MedicineManager for the
        // life-time of this application.
        static MedicineManager& getInstance();

        // Add(m) adds a Medicine object, m to _data.
        void add(const Medicine& m);

        // Contains(id) returns true if a Medicine object with id, 'id'
        // exists in _data. And otherwise, false.
        bool contains(const unsigned int id);

        // RemoveById(id) removes any Medicine object that contains id, 'id'.
        void removeById(const unsigned int id);

        // GetById(id) returns the Medicine object with id, 'id'. If no match is
        // found, @see DUMMY_MED is returned.
        Medicine& getById(const unsigned int id);

        // IsUniqueName(name) returns true if a Medicine object has the name property == 'name'.
        // And otherwise, false.
        bool isUniqueName(const std::string name);

        // BubbleSortById() sorts @see _data in ascending order using
        // a bubble sort algorithm on the variable _id.
        void bubbleSortById();

        // GetData() returns the cached list of Medicine object(s), @see _data.
        boost::container::vector<Medicine>& getData();
    
};

#endif
