#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include "MemoryBlock.h"
#include "Stack.h"

class Object;

class Memory {
public:
    Memory();
    virtual ~Memory();

    Object * allocate(unsigned int requestedSize);

    void collectGarbage();

    Stack stack;
private:
    std::vector<MemoryBlock *> blocks;

    unsigned char * getFreeAddress(unsigned int requestedSize);

    void mark();
    void sweep();
};

#endif /* MEMORY_H */

