#include "Memory.h"

using namespace std;

Memory memory;

Memory::Memory() { }

Memory::~Memory() {
   for (MemoryBlock * block : blocks)
      delete block;
}

Object * Memory::allocate(unsigned int requestedSize) {
   unsigned char * address = this->getFreeAddress(requestedSize);
   if (address)
      return reinterpret_cast<Object*> (address);
   blocks.push_back(new MemoryBlock());
   return reinterpret_cast<Object*> (blocks.back()->allocate(requestedSize));
}

unsigned char * Memory::getFreeAddress(unsigned int requestedSize) {
   for (MemoryBlock * block : blocks)
      if (block->hasSpaceFor(requestedSize))
         return block->allocate(requestedSize);
   return nullptr;
}

void Memory::free(const Object * object) {
   ;
}