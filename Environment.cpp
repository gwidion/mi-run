#include "Environment.h"
#include "Stack.h"

using namespace std;

Environment globalEnvironment;

//
// (+ <number>...)
// special: with 0 args returns 0 (neutral element of addition)
//

Object* builtinPlus(int numArgs, Stack& stack) {
    long int sum = 0;
    for (int i = 0; i < numArgs; i++) {
        Object* nextArg = stack.pop();
        sum += nextArg->intValue();
    }
    return ObjectInt::allocate(sum);
}

//
// (- <number>...)
// (- 5) returns -5
// (- 3 2) return 1
// (- 3 2 2) return -1
//

Object* builtinMinus(int numArgs, Stack& stack) {
    if (numArgs == 0) {
        cout << "no args for minus operator";
        return ObjectVoid::allocate();
    } else if (numArgs == 1) {
        Object* nextArg = stack.pop();
        int intValue = nextArg->intValue();
        return ObjectInt::allocate(-intValue);

    } else {
        long int result = 0;
        for (int i = 0; i < numArgs; i++) {
            Object* nextArg = stack.pop();
            if (i == numArgs - 1) // first number, needs to be added
                result += nextArg->intValue();
            else // the rest subtracted
                result -= nextArg->intValue();
        }
        return ObjectInt::allocate(result);
    }
}

//
// (* <number>...)
// special: with 0 args returns 1 (neutral element of multiplication)
//

Object* builtinTimes(int numArgs, Stack& stack) {
    long int sum = 1;
    for (int i = 0; i < numArgs; i++) {
        Object* nextArg = stack.pop();
        sum *= nextArg->intValue();
    }
    return ObjectInt::allocate(sum);
}

//
// (= <number1> <number2>) -> boolean
//

Object* builtinEquals(int numArgs, Stack& stack) {
    if (numArgs != 2) {
        cout << "= expects exactly 2 arguments ";
        return ObjectVoid::allocate();
    }
    Object* first = stack.pop();
    Object* second = stack.pop();

    if (first->intValue() == second->intValue())
        return ObjectTrue::allocate();
    else
        return ObjectFalse::allocate();
}


//
// (< <number1> <number2>) -> boolean
//

Object* builtinLessThan(int numArgs, Stack& stack) {
    if (numArgs != 2) {
        cout << "< expects exactly 2 arguments ";
        return ObjectVoid::allocate();
    }

    // arguments are reversed on stack
    Object* second = stack.pop();
    Object* first = stack.pop();

    if (first->intValue() < second->intValue())
        return ObjectTrue::allocate();
    else
        return ObjectFalse::allocate();
}

//
// (> <number1> <number2>) -> boolean
//

Object* builtinGreaterThan(int numArgs, Stack& stack) {
    if (numArgs != 2) {
        cout << "> expects exactly 2 arguments ";
        return ObjectVoid::allocate();
    }

    // arguments are reversed on stack
    Object* second = stack.pop();
    Object* first = stack.pop();

    if (first->intValue() > second->intValue())
        return ObjectTrue::allocate();
    else
        return ObjectFalse::allocate();
}

Environment::Environment() {
    parent = nullptr;
    objectMap["+"] = ObjectBuiltInFunction::allocate(builtinPlus);
    objectMap["-"] = ObjectBuiltInFunction::allocate(builtinMinus);
    objectMap["*"] = ObjectBuiltInFunction::allocate(builtinTimes);
    objectMap["="] = ObjectBuiltInFunction::allocate(builtinEquals);
    objectMap["<"] = ObjectBuiltInFunction::allocate(builtinLessThan);
    objectMap[">"] = ObjectBuiltInFunction::allocate(builtinGreaterThan);
}

void Environment::mark() {
    for (auto & pair : objectMap)
        if (pair.second)
            pair.second->mark();
}

void Environment::markWithParents() {
    this->mark();
    if (parent)
        parent->mark();
}