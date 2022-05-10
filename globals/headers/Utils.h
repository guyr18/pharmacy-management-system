#ifndef UTILS_H
#define UTILS_H

#include "../../db/SQLConnection.cpp"
#include <boost/tuple/tuple.hpp>
#include <boost/shared_ptr.hpp>
class Utils
{

    private:

        // Default constructor.
        Utils();

        // Hide copy and move semantics to enforce singleton.
        Utils(const Utils& rhs) = delete;
        Utils(const Utils&& rhs) = delete;
        Utils& operator=(const Utils& rhs) = delete;
        Utils& operator=(const Utils&& rhs) = delete;

    public:

        // Default destructor.
        ~Utils();

        static Utils& getInstance();

        // PrintNSpaces(n) prints n spaces to the standard terminal.
        static void printNSpaces(const unsigned int n) noexcept;

        // Abs(x) returns x if x >= 0 and -x if x < 0.
        static int abs(const int x) noexcept;

        // SplitBy(src, delim) splits a string src by delimiter delim, appends both portions
        // into a two element boost::tuple and returns a corresponding pointer reference.
        boost::shared_ptr<boost::tuple<std::string, std::string>> splitBy(const std::string src, const char delim);

        // IsStringDouble(s) returns true if s represents a double. And otherwise, false.
        bool isStringDouble(const std::string s);

        // IsStringInteger(s) returns true if s represents an integer. And otherwise, false.
        bool isStringInteger(const std::string s);   

        // IsLeap(year) returns true if year is a leap year. And otherwise, false.
        //
        // This is a modified implementation of the following article: https://www.geeksforgeeks.org/program-check-date-valid-not/
        bool isLeap(int year);
        
        // IsStringDate(s) return true if s represents a date of the following format: DD-MM-YYYY.
        // And otherwise, false.
        bool isStringDate(const std::string s);

        // GetCurrentDateAsString() returns the current date as a string
        // of the following format: DD-MM-YYYY.
        const std::string getCurrentDateAsString();

};

#endif