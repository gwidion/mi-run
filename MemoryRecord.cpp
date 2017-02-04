#include "MemoryRecord.h"

using namespace std;

MemoryRecord::MemoryRecord(unsigned char * address, unsigned int size) : address(address), size(size) { }

bool MemoryRecord::hasSpaceFor(unsigned int requestedSize) const {
   return size >= requestedSize;
}

unsigned char * MemoryRecord::allocate(unsigned int requestedSize) {
   if (!(this->hasSpaceFor(requestedSize)))
      throw "not enought memory in this record";
   address += requestedSize;
   size -= requestedSize;
   return address - requestedSize;
}

unsigned char * MemoryRecord::allocateAtEnd(unsigned int requestedSize) {
   if (!(this->hasSpaceFor(requestedSize)))
      throw "not enought memory in this record";
   size -= requestedSize;
   return address + size;
}

bool MemoryRecord::isEmpty() const {
   return size == 0;
}

unsigned char * MemoryRecord::next() {
   return address + size;
}

bool MemoryRecord::isAddress(unsigned char * requestedAddress) {
   return address == requestedAddress;
}
