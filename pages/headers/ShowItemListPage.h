#ifndef SHOW_ITEM_LIST_H
#define SHOW_ITEM_LIST_H

#include "../ITerminalPage.cpp"

class ShowItemListPage : public ITerminalPage
{

    // CheckSharedMemory() is run by a separate thread of execution.
    // It checks the shared memory region for any data changes before
    // executing the remainder of the .monitor() method.
    void checkSharedMemory();

    public:

        // Default destructor.
        ShowItemListPage();

        // Default constructor.
        ~ShowItemListPage();

        // Monitor() monitors() user input.
        void monitor();

        // Log() will log the current item list.
        void log();

};

#endif