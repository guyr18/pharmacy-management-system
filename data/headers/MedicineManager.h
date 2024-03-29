#ifndef MED_MGR_H
#define MED_MGR_H

#include "Medicine.h"
#include <boost/container/vector.hpp>

class MedicineManager
{

    private:


        boost::container::vector<Medicine> _data; // List of Medicine objects.
        Medicine DUMMY_MED{0, "", "", "", "", 0.0, 0};

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

        // Clear() clears @see _data.
        void clear();

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

        // HeapSortById() sorts @see _data in ascending order using a heap sort
        // algorithm; this method executes in the following time:
        // Time Complexity: O(NLogN)
        // Space Complexity O(N)
        void heapSortById();

        // SyncItemProperty(id, field) locates the Medicine object with a
        // unique identifier of id and sets it member variable field to 
        // val.
        void syncItemProperty(const unsigned int id, const std::string field, const std::string val);

        // SyncItemProperty(id, field, val) locates the Medicine object with a
        // unique identifier of id and sets it member variable field to 
        // val. This is a function overload intended to avoid an implicit conversion.
        void syncItemProperty(const unsigned int id, const std::string field, const double val);

        // SyncItemProperty(id, field, val) locates the Medicine object with a
        // unique identifier of id and sets it member variable field to 
        // val. This is a function overload intended to avoid an implicit conversion.
        void syncItemProperty(const unsigned int id, const std::string field, const int val);

        // GetData() returns the cached list of Medicine object(s), @see _data.
        boost::container::vector<Medicine>& getData();
    
};

#endif
