#pragma once
#include "../globals/Pages.cpp"
#include "headers/UpdateStockPage.h"
#include "../data/MedicineManager.cpp"
#include "../data/Medicine.cpp"
#include "../globals/Utils.cpp"
#include "../db/SQLConnection.cpp"
#include "../globals/DBConfig.cpp"
#include <boost/shared_ptr.hpp>
#include <string>
#include <iostream>

// Default constructor.
UpdateStockPage::UpdateStockPage()
{

    _questions.push_back(boost::make_tuple<std::string, unsigned int>("Enter Product Name (x not to change): ", _FLAG_ALL_TYPES));
    _questions.push_back(boost::make_tuple<std::string, unsigned int>("Enter Company Name (x not to change): ", _FLAG_ALL_TYPES));
    _questions.push_back(boost::make_tuple<std::string, unsigned int>("Enter Arrival Date (x not to change); DD-MM-YYYY: ", _FLAG_DATE_ONLY));
    _questions.push_back(boost::make_tuple<std::string, unsigned int>("Enter Expiration Date (x not to change); DD-MM-YYYY: ", _FLAG_DATE_ONLY));
    _questions.push_back(boost::make_tuple<std::string, unsigned int>("Enter Price (x not to change): ", _FLAG_DBL_ONLY));
    _questions.push_back(boost::make_tuple<std::string, unsigned int>("Enter Quantity (x not to change): ", _FLAG_INT_ONLY));

}

// Default destructor.
UpdateStockPage::~UpdateStockPage() {}


// Monitor() monitors() user input.
void UpdateStockPage::monitor()
{

    const size_t n = MedicineManager::getInstance().getData().size();
    std::string id;
    int convId;
    unsigned int maxId = n == 0 ? 1 : MedicineManager::getInstance().getData().at(n - 1)._id;
    bool emptyCache = n == 0;
    std::cin.ignore();

    if(!emptyCache)
    {

    while(true)
    {

        std::cout << std::endl << "Enter Product ID: ";
        std::getline(std::cin, id);
        bool isInt = Utils::getInstance().isStringInteger(id);

        if(!isInt)
        {

            std::cout << "Please enter a valid product ID." << std::endl;
            continue;

        }
        else
        {

            convId = std::stoi(id);

            if(convId >= 1 && convId <= maxId && MedicineManager::getInstance().getById(convId)._id > 0)
            {

                break;

            }
            else
            {

                std::cout << "ID specified is out of bounds." << std::endl;

            }
        }
    }

    // Print information about this product.
    const Medicine& lookup = MedicineManager::getInstance().getById(convId);
    std::cout << std::endl << "Product ID: " << lookup._id << std::endl;
    std::cout << "Product Name: " << lookup._name << std::endl;
    std::cout << "Owning Company: " << lookup._ownedBy << std::endl;
    std::cout << "Arrival Date: " << lookup._arrivalDate << std::endl;
    std::cout << "Expiration Date: " << lookup._expirationDate << std::endl;
    std::cout << "Price: " << lookup._price << std::endl;
    std::cout << "Quantity: " << lookup._qty << std::endl;

    std::string answer;
    unsigned int curQuestionIndex = 0;
    std::cout << std::endl;

    while(curQuestionIndex < _questions.size())
    {

        const boost::tuple<std::string, unsigned int> tup = _questions[curQuestionIndex];
        const std::string strQuestion = tup.get<0>();
        const unsigned int intFlag = tup.get<1>();
        std::cout << std::endl << strQuestion;
        std::getline(std::cin, answer);

        // If we get an "x", skip this question, do not update the field.
        if(answer == "x")
        {

            curQuestionIndex++;

        // Any type is accepted -> we don't need validation, take whatever it is
        // the user gives us as input and proceed to the next question.
        }
        else if(intFlag == _FLAG_ALL_TYPES)
        {

            if(answer.size() < _MIN_ALL_SIZE || answer.size() > _MAX_ALL_SIZE)
            {

                std::cout << std::endl << "Product Name and Company Name input must be between " << _MIN_ALL_SIZE << " and " << _MAX_ALL_SIZE << " characters." << std::endl;

            }
            else if(curQuestionIndex == 0 && !MedicineManager::getInstance().isUniqueName(answer))
            {

                std::cout << std::endl << "Invalid product name specified; please enter a unique name." << std::endl;

            }
            else
            {

                updateField(curQuestionIndex, id, answer);

            }

        // Only accept integers; validate that this is a integer, if so take it.
        // If not, print an error message and ask the question again.
        }
        else if(intFlag == _FLAG_INT_ONLY)
        {

            bool isValidInt = Utils::getInstance().isStringInteger(answer);

            if(isValidInt)
            {

                updateField(curQuestionIndex, id, answer);

            }
            else
            {

                std::cout << std::endl << "Invalid value provided; only integers are allowed." << std::endl;

            }

        // Only accept doubles.
        }
        else if(intFlag == _FLAG_DBL_ONLY)
        {

            bool isValidDbl = Utils::getInstance().isStringDouble(answer);

            if(isValidDbl)
            {

                updateField(curQuestionIndex, id, answer);

            }
            else
            {

                std::cout << std::endl << "Invalid value provided; only decimal numbers are allowed." << std::endl;

            }
        
        // Only accept date strings of the format DD-MM-YYYY.
        }
        else
        {

            bool isValidDate = Utils::getInstance().isStringDate(answer);

            if(isValidDate)
            {

                updateField(curQuestionIndex, id, answer);

            }
            else
            {

                std::cout << std::endl << "Invalid value provided; only dates of format DD-MM-YYYY are accepted." << std::endl;

            }
        }

        continue;

    }

    std::cout << std::endl << "Update successful." << std::endl;

    }

    if(emptyCache)
    {

        std::cout << std::endl << "Press 'm' to return to the main menu.." << std::endl;

    } 
    else
    {

        std::cout << std::endl << "Press 'm' to return to the main menu or 'u' to update a new item..." << std::endl;

    }

    std::string input;
    bool updateReq = false;
    
    while(true && !emptyCache)
    {
        

        std::cin >> input;

        if(input == "m")
        {

            break;

        }

        if(input == "u")
        {

            updateReq = true;
            break;

        }

        std::cout << "Invalid key pressed; you pressed " << input << std::endl;

    }

    while(true && emptyCache)
    {
        

        std::cin >> input;

        if(input == "m")
        {

            break;

        }

        std::cout << "Invalid key pressed; you pressed " << input << std::endl;

    }

    if(updateReq)
    {

        monitor();

    }
    else
    {

        system("clear");
        Pages::getInstance().MAIN.log();

    }
}

