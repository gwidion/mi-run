#include "Environment.h"
#include "Memory.h"

using namespace std;

Environment globalEnvironment;
extern Memory memory;

//
// (+ <number>...)
// special: with 0 args returns 0 (neutral element of addition)
//

Object* builtinPlus(int numArgs, Environment& environment) {
    long int sum = 0;
    for (int i = 0; i < numArgs; i++) {
        Object* nextArg = memory.stack.pop();
        sum += nextArg->intValue(environment);
    }
    return ObjectInt::allocate(sum);
}

//
// (- <number>...)
// (- 5) returns -5
// (- 3 2) return 1
// (- 3 2 2) return -1
//

Object* builtinMinus(int numArgs, Environment& environment) {
    if (numArgs == 0) {
        cout << "no args for minus operator";
        return ObjectVoid::allocate();
    } else if (numArgs == 1) {
        Object* nextArg = memory.stack.pop();
        int intValue = nextArg->intValue(environment);
        return ObjectInt::allocate(-intValue);

    } else {
        long int result = 0;
        for (int i = 0; i < numArgs; i++) {
            Object* nextArg = memory.stack.pop();
            if (i == numArgs - 1) // first number, needs to be added
                result += nextArg->intValue(environment);
            else // the rest subtracted
                result -= nextArg->intValue(environment);
        }
        return ObjectInt::allocate(result);
    }
}

//
// (* <number>...)
// special: with 0 args returns 1 (neutral element of multiplication)
//

Object* builtinTimes(int numArgs, Environment& environment) {
    long int sum = 1;
    for (int i = 0; i < numArgs; i++) {
        Object* nextArg = memory.stack.pop();
        sum *= nextArg->intValue(environment);
    }
    return ObjectInt::allocate(sum);
}

//
// (= <number1> <number2>) -> boolean
//

Object* builtinEquals(int numArgs, Environment& environment) {
    if (numArgs != 2) {
        cout << "= expects exactly 2 arguments ";
        return ObjectVoid::allocate();
    }
    Object* first = memory.stack.pop();
    Object* second = memory.stack.pop();

    if (first->intValue(environment) == second->intValue(environment))
        return ObjectTrue::allocate();
    else
        return ObjectFalse::allocate();
}


//
// (< <number1> <number2>) -> boolean
//

Object* builtinLessThan(int numArgs, Environment& environment) {
    if (numArgs != 2) {
        cout << "< expects exactly 2 arguments ";
        return ObjectVoid::allocate();
    }

    // arguments are reversed on stack
    Object* second = memory.stack.pop();
    Object* first = memory.stack.pop();

    if (first->intValue(environment) < second->intValue(environment))
        return ObjectTrue::allocate();
    else
        return ObjectFalse::allocate();
}

//
// (> <number1> <number2>) -> boolean
//

Object* builtinGreaterThan(int numArgs, Environment& environment) {
    if (numArgs != 2) {
        cout << "> expects exactly 2 arguments ";
        return ObjectVoid::allocate();
    }

    // arguments are reversed on stack
    Object* second = memory.stack.pop();
    Object* first = memory.stack.pop();

    if (first->intValue(environment) > second->intValue(environment))
        return ObjectTrue::allocate();
    else
        return ObjectFalse::allocate();
}

//
// (if <cond> <trueExpr> <falseExpr>
//
//Object* builtinIf(environment env) {

Object* builtinIf(int numArgs, Environment& environment) {
    if (numArgs != 3) {
        cout << "if expects exactly 3 arguments ";
        return ObjectVoid::allocate();
    }

    Object* ifFalseExpr = memory.stack.pop();
    Object* ifTrueExpr = memory.stack.pop();
    Object* condition = memory.stack.pop();

    Object* evaluatedCondition = condition->eval(environment);
    if (evaluatedCondition->boolValue())
        return ifTrueExpr->eval(environment);
    else
        return ifFalseExpr->eval(environment);
}

Environment::Environment() {
    parent = nullptr;
    objectMap["+"] = ObjectBuiltInFunction::allocate(builtinPlus);
    objectMap["-"] = ObjectBuiltInFunction::allocate(builtinMinus);
    objectMap["*"] = ObjectBuiltInFunction::allocate(builtinTimes);
    objectMap["="] = ObjectBuiltInFunction::allocate(builtinEquals);
    objectMap["<"] = ObjectBuiltInFunction::allocate(builtinLessThan);
    objectMap[">"] = ObjectBuiltInFunction::allocate(builtinGreaterThan);
    objectMap["if"] = ObjectBuiltInFunction::allocate(builtinIf);
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