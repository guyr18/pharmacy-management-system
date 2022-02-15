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
        static void printNSpaces(const unsigned int n);

        // Abs(x) returns x if x >= 0 and -x if x < 0.
        static int abs(const int x);

        // SplitBy(src, delim) splits a string src by delimiter delim, appends both portions
        // into a two element boost::tuple and returns a corresponding pointer reference.
        boost::shared_ptr<boost::tuple<std::string, std::string>> splitBy(const std::string src, const char delim);

};

#endif