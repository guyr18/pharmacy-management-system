#ifndef FITEM_H
#define FITEM_H

#include "../ITerminalPage.cpp"

class FindItemPage : public ITerminalPage
{

    private:

        // IdSearchHandler() handles the algorithm related to id lookup.
        void idSearchHandler();

        // NameSearchHandler() handles the algorithm related to name lookup.
        void nameSearchHandler();

        // CheckSharedMemory() is run by a separate thread of execution.
        // It checks the shared memory region for any data changes before
        // executing the remainder of the .monitor() method.
        void checkSharedMemory(); 
        
    public:

        // Default destructor.
        FindItemPage();

        // Default constructor.
        ~FindItemPage();

        // Monitor() monitors() user input.
        void monitor();

        // Log() will log find item details.
        void log();

};

#endif