#ifndef MAINPAGE_H
#define MAINPAGE_H

#include "../ITerminalPage.cpp"

class MainPage : public ITerminalPage
{

    public:

        // Default destructor.
        MainPage();

        // Default constructor.
        ~MainPage();

        // Monitor() monitors() user input.
        void monitor();

        // Log() will log the default commands (main menu) for this application.
        void log();

};

#endif