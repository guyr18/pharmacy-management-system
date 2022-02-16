#ifndef DELSTK_H
#define DELSTK_H

#include "../ITerminalPage.cpp"

class DeleteStockPage : public ITerminalPage
{

    public:

        // Default destructor.
        DeleteStockPage();

        // Default constructor.
        ~DeleteStockPage();

        // Monitor() monitors() user input.
        void monitor();

        // Log() will log medicine deletion details.
        void log();

};

#endif