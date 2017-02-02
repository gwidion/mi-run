#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include "MemoryBlock.h"

class Object;

class Memory {
public:
   Memory();
   virtual ~Memory();

   Object * allocate(unsigned int requestedSize);
private:
   std::vector<MemoryBlock *> blocks;

   unsigned char * getFreeAddress(unsigned int requestedSize);
   void free(const Object * object);

};

#endif /* MEMORY_H */

