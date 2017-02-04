#include "Memory.h"
#include "Stack.h"
#include "Environment.h"

using namespace std;

Memory memory;
extern Environment globalEnvironment;

Memory::Memory() { }

Memory::~Memory() {
    for (MemoryBlock * block : blocks)
        delete block;
    blocks.clear();
}

Object * Memory::allocate(unsigned int requestedSize) {
    cout << "DEBUG: requesting " << requestedSize << " B" << endl;
    unsigned char * address = this->tryAllocate(requestedSize);
    if (!address) {
        cout << "DEBUG: collecting garbage" << endl;
        collectGarbage();
        address = this->tryAllocate(requestedSize);
        if (address)
            cout << "DEBUG: freed enough memory" << endl;
    }
    if (address) {
        cout << "DEBUG: allocated address " << static_cast<void*> (address) << endl;
        return reinterpret_cast<Object*> (address);
    }
    cout << "DEBUG: increasing memory" << endl;
    blocks.push_back(new MemoryBlock());
    address = blocks.back()->allocate(requestedSize);
    cout << "DEBUG: allocated new address " << static_cast<void*> (address) << endl;
    if (!address)
        throw runtime_error("no address");
    return reinterpret_cast<Object*> (address);
}

unsigned char * Memory::tryAllocate(unsigned int requestedSize) {
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
