#ifndef ADDSTK_H
#define ADDSTK_H

#include "../ITerminalPage.cpp"

class AddStockPage : public ITerminalPage
{

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