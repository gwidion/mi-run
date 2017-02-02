#include "Memory.h"

using namespace std;

Memory::Memory() { }

Memory::~Memory() { }

Object * Memory::allocate(unsigned int size) {
   unsigned char * address = this->getFreeAddress(size);
   if (address)
      return reinterpret_cast<Object*> (address);
   blocks.push_back(MemoryBlock());
   return reinterpret_cast<Object*> (blocks.back().allocate(size));
}

unsigned char * Memory::getFreeAddress(unsigned int size) {
   for (MemoryBlock & block : blocks)
      if (block.hasSpaceFor(size))
         return block.allocate(size);
   return nullptr;
}

void Memory::free(const Object * object) {
   ;
}