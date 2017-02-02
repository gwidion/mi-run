#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <unordered_map>

#include "Object.h"
#include "Stack.h"

//
// (+ <number>...)
// special: with 0 args returns 0 (neutral element of addition)
//

Object * builtinPlus(int numArgs, Stack* stack) {
    long int sum = 0;
    for (int i = 0; i < numArgs; i++) {
        Object* nextArg = stack->pop();
        sum += nextArg->intValue();
    }
    return new ObjectInt(sum);
}

class Environment {
public:

    Environment() : parent(nullptr), objectMap() {
        objectMap["+"] = builtinPlus;
    }


private:
    Environment* parent;
    std::unordered_map<std::string, ObjectFunction > objectMap;
};

#endif /* ENVIRONMENT_H */

