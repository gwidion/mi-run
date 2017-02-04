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
    set<MemoryRecord>::iterator iterator;
    for (iterator = freeAddresses.begin(); iterator != freeAddresses.end(); iterator++) {
        if (iterator->hasSpaceFor(requestedSize))
            break; // found big enough free address
    }
    if (iterator == freeAddresses.end())
        return nullptr; // no free address found
    unsigned char * address = iterator->address();
    if (iterator->hasMoreSpaceThan(requestedSize)) // if free size == requestedSize, this would create 0 free size record
        freeAddresses.insert(iterator->allocated(requestedSize));
    freeAddresses.erase(iterator);

    //    this->print();
    return address;
}

void MemoryBlock::sweep() {
    vector<Object *> unused;
    set<MemoryRecord>::const_iterator nextFree = freeAddresses.begin();
    unsigned char * address = data;
    while (address < data + blockSize) {
        bool aa = nextFree != freeAddresses.end();
        bool bb = false;
        if (aa)
            bb = nextFree->address() == address;
        if (nextFree != freeAddresses.end() && (nextFree->address() == address)) {
            // address is amoungst free ones
            cout << "DEBUG: FREE from " << static_cast<void*> (nextFree->address()) << " to " << static_cast<void*> (nextFree->next()) << endl;
            address = nextFree->next();
            nextFree++;
        } else {
            // there is object in address
            Object * object = reinterpret_cast<Object *> (address);
            unsigned int size = object->size();
            if (object->freed) {
                throw runtime_error("freed object!");
            }
            if (!object->isMarked()) {
                cout << "DEBUG: freed ";
                object->print();
                cout << " at " << static_cast<void*> (object) << endl;
                cout.flush();
                unused.push_back(object);
            } else {
                cout << "DEBUG: in use ";
                object->print();
                cout << " at " << static_cast<void*> (object) << endl;
                object->unMark();
            }
            address += size;
        }
    }
    for (Object * object : unused)
        this->free(object);
}

void MemoryBlock::free(Object * object) {
    object->freed = true;
    unsigned char * address = reinterpret_cast<unsigned char *> (object);
    unsigned int size = object->size();

    // merge new free space with previous
    for (set<MemoryRecord>::iterator iterator = freeAddresses.begin(); iterator != freeAddresses.end(); iterator++) {
        if (iterator->next() == address) {
            address = iterator->address();
            size += iterator->getSize();
            freeAddresses.erase(iterator);
            break;
        }
        if (iterator->address() > address)
            break;
    }

    // merge new free space with next
    for (set<MemoryRecord>::iterator iterator = freeAddresses.begin(); iterator != freeAddresses.end(); iterator++) {
        if (iterator->address() == address + size) {
            size += iterator->getSize();
            freeAddresses.erase(iterator);
            break;
        }
    }
    freeAddresses.insert(MemoryRecord(address, size));
}