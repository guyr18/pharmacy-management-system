#pragma once
#include "../globals/Pages.cpp"
#include "BuyMedicinePage.cpp"
#include "../globals/DBConfig.cpp"
#include "headers/AddStockPage.h"
#include <boost/date_time.hpp>
#include <string>
#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

using namespace boost::interprocess;
typedef allocator<char, managed_shared_memory::segment_manager> CharAllocator;
typedef basic_string<char, std::char_traits<char>, CharAllocator> MyString;
typedef allocator<MyString, managed_shared_memory::segment_manager> StringAllocator;
typedef boost::interprocess::vector<MyString, StringAllocator> Vect;

// Default constructor.
AddStockPage::AddStockPage()
{

    _questions.push_back(boost::make_tuple<std::string, unsigned int>("Enter Product Name: ", _FLAG_ALL_TYPES));
    _questions.push_back(boost::make_tuple<std::string, unsigned int>("Enter Company Name: ", _FLAG_ALL_TYPES));
    _questions.push_back(boost::make_tuple<std::string, unsigned int>("Enter Arrival Date: ", _FLAG_DATE_ONLY));
    _questions.push_back(boost::make_tuple<std::string, unsigned int>("Enter Expiration Date: ", _FLAG_DATE_ONLY));
    _questions.push_back(boost::make_tuple<std::string, unsigned int>("Enter Price: ", _FLAG_DBL_ONLY));
    _questions.push_back(boost::make_tuple<std::string, unsigned int>("Enter Quantity: ", _FLAG_INT_ONLY));

}

// Default destructor.
AddStockPage::~AddStockPage() { }

// HandleAdd() is run by a separate thread of execution. It blocks until
// it is able to obtain the mutex lock. Then, it will perform back-end
// operations relative to adding @param newMedicineObject to the database.
void AddStockPage::handleAdd(Medicine& newMedicineObject)
{

    // Initialize and acquire lock.
    boost::mutex myMutex;
    myMutex.lock();
    const size_t n = MedicineManager::getInstance().getData().size();
    newMedicineObject._id = n == 0 ? 1 : MedicineManager::getInstance().getData().at(n - 1)._id + 1;

    std::string q1 = "INSERT INTO public.medicines(id, name, \"owned_by\", \"arrival_date\", \"expire_date\", price, qty) VALUES (";
    std::string q2 = std::to_string(newMedicineObject._id) + ", \'" + newMedicineObject._name + "\', \'" + newMedicineObject._ownedBy + "\', \'" + newMedicineObject._arrivalDate + "\', \'" + newMedicineObject._expirationDate
                     + "\', " + std::to_string(newMedicineObject._price) + ", " + std::to_string(newMedicineObject._qty) + ");";
    std::string q3 = q1 + q2;

    SQLConnection& conn = *DBConfig::getInstance().connObj.get();
    conn.connect();
    conn.insert(q3);
    conn.disconnect();
    MedicineManager::getInstance().add(newMedicineObject);

    // We need to write this new item to shared memory so that other processes will know
    // that a change was made.
    managed_shared_memory* shm = DBConfig::getInstance().sharedMemoryObject.get();
    Vect* m = shm->find<Vect>("propertyVector").first;

    // Check that it exists in memory.
    if(m != nullptr)
    {

        const CharAllocator charAllocInstance(shm->get_segment_manager());
        const std::string strId = std::to_string(newMedicineObject._id);
        const std::string strPrice = std::to_string(newMedicineObject._price);
        const std::string strQty = std::to_string(newMedicineObject._qty);
        MyString myStringId(strId.begin(), strId.end(), charAllocInstance);
        MyString myStringName(newMedicineObject._name.begin(), newMedicineObject._name.end(), charAllocInstance);
        MyString myStringOwnedBy(newMedicineObject._ownedBy.begin(), newMedicineObject._ownedBy.end(), charAllocInstance);
        MyString myStringAD(newMedicineObject._arrivalDate.begin(), newMedicineObject._arrivalDate.end(), charAllocInstance);
        MyString myStringED(newMedicineObject._expirationDate.begin(), newMedicineObject._expirationDate.end(), charAllocInstance);
        MyString myStringPrice(strPrice.begin(), strPrice.end(), charAllocInstance);
        MyString myStringQty(strQty.begin(), strQty.end(), charAllocInstance);

        // After we have a valid vector, we can add the properties of the Medicine object
        // so other processes may access them.
        m->push_back(myStringId);
        m->push_back(myStringName);
        m->push_back(myStringOwnedBy);
        m->push_back(myStringAD);
        m->push_back(myStringED);
        m->push_back(myStringPrice);
        m->push_back(myStringQty);
        
    }

    if((n + 1) > 2)
    {

        MedicineManager::getInstance().heapSortById();

    }
    
    myMutex.unlock();

}

