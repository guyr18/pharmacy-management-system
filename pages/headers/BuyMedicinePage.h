#ifndef BUYMED_H
#define BUYMED_H

#include "../ITerminalPage.cpp"

class BuyMedicinePage : public ITerminalPage
{

    public:

        // Default destructor.
        BuyMedicinePage();

        // Default constructor.
        ~BuyMedicinePage();

        // Monitor() monitors() user input.
        void monitor();

        // Log() will log medicine transaction details.
        void log();

};

#endif