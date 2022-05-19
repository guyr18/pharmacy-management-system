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
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

typedef allocator<char, managed_shared_memory::segment_manager> CharAllocator;
typedef basic_string<char, std::char_traits<char>, CharAllocator> MyString;
typedef allocator<MyString, managed_shared_memory::segment_manager> StringAllocator;
typedef boost::interprocess::vector<MyString, StringAllocator> Vect;

using namespace boost::interprocess;

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

// CheckSharedMemory() is run by a separate thread of execution.
// It checks the shared memory region for any data changes before
// executing the remainder of the .monitor() method.
void UpdateStockPage::checkSharedMemory()
{

    boost::mutex myMutex;
    myMutex.lock();

    try {

        // Reference shared memory.
        managed_shared_memory* shm = DBConfig::getInstance().sharedMemoryObject.get();

        // Obtain the thread-safe vector that is stored in shared memory.
        Vect* m = shm->find<Vect>("propertyVector").first;

        // Are their new properties in this vector?
        if(m != nullptr && m->size() >= 7 && m->size() % 7 == 0)
        {

            int count = 0;
            unsigned int id = 0;
            std::string name = "";
            std::string ownedBy = "";
            std::string arrivalDate = "";
            std::string expirationDate = "";
            double price = 0.0;
            int qty = 0;

            // If the vector exists and there are multiple Medicine objects, update in-memory
            // data structure; i.e.: MedicineManager.
            for(auto itr = m->begin(); itr != m->end(); itr++, count++)
            {

                // This is the 7th item. Medicine objects store 7 variables, therefore we have enough
                // information now to potentially add this to our in-memory data structure.
                if((count + 1) >= 7 && (count + 1) % 7 == 0)
                {


                     Medicine& target = MedicineManager::getInstance().getById(id);
                     qty = std::stoi(std::string(itr->begin(), itr->end()));

                    // If we repeatedly go through this process, it is possible that the item
                    // is already consumed and is in the in-memory data structure for this process.
                    // We don't want duplicates, so we want to verify that this Medicine object
                    // is unique before adding it.
                    if(target._id == 0)
                    {

                        // Build Medicine object from properties and add to MedicineManager vector.
                        const Medicine objMedicine{id, name, ownedBy, arrivalDate, expirationDate, price, qty};
                        MedicineManager::getInstance().add(objMedicine);

                        // Reset local variable properties.
                        id = 0;
                        name = "";
                        ownedBy = "";
                        arrivalDate = "";
                        expirationDate = "";
                        price = 0.0;
                        qty = 0;

                    // It is possible that it is not neccessarily unique, but some properties may have been updated. We will need to reflect
                    // this in the MedicineManager class.
                    }
                    else
                    {

                        if(target._name != name)
                        {

                            target._name = name;

                        }

                        if(target._ownedBy != ownedBy)
                        {

                            target._ownedBy = ownedBy;

                        }

                        if(target._arrivalDate != arrivalDate)

                        {

                            target._arrivalDate = arrivalDate;

                        }

                        if(target._expirationDate != expirationDate)
                        {

                            target._expirationDate = expirationDate;

                        }

                        if(target._price != price)
                        {

                            target._price = price;

                        }

                        if(target._qty != qty)
                        {

                            target._qty = qty;
                            
                        }
                    }

                    count = -1; // Set count to -1, because we know it will be incremented to zero at the end of this iteration.
                    itr++;
                    continue;

                }

                 // We need to figure out which property this is. We can logically deduce this from the value of count.
                if(count == 0)
                {

                    id = std::stoi(std::string(itr->begin(), itr->end()));

                }
                else if(count == 1)
                {

                    name = std::string(itr->begin(), itr->end());

                }
                else if(count == 2)
                {

                    ownedBy = std::string(itr->begin(), itr->end());

                }
                else if(count == 3)
                {

                    arrivalDate = std::string(itr->begin(), itr->end());

                }
                else if(count == 4)
                {

                    expirationDate = std::string(itr->begin(), itr->end());

                }
                else if(count == 5)
                {

                    price = std::stod(std::string(itr->begin(), itr->end()));

                }
            }
        }

    // Check if any IDs are present in MedicineManager and deleteVector. If so, we
    // should remove them.
    Vect* deleteVector = shm->find<Vect>("deleteVector").first;

    for(auto dItr = deleteVector->begin(); dItr != deleteVector->end(); ++dItr)
    {

        const unsigned int intId = std::stoi(std::string(dItr->begin(), dItr->end()));
        Medicine& targetMed = MedicineManager::getInstance().getById(intId);
        
        if(targetMed._id != 0)
        {

            MedicineManager::getInstance().removeById(targetMed._id);

        }
    }

    }
    catch(const std::exception& e) 
    {}

    myMutex.unlock();

}