// Monitor() monitors() user input.
void AddStockPage::monitor()
{

    std::string answer;
    unsigned int curQuestionIndex = 0;
    Medicine newMedicineObject;
    std::cin.ignore();

    while(curQuestionIndex < _questions.size())
    {


        const boost::tuple<std::string, unsigned int> tup = _questions[curQuestionIndex];
        const std::string strQuestion = tup.get<0>();
        const unsigned int intFlag = tup.get<1>();
        std::cout << std::endl << strQuestion;
        std::getline(std::cin, answer);

        if(intFlag == _FLAG_ALL_TYPES)
        {

            if(answer.size() < _MIN_ALL_SIZE || answer.size() > _MAX_ALL_SIZE)
            {

                std::cout << std::endl << "Product Name and Company Name input must be between " << _MIN_ALL_SIZE << " and " << _MAX_ALL_SIZE << " characters." << std::endl;

            }
            else
            {

                if(!MedicineManager::getInstance().isUniqueName(answer))
                {

                    std::cout << std::endl << "Invalid product name specified; please enter a unique name." << std::endl;

                }
                else if(curQuestionIndex == 0)
                {

                    newMedicineObject._name = answer;
                    curQuestionIndex++;

                }
                else
                {

                    newMedicineObject._ownedBy = answer;
                    curQuestionIndex++;

                }   
            }
        }
        else if(intFlag == _FLAG_DATE_ONLY)
        {

            bool isDateValid = Utils::getInstance().isStringDate(answer);

            // Use current date
            if(answer.size() == 0)
            {

                std::string strCurDate = Utils::getInstance().getCurrentDateAsString();

                if(curQuestionIndex == 2)
                {

                    newMedicineObject._arrivalDate = strCurDate;

                }
                else
                {

                    newMedicineObject._expirationDate = strCurDate;

                }

                std::cout << std::endl << "No input specified; defaulting to current date." << std::endl;
                curQuestionIndex++;

            }
            else if(isDateValid)
            {

                if(curQuestionIndex == 2)
                {

                    newMedicineObject._arrivalDate = answer;

                }
                else
                {

                    newMedicineObject._expirationDate = answer;

                }

                curQuestionIndex++;

            }
            else
            {

                std::cout << std::endl << "Invalid date specified; please follow the following format: DD-MM-YYYY." << std::endl;

            }

        }
        else if(intFlag == _FLAG_DBL_ONLY)
        {

            bool isDblValid = Utils::getInstance().isStringDouble(answer);

            if(isDblValid)
            {

                newMedicineObject._price = std::stod(answer);
                curQuestionIndex++;

            }
            else
            {

                std::cout << std::endl << "Invalid price specified; please enter a valid floating point number." << std::endl;

            }

        }
        else
        {

            try
            {
                

                bool isIntValid = Utils::getInstance().isStringInteger(answer);

                // If the string is validated as an integer, then cast it and assign it
                // to the Medicine object.
                if(isIntValid)
                {

                    newMedicineObject._qty = std::stoi(answer);
                    curQuestionIndex++;

                // If we reach this block, neither an integer or double was provided, report the error.
                }
                else
                {

                    std::cout << std::endl << "Invalid integer specified; please enter a valid integer value." << std::endl;

                }
            } 
            catch(const std::exception& e)
            {

                // If an exception occurs, this means that the input provided
                // was a double. We should use a different standard library
                // function to cast a double; std::stod.
                newMedicineObject._qty = std::stod(answer);
                curQuestionIndex++;

            }
        }
    }

    boost::thread sharedMemoryThread(&AddStockPage::checkSharedMemory, this);
    boost::thread workerThread(&AddStockPage::handleAdd, this, std::ref(newMedicineObject));
    sharedMemoryThread.join();
    workerThread.join();
    system("clear");
    Pages::getInstance().MAIN.log();
    return;

}

// Log() will log medicine transaction details.
void AddStockPage::log()
{

    std::cout << "Add Product To Stock" << std::endl << std::endl;

}