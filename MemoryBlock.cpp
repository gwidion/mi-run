#include "MemoryBlock.h"

using namespace std;

MemoryBlock::MemoryBlock() {
   data = new unsigned char [1048576];
   freeAddresses.push_back(MemoryRecord(data, 1048576));
}

MemoryBlock::~MemoryBlock() {
   delete [] data;
   data = nullptr;
}

bool MemoryBlock::hasSpaceFor(unsigned int requestedSize) const {
   for (const MemoryRecord & record : freeAddresses)
      if (record.hasSpaceFor(requestedSize))
         return true;
   return false;
}

unsigned char * MemoryBlock::allocate(unsigned int requestedSize) {
   MemoryRecord * address = this->recordFor(requestedSize);
   if (!address)
      throw "unable to allocate in this block";
   return address->allocate(requestedSize);
}

MemoryRecord * MemoryBlock::recordFor(unsigned int requestedSize) {
   for (MemoryRecord & record : freeAddresses)
      if (record.hasSpaceFor(requestedSize))
         return &record;
   return nullptr;
}