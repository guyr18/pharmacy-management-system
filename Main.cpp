#include "db/SQLConnection.cpp"
#include "globals/Utils.cpp"
#include "pages/MainPage.cpp"
#include "pages/ShowItemListPage.cpp"
#include "globals/DBConfig.cpp"

int main(int argc, char* argv[])
{

    // Establish connection and cache table rows from state S where state S
    // is a snapshot of the table at the start of this application.
    const std::string CONFIG_PATH = "psql_config.conf";
    DBConfig& dbc = DBConfig::getInstance();
    std::string conf;
    dbc.loadConfig(CONFIG_PATH);
    SQLConnection conn{dbc.HOST_NAME, dbc.DB_NAME, dbc.USER_NAME, dbc.PWD, std::stoi(dbc.PORT_NO)};
    conn.connect();
    pqxx::result r = conn.fetch("SELECT * FROM medicines");
    MedicineManager& ref = MedicineManager::getInstance();

    for(const pqxx::row& row : r)
    {

        Medicine m{row[0].as<unsigned int>(), row[1].as<std::string>(), row[2].as<std::string>(), row[3].as<std::string>(), row[4].as<std::string>(), row[5].as<double>(), row[6].as<int>()};
        ref.add(m);

    }

    conn.disconnect();
    *dbc.connObj.get() = conn;

    // Log main menu.
    Pages::getInstance().MAIN.log();

    // Await input.
    Pages::getInstance().MAIN.monitor();
    return 0;

}