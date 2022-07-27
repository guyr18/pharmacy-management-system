#ifndef BUYMED_H
#define BUYMED_H

#include "../ITerminalPage.cpp"
#include "../ISharedMemory.cpp"

class BuyMedicinePage : public ITerminalPage, public ISharedMemory
{

    private:

    // HandleBuy(newQty) takes a vector of tuples (i, q) where i is the
    // ID of the item being update and q is the new quantity. It is thread-safe.
    void handleBuy(boost::container::vector<boost::tuple<int, int>>& newQty);

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