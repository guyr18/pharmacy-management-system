#ifndef IPRINTABLE_H
#define IPRINTABLE_H

class IPrintable
{

    public:

        IPrintable() {}
        ~IPrintable() {}
    protected:

        //IPrintable(const IPrintable& rhs) = delete;
        //IPrintable(const IPrintable&& rhs) = delete;
        //IPrintable& operator=(const IPrintable& rhs) = delete;
        //IPrintable& operator=(const IPrintable&& rhs) = delete;

        // Print(level) is a virtual function that prints information
        // relative to the deriving class. Level is provided as a specifier
        // to describe what subset of a particular set of information 
        // should be printed.
        virtual void print(const unsigned int level = 0, const bool printHeader = true) {}
    
};

#endif