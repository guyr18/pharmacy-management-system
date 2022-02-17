#ifndef DELSTK_H
#define DELSTK_H

#include "../ITerminalPage.cpp"

class DeleteStockPage : public ITerminalPage
{

    private:

        // HandleDeleteRelation(id) deletes a relation from the appropriate database table
        // given the primary key id.
        void handleDeleteRelation(const unsigned int& id);

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