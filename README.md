# Pharmacy Management System



https://user-images.githubusercontent.com/46636441/169282481-1b0d8f2a-f800-4eba-b178-d283ba31a5aa.mp4


# Purpose
Pharmacy Management System is a command-line application that is being developed to
handle the inventory management of medicinal products at local pharmacist location.

Three primary reasons for its development are as follows:
- To immerse myself in the Boost C++ Library
- To gain educational experience with back-end integration in C++
- To refine my knowledge of OOP in C++; abstract classes, virtual methods, and more

# Requirements
In order to run this application, it is recommended that you install the following
third-party libraries and/or dependencies on your local machine:

- Boost 1.71.0 or higher
- PGAdmin 4 6.4 or higher
- PostgreSQL 14.2 or higher
- GCC 9.3 or higher
- Libpqxx 7.1.1 or higher

Libpqxx 7.1.1 utilizes functionality that requires C++ 17 or higher. Therefore, it is
recommended to compile using C++ 17 at a minimum.

# Preparing to Compile
There are two additional steps that are required to compile this application. 
  (1) Export **medicine_schema.sql** inside PGAdmin.
  (2) Update **psql_config.conf** so that it is consistent with your database configuration.
      
Failure to complete these steps will result in additional exceptions. The first step is self
explanatory, however I will briefly explain step (2) below. **psql_config.conf** contains
several lines that handle database configuration, respectively. They are formatted as the following:

```
<field_name>=<field_value>
```
This application expects <field_name> to remain unchanged. Failure to do so, will result in a
configuration error when starting this application. Conversely, <field_value> should be updated
to correspond to your database server settings.

# Compiling this Application
This application has only been tested under the G++ compiler. Furthermore, In order to compile 
this application, you will also need to link several libraries. The compiler flags used for 
this are as follows:

```
-lpqxx -lpq -l rt -lboost_thread -pthread 
```
The full command line statement that you should issue will follow the following format (assuming you are in the root directory):

```
g++ Main.cpp -lpqxx -lpq -l rt -lboost_thread -pthread -o Main
```
Subsequently you may run the executable as: **./Main**  
Conclusively, it is possible you may encounter permission-based errors if you are not an adminstrator or super-user on your local machine.
To address this on a Linux or Unix based machine, you may use the chmod command on the yielded executable:

```
chmod +x Main
```

# Future Plans
* Implement inter-process communication for in-memory data synchronization. I would like to look into **Boost.Interprocess** to synchronize
  data through shared memory segments. Currently, data synchronization is handling by loading all data relations in parallel while switching
  terminal screens. As you can imagine, this is a very computationally heavy process. It would be much more ideal to pass data from process to
  process and only update the data in-memory that was modified.

  **EDIT**: This was implemented, however the efficiency of the implementation could be improved. This will be described in the demo video
            embedded above. However, run-time has significantly been improved. All data relations are no longer loaded except at start-time.
            Two vectors are used to facilitate data synchronization within a shared memory segment or region. This is based on the allocated
            amount of memory and currently does not expand. If too many operations are encountered, the memory capacity will be exceeded
            and an std::bad_alloc exception will be thrown. I am currently looking for ways around this. A potential thought is to load
            all data relations or a partition of the data relations from PostgreSQL if the exception is raised. This exception could be caught
            this way.

* Expand the system to allocate stock, inventory and prescriptions to customers based on ID.

* Bubble Sort is currently used to sort data to ensure stability regarding data (relative data ordering). This is a quadratic time algorithm and
  I would like to pivot to an O(NLogN) algorithm such as Merge Sort or Quick Sort for optimal performance with sufficiently large values of N.
  where N is the total number of medicine relations.

  **Edit**: Pivoted from Bubble Sort to Heap sort leveraging a priority_queue with a custom comparator (via a function object). This improves  
            the time complexity from a worst case of O(N^2) to O(NLogN). Conversely, the space complexity has a slight increase from an iterative  
            Bubble sort algorithm as it jumps from constant time to linear time.  


