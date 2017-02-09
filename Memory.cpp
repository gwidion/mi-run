#include "Memory.h"
#include "Stack.h"
#include "Environment.h"

using namespace std;

Memory memory;
extern Environment globalEnvironment;

Memory::Memory() {
    blocks.push_back(new MemoryBlock());
}

Memory::~Memory() {
    for (MemoryBlock * block : blocks)
        delete block;
    blocks.clear();
}

Object * Memory::allocate(unsigned int requestedSize) {
    unsigned char * address = this->tryAllocate(requestedSize);
    if (freeBlock >= 0 && !address) {
#ifdef DEBUG
        cout << "DEBUG: collecting garbage to free " << requestedSize << " B" << endl;
#endif
        collectGarbage();
        freeBlock = -1;
#ifdef DEBUG
        cout << "DEBUG: garbage collected - mode set to pick free spaces";
#endif
        address = this->tryAllocate(requestedSize);
#ifdef DEBUG
        if (address)
            cout << " - freed enough memory";
        cout << endl;
#endif
    }
    if (address)
        return reinterpret_cast<Object*> (address);
    freeBlock = blocks.size();
#ifdef DEBUG
        cout << "DEBUG: mode set to always append to end" << endl;
#endif
    blocks.push_back(new MemoryBlock());
    address = blocks.back()->allocateAtEnd(requestedSize);
    if (!address)
        throw runtime_error("no address");
    return reinterpret_cast<Object*> (address);
}

unsigned char * Memory::tryAllocate(unsigned int requestedSize) {
    if (freeBlock >= 0)
        return blocks[freeBlock]->allocateAtEnd(requestedSize);
    unsigned char * address;
    for (MemoryBlock * block : blocks) {
        address = block->allocate(requestedSize);
        if (address)
            return address;
    }
    return nullptr;
}

void Memory::collectGarbage() {
    mark();
    sweep();
}

void Memory::mark() {
    globalEnvironment.mark();
    stack.mark();
}

void Memory::sweep() {
    for (MemoryBlock * block : blocks)
        block->sweep();
}
