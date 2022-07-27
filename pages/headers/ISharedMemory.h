#ifndef SHAREDMEM_H
#define SHAREDMEM_H

class ISharedMemory
{
    
    private:

        // Delete move and copy semantics; this is an interface
        // and should be used as an abstract class.
        ISharedMemory(const ISharedMemory& rhs) = delete;
        ISharedMemory(const ISharedMemory&& rhs) = delete;
        ISharedMemory& operator=(const ISharedMemory&& rhs) = delete;
        ISharedMemory& operator=(const ISharedMemory& rhs) = delete;

    public:

        // Default constructor.
        ISharedMemory();

        // Default destructor.
        ~ISharedMemory();

        // CheckSharedMemory() checks the shared memory region and makes appropriate
        // changes.
        void checkSharedMemory();

};

#endif