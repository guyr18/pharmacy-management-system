#ifndef SQLCONN_H
#define SQLCONN_H

#include <pqxx/pqxx>
#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>

class SQLConnection
{

    private:

        boost::shared_ptr<pqxx::connection> _conn; // Connection object managed through boost::shared_ptr
        std::string _hostname; // Server / host name.
        std::string _dbname; // Database name.
        std::string _username; // Username.
        std::string _password; // Password.
        unsigned int _port; // Port no.
        bool _active; // Is the connection active?
        pqxx::result DUMMY_RESULT; // Dummy result to act as a non-pointer type null;

        // Delete move constructor and move assignment.
        SQLConnection(const SQLConnection&& rhs) = delete;
        SQLConnection& operator=(const SQLConnection&& rhs);


    public:


        // Copy semantics.
        SQLConnection(const SQLConnection& rhs);
        SQLConnection& operator=(const SQLConnection& rhs);

        // Parameterless default constructor.
        SQLConnection();
        
        // Default constructor.
        SQLConnection(const std::string host, const std::string dbname, const std::string username, const std::string pwd, const unsigned int port_no);

        // Default destructor.
        ~SQLConnection();

        // Connect() initializes a pqxx::connection object and builds the relative
        // connection string.
        void connect();

        // Disconnect() terminates the connection established by @see _conn.
        void disconnect();

        // Fetch(q) executes a query q returns the result as pqxx::result object.
        pqxx::result fetch(const std::string q);

        // Insert(q) executes non-transaction, inserts rows relative to provide query
        // string q and commits the changes to the database.
        void insert(const std::string q);
    
        // IsActive() returns true if a valid connection is open (@see _conn) and
        // false otherwise.
        const bool isActive() const;

        // GetHostName() returns the hostname that is associated with
        // this SQLConnection instance.
        const std::string getHostName() const;

        // GetPortNo() returns the port no. that is associated with
        // this SQLConnection instance.
        const unsigned int getPortNo() const;

        // GetDbName() returns the name of the database that is associated
        // with this SQLConnection instance.
        const std::string getDbName() const;

        // GetPassword() returns the password that is associated with the
        // configuration of this SQLConnection instance.
        const std::string getPassword() const;

        // GetUserName() returns the username that is associated with the
        // configuration of this SQLConnection instance.
        const std::string getUserName() const;
        
};

#endif