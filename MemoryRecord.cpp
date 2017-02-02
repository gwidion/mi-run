#include "MemoryRecord.h"

using namespace std;

MemoryRecord::MemoryRecord(unsigned char * address, int size) : address(address), size(size) { }

bool MemoryRecord::hasSpaceFor(unsigned int requestedSize) const {
   return size >= requestedSize;
}

unsigned char * MemoryRecord::allocate(unsigned int requestedSize) {
   if(!(this->hasSpaceFor(requestedSize)))
      throw "not enought memory in this record";
   unsigned char * allocated = address;
   address += requestedSize;
   size -= requestedSize;
   return allocated;
}

unsigned char * MemoryRecord::allocateAtEnd(unsigned int requestedSize) {
   if(!(this->hasSpaceFor(requestedSize)))
      throw "not enought memory in this record";
   unsigned char * allocated = address;
   size -= requestedSize;
   return allocated;
}

bool MemoryRecord::isEmpty() const {
   return size == 0;
}

