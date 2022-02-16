#pragma once
#include "headers/Utils.h"
#include <iostream>
#include <string>
#include <locale>
#include <boost/tuple/tuple.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/scoped_ptr.hpp>

// Default constructor / destructor.
Utils::Utils() {}

Utils::~Utils() {}

Utils& Utils::getInstance()
{

    static Utils inst;
    return inst;

}

// PrintNSpaces(n) prints n spaces to the standard terminal.
void Utils::printNSpaces(const unsigned int n) noexcept
{

    for(int i = 0; i < n; i++)
    {

        std::cout << " ";

    }
}

// Abs(x) returns x if x >= 0 and -x if x < 0.
int Utils::abs(const int x) noexcept
{

    return x >= 0 ? x : -x;
    
}

// SplitBy(src, delim) splits a string src by delimiter delim, appends both portions
// into a two element boost::tuple and returns a corresponding reference.
boost::shared_ptr<boost::tuple<std::string, std::string>> Utils::splitBy(const std::string src, const char delim)
{

    unsigned int start = 0, end = 0;

    for(const char& ch : src)
    {

        if(ch != delim)
        {

            end++;
            
        }
        else
        {

            break;

        }
    }

    std::string left = src.substr(start, end);
    std::string right = src.substr(end + 1, src.size());
    boost::shared_ptr<boost::tuple<std::string, std::string>> res{new boost::tuple<std::string, std::string>(left, right)};
    return res;

}

// IsStringDouble(s) returns true if s represents a double. And otherwise, false.
bool Utils::isStringDouble(const std::string s)
{

    try
    {
        
        double temp = std::atof(s.c_str());
        return true;

    }
    catch(const std::exception& e)
    {
        
        return false;

    }
}

// IsStringInteger(s) returns true if s represents an integer. And otherwise, false.
bool Utils::isStringInteger(const std::string s)
{

    try
    {

        int temp = std::stoi(s);
        return true;

    }
    catch(const std::exception& e)
    {
        
        return false;

    }
}

// IsLeap(year) returns true if year is a leap year. And otherwise, false.
//
// This is a modified implementation of the following article: https://www.geeksforgeeks.org/program-check-date-valid-not/
bool Utils::isLeap(int year)
{

    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
    
}

// IsStringDate(s) return true if s represents a date of the following format: DD-MM-YYYY.
// And otherwise, false. DD-MM-YYYY
// 
// This is a modified implementation of the following article: https://www.geeksforgeeks.org/program-check-date-valid-not/
bool Utils::isStringDate(const std::string s)
{

    if(s.length() != 10)
    {

        return false;

    }

    // DD-MM-YYYY
    int d, m, y = -1;
    
    try
    {
        
        d = std::stoi(s.substr(0, 2));
        m = std::stoi(s.substr(3, 4));
        y = std::stoi(s.substr(6, 9));

    }
    catch(const std::exception& e)
    {

        return false;

    }
    
	if (y > 9999 || y < 1800)
    {

	    return false;

    }

	if (m < 1 || m > 12)
    {

	    return false;

    }

	if (d < 1 || d > 31)
    {

	    return false;

    }

	if (m == 2)
	{
		if (isLeap(y))
        {

		    return (d <= 29);

        }
		else
        {

		    return (d <= 28);

        }
	}

	if (m == 4 || m == 6 || m == 9 || m == 11)
    {
		return (d <= 30);

    }

	return true;

}