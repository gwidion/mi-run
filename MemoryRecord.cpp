#include "MemoryRecord.h"

using namespace std;

MemoryRecord::MemoryRecord(unsigned char * startAddress, unsigned int requestedSize) : startAddress(startAddress), size(requestedSize) {
    if (requestedSize == 0)
        throw "aaaa";
}

bool MemoryRecord::hasSpaceFor(unsigned int requestedSize) const {
    return size >= requestedSize;
}

bool MemoryRecord::hasMoreSpaceThan(unsigned int requestedSize) const {
    return size > requestedSize;
}

MemoryRecord MemoryRecord::allocated(unsigned int requestedSize) const {
    if (!(this->hasSpaceFor(requestedSize)))
        throw "not enought memory in this record";
    if (size <= requestedSize)
        throw "size has to be greater - somewhere is a problem!";
    return MemoryRecord(startAddress + requestedSize, size - requestedSize);
}

unsigned char * MemoryRecord::address() const {
    return startAddress;
}

unsigned int MemoryRecord::getSize() const {
    return size;
}

bool MemoryRecord::isEmpty() const {
    return size == 0;
}

unsigned char * MemoryRecord::next() const {
    return startAddress + size;
}

bool MemoryRecord::operator<(const MemoryRecord & other) const {
    return startAddress < other.address();
}