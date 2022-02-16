#ifndef UPDSTOCK_H
#define UPDSTOCK_H

#include "../ITerminalPage.cpp"
#include <boost/container/vector.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/shared_ptr.hpp>

class UpdateStockPage : public ITerminalPage
{

    private:

        const unsigned int _FLAG_ALL_TYPES = 0;
        const unsigned int _FLAG_INT_ONLY = 1;
        const unsigned int _FLAG_DBL_ONLY = 2;
        const unsigned int _FLAG_DATE_ONLY = 3;
        boost::container::vector<boost::tuple<std::string, unsigned int>> _questions;
       
        // GetQueryableTuple(index) returns a two element boost::tuple that contains the queryable field 
        // for the question located at the ith index of @see _questions and a constant representing the
        // data type of the value to be assigned. This is wrapped within a boost::shared_ptr to avoid
        // data loss during function frame destruction.
        boost::shared_ptr<boost::tuple<std::string, unsigned int>> getQueryableTuple(const unsigned int index);

        // UpdateField(index, strId, answer) updates the relation with id, strId, to a value
        // of answer. Index is used to obtain the appropriate field name.
        void updateField(unsigned int& index, const std::string strId, const std::string answer);


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