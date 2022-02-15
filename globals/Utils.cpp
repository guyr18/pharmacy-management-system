#pragma once
#include "headers/Utils.h"
#include <iostream>
#include <string>
#include <boost/tuple/tuple.hpp>

// Default constructor / destructor.
Utils::Utils() {}

Utils::~Utils() {}

Utils& Utils::getInstance()
{

    static Utils inst;
    return inst;

}

// PrintNSpaces(n) prints n spaces to the standard terminal.
void Utils::printNSpaces(const unsigned int n)
{

    for(int i = 0; i < n; i++)
    {

        std::cout << " ";

    }
}

// Abs(x) returns x if x >= 0 and -x if x < 0.
int Utils::abs(const int x)
{

    return x >= 0 ? x : -x;
    
}

// SplitBy(src, delim) splits a string src by delimiter delim, appends both portions
// into a two element boost::tuple and returns a corresponding reference.
boost::tuple<std::string, std::string>* Utils::splitBy(const std::string src, const char delim)
{

    unsigned int start = 0, end = 0;

    for(const char& ch : src)
    {

        if(ch != delim)
        {

            end++;

        }

        break;

    }

    std::string left = src.substr(start, end);
    std::string right = src.substr(end + 1, src.size());
    boost::tuple<std::string, std::string>* res = new boost::tuple<std::string, std::string>(left, right);
    return res;

}