// Monitor() monitors() user input.
void UpdateStockPage::monitor()
{

    const size_t n = MedicineManager::getInstance().getData().size();
    std::string id;
    int convId;
    unsigned int maxId = n == 0 ? 1 : MedicineManager::getInstance().getData().at(n - 1)._id;
    bool emptyCache = n == 0;

    // Check for shared memory on separate thread one last time
    // before logging data.
    boost::thread memoryThread{&UpdateStockPage::checkSharedMemory, this};
    memoryThread.join(); // Block for thread completion.
    log(); // Log data.
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

                boost::thread memoryThread(&UpdateStockPage::checkSharedMemory, this);
                boost::thread workerThread(&UpdateStockPage::updateField, this, std::ref(curQuestionIndex), id, answer);
                memoryThread.join();
                workerThread.join();

            }

        // Only accept integers; validate that this is a integer, if so take it.
        // If not, print an error message and ask the question again.
        }
        else if(intFlag == _FLAG_INT_ONLY)
        {

            bool isValidInt = Utils::getInstance().isStringInteger(answer);

            if(isValidInt)
            {

                boost::thread memoryThread(&UpdateStockPage::checkSharedMemory, this);
                boost::thread workerThread(&UpdateStockPage::updateField, this, std::ref(curQuestionIndex), id, answer);
                memoryThread.join();
                workerThread.join();

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

                boost::thread memoryThread(&UpdateStockPage::checkSharedMemory, this);
                boost::thread workerThread(&UpdateStockPage::updateField, this, std::ref(curQuestionIndex), id, answer);
                memoryThread.join();
                workerThread.join();

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
                
                boost::thread memoryThread(&UpdateStockPage::checkSharedMemory, this);
                boost::thread workerThread(&UpdateStockPage::updateField, this, std::ref(curQuestionIndex), id, answer);
                memoryThread.join();
                workerThread.join();

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
    boost::mutex myMutex;
    myMutex.lock();

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

    // Reference shared memory.
    managed_shared_memory* shm = DBConfig::getInstance().sharedMemoryObject.get();

    // Register changes in shared memory segment.
    Vect* m = shm->find<Vect>("propertyVector").first;
    const CharAllocator charAllocInst(shm->get_segment_manager());
    int skipCount = 1;
    auto startIterator = m->end();

    for(auto itr = m->begin(); itr != m->end(); itr += 7, skipCount++)
    {

        std::string ref(itr->begin(), itr->end());

        if(ref == strId)
        {

            auto temp = itr;
            startIterator = m->begin() + (skipCount * 7);
            break;

        }
    }

    // This ID is not in the shared memory segment, so we will add it to the vector.
    if(startIterator == m->end())
    {

        Medicine& target = MedicineManager::getInstance().getById(std::stoi(strId));
        const std::string strId2 = std::to_string(target._id);
        const std::string strPrice = std::to_string(target._price);
        const std::string strQty = std::to_string(target._qty);
        MyString myStringId(strId2.begin(), strId2.end(), charAllocInst);
        MyString myStringName(target._name.begin(), target._name.end(), charAllocInst);
        MyString myStringOwnedBy(target._ownedBy.begin(), target._ownedBy.end(), charAllocInst);
        MyString myStringAD(target._arrivalDate.begin(), target._arrivalDate.end(), charAllocInst);
        MyString myStringED(target._expirationDate.begin(), target._expirationDate.end(), charAllocInst);
        MyString myStringPrice(strPrice.begin(), strPrice.end(), charAllocInst);
        MyString myStringQty(strQty.begin(), strQty.end(), charAllocInst);
        m->push_back(myStringId);
        m->push_back(myStringName);
        m->push_back(myStringOwnedBy);
        m->push_back(myStringAD);
        m->push_back(myStringED);
        m->push_back(myStringPrice);
        m->push_back(myStringQty);

    }
    else
    {

        // The ID is in the shared memory segment. So, we should see if any of the properties differ
        // from what is currently in MedicineManager. We need everyone to be in sync.
        Medicine& target = MedicineManager::getInstance().getById(std::stoi(strId));
        int count = 1;
        startIterator += 1;

        while(startIterator != m->end())
        {
            
            std::string ref = std::string(startIterator->begin(), startIterator->end());

            if(count == 1 && target._name != ref)
            {

                *startIterator = MyString(target._name.begin(), target._name.end(), charAllocInst);

            }
            else if(count == 2 && target._ownedBy != ref)
            {

                *startIterator = MyString(target._ownedBy.begin(), target._ownedBy.end(), charAllocInst);

            }
            else if(count == 3 && target._arrivalDate != ref)
            {

                *startIterator = MyString(target._arrivalDate.begin(), target._arrivalDate.end(), charAllocInst);

            }
            else if(count == 4 && target._expirationDate != ref)
            {

                *startIterator = MyString(target._expirationDate.begin(), target._expirationDate.end(), charAllocInst);

            }
            else if(count == 5 && target._price != std::stoi(ref))
            {

                std::string temp = std::to_string(target._price);
                *startIterator = MyString(temp.begin(), temp.end(), charAllocInst);

            }
            else
            {
                
                std::string temp = std::to_string(target._qty);
                *startIterator = MyString(temp.begin(), temp.end(), charAllocInst);

            }

            startIterator++;
            count++;

        }   
    }

    myMutex.unlock();

}