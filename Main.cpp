#include "db/SQLConnection.cpp"
#include "globals/Utils.cpp"
#include "pages/MainPage.cpp"
#include "pages/ShowItemListPage.cpp"
#include "globals/DBConfig.cpp"
#include <thread>
#include <mutex>

void run()
{

    std::mutex myMutex; // Declare mutex.
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

int main(int argc, char* argv[])
{

   
    std::thread workerThread{run}; // Declare initial load thread.
    workerThread.join(); // Block until it completes the callable.

     // Log main menu.
    Pages::getInstance().MAIN.log();

    // Await input.
    Pages::getInstance().MAIN.monitor();

    // Free memory allocated to SQLConnection object.
    DBConfig::getInstance().connObj.reset();
    return 0;

}