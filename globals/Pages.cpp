#pragma once
#include "../pages/headers/MainPage.h"
#include "../pages/headers/ShowItemListPage.h"
#include "../pages/headers/BuyMedicinePage.h"
#include "../pages/headers/AddStockPage.h"
#include "../pages/headers/FindItemPage.h"
#include "../pages/headers/UpdateStockPage.h"
#include "../pages/headers/DeleteStockPage.h"

class Pages
{

    private:

        // Default constructor for singleton initialization; private,
        // inaccessible to outsiders.
        Pages() {}

        // Suppress copy and move semantics.
        Pages(const Pages& rhs) = delete;
        Pages(const Pages&& rhs) = delete;
        Pages& operator=(const Pages& rhs) = delete;
        Pages& operator=(const Pages&& rhs) = delete;

    public:

        // GetInstance(); singleton method.
        static Pages& getInstance() {

            static Pages inst;
            return inst;

        }

        MainPage MAIN; // MainPage copy.
        
};