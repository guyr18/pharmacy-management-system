#ifndef SHOW_ITEM_LIST_H
#define SHOW_ITEM_LIST_H

#include "../ITerminalPage.cpp"
#include "../ISharedMemory.cpp"

class ShowItemListPage : public ITerminalPage, public ISharedMemory
{

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