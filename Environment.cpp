#include "Environment.h"

#include "Stack.h"

//
// (+ <number>...)
// special: with 0 args returns 0 (neutral element of addition)
//

Object* builtinPlus(int numArgs, Stack* stack) {
    long int sum = 0;
    for (int i = 0; i < numArgs; i++) {
        Object* nextArg = stack->pop();
        sum += nextArg->intValue();
    }
    return new ObjectInt(sum);
}

Environment::Environment() {
	parent = nullptr;
	objectMap["+"] = (ObjectFunction*)builtinPlus;
}