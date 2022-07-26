#pragma once
#include "headers/SQLConnection.h"

SQLConnection::SQLConnection() {}

// Default constructor.
SQLConnection::SQLConnection(const std::string host, const std::string dbname, const std::string username, const std::string pwd, const unsigned int port_no)
{

    _hostname = host;
    _dbname = dbname;
    _username = username;
    _password = pwd;
    _port = port_no;

}

// Copy constructor.
SQLConnection::SQLConnection(const SQLConnection& rhs)
{

    _hostname = rhs.getHostName();
    _dbname = rhs.getDbName();
    _username = rhs.getUserName();
    _password = rhs.getPassword();
    _port = rhs.getPortNo();

}

// Copy assignment.
SQLConnection& SQLConnection::operator=(const SQLConnection& rhs)
{

    if(&rhs != this)
    {
        
        _hostname = rhs.getHostName();
        _dbname = rhs.getDbName();
        _username = rhs.getUserName();
        _password = rhs.getPassword();
        _port = rhs.getPortNo();
        return *this;
        
    }
}

// Default destructor.
SQLConnection::~SQLConnection()
{

    if(_conn)
    {

        _conn.reset();
        _active = false;

    }
}

// Connect() initializes a pqxx::connection object and builds the relative
// connection string.
void SQLConnection::connect()
{

    try
    {
   
        if(!_conn)
        {

            _conn.reset(new pqxx::connection{
                "user=" + _username + " "
                "host=" + _hostname + " "
                "port=" + std::to_string(_port) + " "
                "password=" + _password + " "
                "dbname=" + _dbname});

        }

        _active = true;

    }
    catch(const std::exception& e)
    {

        std::cerr << e.what() << std::endl;

    }
}

// Disconnect() terminates the connection established by @see _conn.
void SQLConnection::disconnect()
{

    _conn.get()->disconnect();

}

// Fetch(q) executes a query q returns the result as pqxx::result object.
pqxx::result SQLConnection::fetch(const std::string q)
{

    try
    {

        pqxx::work w{*_conn.get()};
        pqxx::result r = w.exec(q);
        w.commit();
        return r;

    }
    catch(const std::exception& e)
    {

        std::cerr << e.what() << std::endl;

    }

    return DUMMY_RESULT;

}

// Insert(q) executes non-transaction, inserts rows relative to provide query
// string q and commits the changes to the database.
void SQLConnection::insert(const std::string q)
{

    try
    {
        
        pqxx::work w{*_conn.get()};
        w.exec(q);
        w.commit();

    }
    catch(const std::exception& e)
    {

        std::cerr << e.what() << std::endl;

    }
}


// IsActive() returns true if a valid connection is open (@see _conn) and
// false otherwise.
bool SQLConnection::isActive() const
{

    return _active;

}

// GetHostName() returns the hostname that is associated with
// this SQLConnection instance.
std::string SQLConnection::getHostName() const 
{

    return _hostname;

}

// GetPortNo() returns the port no. that is associated with
// this SQLConnection instance.
unsigned int SQLConnection::getPortNo() const
{

    return _port;

}

// GetDbName() returns the name of the database that is associated
// with this SQLConnection instance.
std::string SQLConnection::getDbName() const
{

    return _dbname;

}

// GetPassword() returns the password that is associated with the
// configuration of this SQLConnection instance.
std::string SQLConnection::getPassword() const
{

    return _password;

}

// GetUserName() returns the username that is associated with the
// configuration of this SQLConnection instance.
std::string SQLConnection::getUserName() const
{

    return _username;

}
