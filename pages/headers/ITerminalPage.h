#ifndef TERMPAGE_H
#define TERMPAGE_H

class ITerminalPage
{
    
    private:

        // Delete move and copy semantics; this is an interface
        // and should be used as an abstract class.
        ITerminalPage(const ITerminalPage& rhs) = delete;
        ITerminalPage(const ITerminalPage&& rhs) = delete;
        ITerminalPage& operator=(const ITerminalPage&& rhs) = delete;
        ITerminalPage& operator=(const ITerminalPage& rhs) = delete;

    public:

        // Default constructor.
        ITerminalPage();

        // Default destructor.
        ~ITerminalPage();

        // A pure virtual function intended to monitor user input from a
        // sub-class of ITerminalPage.
        virtual void monitor() = 0;
        
        // A pure virtual function intended to log information specific to
        // a sub-class of ITerminalPage.
        virtual void log() = 0;

};

#endif