#ifndef MEMORYBLOCK_H
#define MEMORYBLOCK_H

#include "MemoryRecord.h"
#include <vector>

class MemoryBlock {
public:
   MemoryBlock();
   virtual ~MemoryBlock();

   unsigned char * allocate(unsigned int requestedSize);
   bool hasSpaceFor(unsigned int requestedSize) const;
private:
   unsigned char * data;
   std::vector<MemoryRecord> freeAddresses;   
   MemoryRecord * recordFor(unsigned int requestedSize);
};

#endif /* MEMORYBLOCK_H */

