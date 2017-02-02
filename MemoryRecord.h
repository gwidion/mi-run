/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FreeMemory.h
 * Author: Honza
 *
 * Created on 2. února 2017, 14:38
 */

#ifndef MEMORYRECORD_H
#define MEMORYRECORD_H

class MemoryRecord {
public:
   MemoryRecord(unsigned char * address, int size);

   bool hasSpaceFor(unsigned int requestedSize) const;
   unsigned char * allocate(unsigned int requestedSize);
   unsigned char * allocateAtEnd(unsigned int requestedSize);
   bool isEmpty() const;
private:
   unsigned char * address;
   unsigned int size;
};

#endif /* MEMORYRECORD_H */

