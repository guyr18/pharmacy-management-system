#pragma once
#include "../globals/Pages.cpp"
#include "BuyMedicinePage.cpp"
#include "AddStockPage.cpp"
#include "FindItemPage.cpp"
#include "UpdateStockPage.cpp"
#include "DeleteStockPage.cpp"
#include "headers/MainPage.h"
#include<boost/scoped_ptr.hpp>
#include <string>
#include <iostream>

// Default constructor.
MainPage::MainPage() { }

// Default destructor.
MainPage::~MainPage() { }

// Monitor() monitors() user input.
void MainPage::monitor()
{

    boost::scoped_ptr<ITerminalPage> currentPage; // base pointer (boost::scoped_ptr) to be utilized for polymorphic page navigation.

    while(true)
    {

        std::string input;
        std::cin >> input;

        if(input == "1") // Buy Medicine.
        {
            
            system("clear");
            currentPage.reset(new BuyMedicinePage);
            currentPage->monitor();

        }
        else if(input == "2") // Show Item List.
        {

            system("clear");
            currentPage.reset(new ShowItemListPage);
            currentPage->monitor();
            

        }
        else if(input == "3") // Item database lookup.
        {

            system("clear");
            currentPage.reset(new FindItemPage);
            currentPage->log();
            currentPage->monitor();

        }
        else if(input == "4") // Add product to database.
        {

            system("clear");
            currentPage.reset(new AddStockPage);
            currentPage->log();
            currentPage->monitor();

        }
        else if(input == "5") // Update product attribute.
        {
            
            system("clear");
            currentPage.reset(new UpdateStockPage);
            currentPage->log();
            currentPage->monitor();            

        }
        else if(input == "6") // Delete product from database.
        {

            system("clear");
            currentPage.reset(new DeleteStockPage);
            currentPage->monitor();

           
        }
        else if(input == "7") // Exit application.
        {

            currentPage.reset();
            return;

        }
        else
        {

            continue;

        }
    }
}

// Log() will log the default commands (main menu) for this application.
void MainPage::log()
{

    std::cout << "1. Buy Medicine " << std::endl;
    std::cout << "2. Show Item List " << std::endl;
    std::cout << "3. Find Item From List " << std::endl;
    std::cout << "4. Add Item In Stock " << std::endl;
    std::cout << "5. Update Stock Item " << std::endl;
    std::cout << "6. Delete Stock Item " << std::endl;
    std::cout << "7. Exit " << std::endl;

}