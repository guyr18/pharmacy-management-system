#include "db/SQLConnection.cpp"
#include "globals/Utils.cpp"
#include "pages/MainPage.cpp"
#include "pages/ShowItemListPage.cpp"
#include "globals/DBConfig.cpp"
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

using namespace boost::interprocess;
typedef allocator<char, managed_shared_memory::segment_manager> CharAllocator;
typedef basic_string<char, std::char_traits<char>, CharAllocator> MyString;
typedef allocator<MyString, managed_shared_memory::segment_manager> StringAllocator;
typedef boost::interprocess::vector<MyString, StringAllocator> Vect;

// Run() loads all database relations through a synchronized procedure involving a boost::mutex.
void run()
{

    boost::mutex myMutex; // Declare mutex.
    myMutex.lock(); // Block until mutex is available.

    // Establish connection and cache table rows from state S where state S
    // is a snapshot of the table at the start of this application.
    const std::string CONFIG_PATH = "psql_config.conf";
    DBConfig& dbc = DBConfig::getInstance();
    std::string conf;
    dbc.loadConfig(CONFIG_PATH);
    dbc.connObj.reset(new SQLConnection{dbc.HOST_NAME, dbc.DB_NAME, dbc.USER_NAME, dbc.PWD, static_cast<unsigned int>(std::stoi(dbc.PORT_NO))});
    dbc.connObj.get()->connect();
    pqxx::result r = dbc.connObj.get()->fetch("SELECT * FROM medicines");
    MedicineManager& ref = MedicineManager::getInstance();

    for(const pqxx::row& row : r)
    {

        Medicine m{row[0].as<unsigned int>(), row[1].as<std::string>(), row[2].as<std::string>(), row[3].as<std::string>(), row[4].as<std::string>(), row[5].as<double>(), row[6].as<int>()};
        ref.add(m);

    }

    dbc.connObj->disconnect();
    myMutex.unlock(); // Unlock mutex so that it is available for other threads.

}

// CreateSharedMemoryRegion() creates a volatile shared memory region if no parent process has created one since
// OS startup. This procedure is bounded by a mutex, if a child process invokes this method, the mutex is released
// and the function exits.
void createSharedMemoryRegion()
{

    // Initialize and acquire a mutex.
    boost::mutex myMutex;
    myMutex.lock();
    DBConfig& dbc = DBConfig::getInstance();

    // Attempt to create a shared memory region. If it throws, this
    // means another process has created the region and we just need
    // to open it.
    try
    {
        
        managed_shared_memory* shm = new managed_shared_memory(create_only, "boost", 65656); // Create shared memory region.
        const StringAllocator allocInstance(shm->get_segment_manager()); // Get allocator reference.

        // In the try block, we would be creating our shared memory region for the first time. So, we initialize our vectors to
        // an empty one.
        Vect* medicineVector = shm->construct<Vect>("propertyVector")(allocInstance);
        Vect* deleteVector = shm->construct<Vect>("deleteVector")(allocInstance);

        // Set managed_shared_memory reference in DBConfig.
        dbc.sharedMemoryObject.reset(shm);

    } 
    catch(const std::exception& e)
    {

        // If we reach this block, another process has already created the shared memory
        // region. There is no data to read at this point. So, just proceed and release
        // the mutex lock.
        managed_shared_memory* shm = new managed_shared_memory(open_only, "boost");
        dbc.sharedMemoryObject.reset(shm);

    }

    myMutex.unlock(); // Release mutex.

}

int main(int argc, char* argv[])
{

   
    boost::thread sharedMemoryThread{createSharedMemoryRegion};
    boost::thread workerThread{run}; // Declare load thread.
    sharedMemoryThread.join();
    workerThread.join(); // Block until it completes the callable.
    const size_t numRelations = MedicineManager::getInstance().getData().size();

    // If their are more than two relations in our vector, sort the ids
    // of these items.
    if(numRelations > 2)
    {

        MedicineManager::getInstance().heapSortById();

    // If their are only two relations, sometimes they may be unordered due to deserialization order from libpqxx.
    // If this is the case, we can swap them.
    }
    else if(numRelations == 2)
    {

        auto items = MedicineManager::getInstance().getData();

        if(items[0]._id > items[1]._id)
        {

            std::swap(items[0], items[1]);

        }
    }

     // Log main menu.
    Pages::getInstance().MAIN.log();

    // Await input.
    Pages::getInstance().MAIN.monitor();

    // Free memory allocated in DBConfig class.
    DBConfig::getInstance().connObj.reset();
    DBConfig::getInstance().sharedMemoryObject.reset();
    return 0;

}