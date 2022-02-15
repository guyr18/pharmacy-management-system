#pragma once
#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include "../db/SQLConnection.cpp"
#include "Utils.cpp"
#include <fstream>

class DBConfig
{

    private:

        // Default constructor; marked as private.
        DBConfig() {}

        // Suppress constructors to enforce singleton.
        DBConfig(const DBConfig& rhs) = delete;
        DBConfig(const DBConfig&& rhs) = delete;
        DBConfig& operator=(const DBConfig& rhs) = delete;
        DBConfig& operator=(const DBConfig&& rhs) = delete;


    public:

        // Configuration variables.
        std::string HOST_NAME, DB_NAME, USER_NAME, PWD, PORT_NO;

        // SQLConnection copy.
        boost::shared_ptr<SQLConnection> connObj;

        // Default destructor.
        ~DBConfig() {}

        static DBConfig& getInstance()
        {

            static DBConfig inst;
            return inst;

        }

        // LoadConfig(path) loads the configuration file found at path, splits each line by
        // its delimiter (=) and assigns the corresponding fields within this class; DBConfig.
        //
        // If an invalid path is specified, an exception and corresponding message is logged
        // to the standard error stream.
        void loadConfig(const std::string path)
        {

            try
            {
                
                std::ifstream ifs{path};
                std::string line;

                while(std::getline(ifs, line))
                {

                    const boost::tuple<std::string, std::string> res = Utils::getInstance().splitBy(line, '=');
                    const std::string strVar = res.get<0>();
                    const std::string strVal = res.get<1>();

                    if(strVar == "HOST_NAME")
                    {

                        HOST_NAME = strVal;

                    }
                    else if(strVar == "USER_NAME")
                    {

                        USER_NAME = strVal;

                    }
                    else if(strVar == "PWD")
                    {

                        PWD = strVal;

                    }
                    else if(strVar == "DB_NAME")
                    {

                        DB_NAME = strVal;

                    }
                    else if(strVar == "PORT_NO")
                    {

                        PORT_NO = strVal;

                    }
                    else
                    {

                        std::cerr << "Invalid configuration file! All field names must maintain the same naming as when downloaded." << std::endl;

                    }
                }
            }
            catch(const std::exception& e)
            {

                std::cerr << "Couldn\'t load configuration file!" << std::endl;

            }
        }

};