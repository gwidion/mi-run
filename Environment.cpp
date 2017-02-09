#include "Environment.h"
#include "Memory.h"

using namespace std;

Environment globalEnvironment;
extern Memory memory;

void popFromStack(int numArgs) {
    for (int i = 0; i < numArgs; i++) {
        memory.stack.pop();
    }
}

//
// (+ <number>...)
// special: with 0 args returns 0 (neutral element of addition)
//

Object* builtinPlus(int numArgs, Environment& environment) {
    long int sum = 0;
    for (int i = 0; i < numArgs; i++) {
        Object* nextArg = memory.stack.pop();
        sum += nextArg->getInt(environment);
    }
    return ObjectInt::allocate(sum);
}

//
// (- <number>...)
// (- 5) returns -5
// (- 3 2) returns 1
// (- 3 2 2) returns -1
//

Object* builtinMinus(int numArgs, Environment& environment) {
    if (numArgs == 0) {
        cout << "no args for minus operator";
        return ObjectVoid::allocate();
    } else if (numArgs == 1) {
        Object* nextArg = memory.stack.pop();
        int intValue = nextArg->getInt(environment);
        return ObjectInt::allocate(-intValue);

    } else {
        long int result = 0;
        for (int i = 0; i < numArgs; i++) {
            Object* nextArg = memory.stack.pop();
            if (i == numArgs - 1) // first number, needs to be added
                result += nextArg->getInt(environment);
            else // the rest subtracted
                result -= nextArg->getInt(environment);
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
        sum *= nextArg->getInt(environment);
    }
    return ObjectInt::allocate(sum);
}

//
// (= <number1> <number2>) -> boolean
//

Object* builtinEquals(int numArgs, Environment& environment) {
    if (numArgs != 2) {
        cout << "= expects exactly 2 arguments";
        popFromStack(numArgs);
        return ObjectVoid::allocate();
    }
    Object* first = memory.stack.pop();
    Object* second = memory.stack.pop();

    if (first->getInt(environment) == second->getInt(environment))
        return ObjectTrue::allocate();
    else
        return ObjectFalse::allocate();
}


//
// (< <number1> <number2>) -> boolean
//

Object* builtinLessThan(int numArgs, Environment& environment) {
    if (numArgs != 2) {
        cout << "< expects exactly 2 arguments";
        popFromStack(numArgs);
        return ObjectVoid::allocate();
    }

    // arguments are reversed on stack
    Object* second = memory.stack.pop();
    Object* first = memory.stack.pop();

    if (first->getInt(environment) < second->getInt(environment))
        return ObjectTrue::allocate();
    else
        return ObjectFalse::allocate();
}

//
// (> <number1> <number2>) -> boolean
//

Object* builtinGreaterThan(int numArgs, Environment& environment) {
    if (numArgs != 2) {
        cout << "> expects exactly 2 arguments";
        popFromStack(numArgs);
        return ObjectVoid::allocate();
    }

    // arguments are reversed on stack
    Object* second = memory.stack.pop();
    Object* first = memory.stack.pop();

    if (first->getInt(environment) > second->getInt(environment))
        return ObjectTrue::allocate();
    else
        return ObjectFalse::allocate();
}

//
// (if <cond> <trueExpr> <falseExpr>
//

Object* builtinIf(int numArgs, Environment& environment) {
    if (numArgs != 3) {
        cout << "if expects exactly 3 arguments";
        popFromStack(numArgs);
        return ObjectVoid::allocate();
    }

    // arguments are reversed on stack
    Object* ifFalseExpr = memory.stack.pop();
    Object* ifTrueExpr = memory.stack.pop();
    Object* condition = memory.stack.pop();

    Object* evaluatedCondition = condition->eval(environment);
    if (evaluatedCondition->getBool())
        return ifTrueExpr->eval(environment);
    else
        return ifFalseExpr->eval(environment);
}

//
// (lambda <argList> <bodyexpr>...)
//

Object* builtinLambda(int numArgs, Environment& environment) {
    if (numArgs != 2) {
        cout << "lambda expects 2 arguments";
        popFromStack(numArgs);
        return ObjectVoid::allocate();
    }

    // arguments are reversed on stack
    Object* bodyList = memory.stack.pop();
    Object* argList = memory.stack.pop();

    return ObjectUserDefinedFunction::allocate(argList, bodyList);
}

//
// (define var expr)
//

Object* builtinDefine(int numArgs, Environment& environment) {
    if (numArgs != 2) {
        cout << "define expects exactly 2 arguments";
        popFromStack(numArgs);
        return ObjectVoid::allocate();
    }

    // arguments are reversed on stack
    Object* expr = memory.stack.pop();
    Object* symbol = memory.stack.pop();

    Object* exprValue = expr->eval(environment);
    environment.addObject(symbol->getString(), exprValue);
    return ObjectVoid::allocate();
}

Environment::Environment() {
    parent = nullptr;
    addObject("+", ObjectBuiltInFunction::allocate(builtinPlus));
    addObject("-", ObjectBuiltInFunction::allocate(builtinMinus));
    addObject("*", ObjectBuiltInFunction::allocate(builtinTimes));
    addObject("=", ObjectBuiltInFunction::allocate(builtinEquals));
    addObject("<", ObjectBuiltInFunction::allocate(builtinLessThan));
    addObject(">", ObjectBuiltInFunction::allocate(builtinGreaterThan));
    addObject("if", ObjectBuiltInSyntax::allocate(builtinIf));
    addObject("define", ObjectBuiltInSyntax::allocate(builtinDefine));
    addObject("lambda", ObjectBuiltInSyntax::allocate(builtinLambda));
}

Environment::Environment(Environment& environment) {
    parent = &environment;
}

void Environment::addObject(std::string key, Object* object) {
    objectMap[key] = object;
}

Object* Environment::getObject(std::string key) {
    Object* object = objectMap[key];

    if (object != nullptr)
        return object;
    else if (parent != nullptr)
        return parent->getObject(key);
    else
        return nullptr;
}

void Environment::mark() {
    for (auto & pair : objectMap)
        if (pair.second)
            pair.second->mark();
    if (parent)
        parent->mark();
}