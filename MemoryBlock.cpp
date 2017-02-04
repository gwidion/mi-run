#include "MemoryBlock.h"
#include "Object.h"

using namespace std;
static const unsigned int blockSize = 100;

MemoryBlock::MemoryBlock() {
   //data = new unsigned char [1048576]; // 1 MB
   //freeAddresses.push_back(MemoryRecord(data, 1048576));
   data = new unsigned char [blockSize]; // 100 B
   freeAddresses.insert(MemoryRecord(data, blockSize));

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
   MemoryRecord * record = this->recordFor(requestedSize);
   if (!record)
      return nullptr;
   return record->allocate(requestedSize);
}

MemoryRecord * MemoryBlock::recordFor(unsigned int requestedSize) {
   for (MemoryRecord & record : freeAddresses)
      if (record.hasSpaceFor(requestedSize))
         return &record;
   return nullptr;
}

void MemoryBlock::sweepAll() {
   set<MemoryRecord>::iterator nextFree = freeAddresses.begin();
   unsigned char * address = data;
   while (address < data + blockSize) {
      if (nextFree != freeAddresses.end() && nextFree->isAddress(address)) {
         address = nextFree->next();
         nextFree++;
      } else {
         Object * object = reinterpret_cast<Object *> (address);
         unsigned int size = object->size();
         if (!object->marked)
            this->free(object);
         address += size;
      }
   }
}

void MemoryBlock::free(Object * object) {
   freeAddresses.insert(MemoryRecord(reinterpret_cast<unsigned char *> (object), object->size()));
}