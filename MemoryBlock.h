#ifndef MEMORYBLOCK_H
#define MEMORYBLOCK_H

#include "MemoryRecord.h"
#include <set>

class Object;

class MemoryBlock {
public:
    MemoryBlock();
    virtual ~MemoryBlock();

    unsigned char * allocate(unsigned int requestedSize);
    bool hasSpaceFor(unsigned int requestedSize) const;
    void sweep();
private:
    unsigned char * data;
    std::set<MemoryRecord> freeAddresses;
    const MemoryRecord & recordFor(unsigned int requestedSize);
    void free(Object * object);

    void print();
};

#endif /* MEMORYBLOCK_H */

