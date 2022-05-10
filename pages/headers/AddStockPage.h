#ifndef ADDSTK_H
#define ADDSTK_H

#include "../ITerminalPage.cpp"
#include <boost/container/vector.hpp>
#include <boost/tuple/tuple.hpp>

class AddStockPage : public ITerminalPage
{

    private:


        const unsigned int _FLAG_ALL_TYPES = 0;
        const unsigned int _FLAG_INT_ONLY = 1;
        const unsigned int _FLAG_DBL_ONLY = 2;
        const unsigned int _FLAG_DATE_ONLY = 3;
        const unsigned int _MIN_ALL_SIZE = 3;
        const unsigned int _MAX_ALL_SIZE = 16;
        boost::container::vector<boost::tuple<std::string, unsigned int>> _questions;   

    public:

        // Default destructor.
        AddStockPage();

        // Default constructor.
        ~AddStockPage();

        // Monitor() monitors() user input.
        void monitor();

        // Log() will log medicine transaction details.
        void log();

};

#endif