#include "Memory.h"
#include "Stack.h"
#include <iostream>

using namespace std;

Memory memory;

Memory::Memory() { }

Memory::~Memory() {
   for (MemoryBlock * block : blocks)
      delete block;
   blocks.clear();
}

Object * Memory::allocate(unsigned int requestedSize) {
   cout << "DEBUG: requesting " << requestedSize << " B" << endl;
   unsigned char * address = this->getFreeAddress(requestedSize);
   if (!address) {
      cout << "DEBUG: collecting garbage" << endl;
      collectGarbage();
      address = this->getFreeAddress(requestedSize);
      if (address)
         cout << "DEBUG: memory freed" << endl;
   }
   if (address) {
      cout << "DEBUG: allocated address " << static_cast<void*> (address) << endl;
      return reinterpret_cast<Object*> (address);
   }
   cout << "DEBUG: increasing memory" << endl;
   blocks.push_back(new MemoryBlock());
   address = blocks.back()->allocate(requestedSize);
   cout << "DEBUG: allocated new address " << static_cast<void*> (address) << endl;

   return reinterpret_cast<Object*> (address);
}

unsigned char * Memory::getFreeAddress(unsigned int requestedSize) {
   unsigned char * address;
   for (MemoryBlock * block : blocks) {
      address = block->allocate(requestedSize);
      if (address)
         return address;
   }
   return nullptr;
}

void Memory::free(const Object * object) {
   ;
}

void Memory::collectGarbage() {
   mark();
   sweep();
}

void Memory::mark() {
   //TODO:
   //    markSymbols();
   //    markWellKnownGlobals();
   //    markObject(globalEnvironment);
   stack.markAll();
}

void Memory::sweep() {
   for (MemoryBlock * block : blocks)
      block->sweepAll();
}
