#ifndef MEMORYRECORD_H
#define MEMORYRECORD_H

class MemoryRecord {
public:
    MemoryRecord(unsigned char * startAddress, unsigned int requestedSize);

    bool hasSpaceFor(unsigned int requestedSize) const;
    MemoryRecord allocated(unsigned int requestedSize) const;
    unsigned char * address() const;
    unsigned int getSize() const;
    bool isEmpty() const;
    unsigned char * next() const;
    bool hasMoreSpaceThan(unsigned int requestedSize) const;

    bool operator<(const MemoryRecord & other) const;
private:
    unsigned char * startAddress;
    unsigned int size;
};

#endif /* MEMORYRECORD_H */

