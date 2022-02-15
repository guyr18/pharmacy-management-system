#ifndef UPDSTOCK_H
#define UPDSTOCK_H

#include "../ITerminalPage.cpp"

class UpdateStockPage : public ITerminalPage
{

    public:

        // Default destructor.
        UpdateStockPage();

        // Default constructor.
        ~UpdateStockPage();

        // Monitor() monitors() user input.
        void monitor();

        // Log() will log information about the update stock page.
        void log();

};

#endif