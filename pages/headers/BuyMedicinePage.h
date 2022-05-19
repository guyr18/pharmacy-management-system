#ifndef BUYMED_H
#define BUYMED_H

#include "../ITerminalPage.cpp"

class BuyMedicinePage : public ITerminalPage
{

    // CheckSharedMemory() is run by a separate thread of execution.
    // It checks the shared memory region for any data changes before
    // executing the remainder of the .monitor() method.
    void checkSharedMemory(); 

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