// Log() will log information about the update stock page.
void UpdateStockPage::log()
{

    std::cout << "Update Stock Center" << std::endl;
    MedicineManager& ref = MedicineManager::getInstance();
    boost::container::vector<Medicine> cache = ref.getData();
    int count = 0;
    
    for(const Medicine& m : cache)
    {

        bool printHeader = count == 0 ? true : false;
        m.print(Medicine::L_ALL, printHeader);
        count++;

    }

    bool emptyCache = cache.size() == 0;

    if(!emptyCache)
    {

        std::cout << "---------------------------------------------------------------------------------------------------------------------------------" << std::endl;

    }
    else
    {

        std::cout << std::endl << "There are no products currently in the database." << std::endl;

    }
}

// GetQueryableTuple(index) returns a two element boost::tuple that contains the queryable field 
// for the question located at the ith index of @see _questions and a constant representing the
// data type of the value to be assigned. This is wrapped within a boost::shared_ptr to avoid
// data loss during function frame destruction.
boost::shared_ptr<boost::tuple<std::string, unsigned int>> UpdateStockPage::getQueryableTuple(const unsigned int index)
{

    
    std::string strFieldName;
    unsigned int intDataType;

    if(index == 0)
    {

        strFieldName = "name";
        intDataType = _FLAG_ALL_TYPES;
        

    }
    else if(index == 1)
    {

        strFieldName = "owned_by";
        intDataType = _FLAG_ALL_TYPES;

    }
    else if(index == 2)
    {

        strFieldName = "arrival_date";
        intDataType = _FLAG_DATE_ONLY;

    }
    else if(index == 3)
    {

        strFieldName = "expire_date";
        intDataType = _FLAG_DATE_ONLY;

    }
    else if(index == 4)
    {

        strFieldName = "price";
        intDataType = _FLAG_DBL_ONLY;

    }
    else
    {

        strFieldName = "qty";
        intDataType = _FLAG_INT_ONLY;

    }

    boost::shared_ptr<boost::tuple<std::string, unsigned int>> res{new boost::tuple<std::string, unsigned int>(strFieldName, intDataType)};
    return res;

}


// UpdateField(index, strId, answer) updates the relation with id, strId, to a value
// of answer. Index is used to obtain the appropriate field name.
void UpdateStockPage::updateField(unsigned int& index, const std::string strId, const std::string answer)
{

    boost::shared_ptr<boost::tuple<std::string, unsigned int>> p = getQueryableTuple(index);
    const std::string strFieldName = p.get()->get<0>();
    const unsigned int intDataType = p.get()->get<1>();
    SQLConnection& conn = *DBConfig::getInstance().connObj.get();
    std::string q;

    // These are represented as strings in the database so we need to
    // escape using backslash.
    if(intDataType == _FLAG_ALL_TYPES || intDataType == _FLAG_DATE_ONLY)
    {


        // Sync the item in the MedicineManager class such that it is consistent with
        // the new change in the database; eliminates need to parse all relations
        // from database again.
        MedicineManager::getInstance().syncItemProperty(std::stoi(strId), strFieldName, answer);
        q = "UPDATE public.medicines SET " + strFieldName + "=\'" + answer + "\' WHERE id=" + strId + ";";

    // This case represents integers or doubles so assigning these values to the corresponding field
    // does not require escaping the string.
    }
    else
    {
        
        if(intDataType == _FLAG_DBL_ONLY)
        {

         
            // Sync the item in the MedicineManager class such that it is consistent with
            // the new change in the database; eliminates need to parse all relations
            // from database again.
            MedicineManager::getInstance().syncItemProperty(std::stoi(strId), strFieldName, std::stod(answer));

        }
        else
        {

            // Sync the item in the MedicineManager class such that it is consistent with
            // the new change in the database; eliminates need to parse all relations
            // from database again.
            MedicineManager::getInstance().syncItemProperty(std::stoi(strId), strFieldName, std::stoi(answer));

        }
        
        q = "UPDATE public.medicines SET " + strFieldName + "=" + answer + " WHERE id=" + strId + ";";

    }

    conn.connect();
    conn.insert(q);

    // Clean up; terminate connection and free memory from boost::shared_ptr.
    conn.disconnect();
    p.reset();
    index++;

}