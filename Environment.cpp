#include "Environment.h"
#include "Memory.h"
#include <limits.h>

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
        long int number = nextArg->getInt(environment);
        if (((sum > 0) && (number > INT_MAX - sum)) || ((sum < 0) && (number < INT_MIN - sum))) {
           popFromStack(numArgs - i);
           throw runtime_error("plus under/overflow");
        }
        sum += number;
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
        throw runtime_error("no args for minus operator");
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
            else { // the rest subtracted
               int number = nextArg->getInt(environment);
               if (((result < 0) && (number > INT_MAX + result)) || ((result > 0) && (number < INT_MIN + result))) {
                  popFromStack(numArgs - i);
                  throw runtime_error("minus under/overflow");
               }
               result -= number;
            }
        }
        return ObjectInt::allocate(result);
    }
}

//
// (* <number>...)
// special: with 0 args returns 1 (neutral element of multiplication)
//

Object* builtinTimes(int numArgs, Environment& environment) {
    long int result = 1;
    for (int i = 0; i < numArgs; i++) {
        Object* nextArg = memory.stack.pop();
        int number = nextArg->getInt(environment);
        if ((result > INT_MAX / number) || ((result < INT_MIN / number))) {
            popFromStack(numArgs - i);
            throw runtime_error("multiplication under/overflow");
        }
        else if(((result == -1) && (number == INT_MIN)) || ((number == -1) && (result == INT_MIN))) {
            popFromStack(numArgs - i);
            throw runtime_error("multiplication possible under/overflow");
        }
        result *= number;
    }
    return ObjectInt::allocate(result);
}

//
// (= <number1> <number2>) -> boolean
//

Object* builtinEquals(int numArgs, Environment& environment) {
    if (numArgs != 2) {
        popFromStack(numArgs);
        throw runtime_error("= expects exactly 2 arguments");
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
        popFromStack(numArgs);
        throw runtime_error("< expects exactly 2 arguments");
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
        popFromStack(numArgs);
        throw runtime_error("if expects exactly 3 arguments");
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
        popFromStack(numArgs);
        throw runtime_error("lambda expects 2 arguments");
    }

    return ObjectUserDefinedFunction::fromStack();
}

//
// (define var expr)
//

Object* builtinDefine(int numArgs, Environment& environment) {
    if (numArgs != 2) {
        popFromStack(numArgs);
        throw runtime_error("define expects exactly 2 arguments");
    }

    // arguments are reversed on stack
    Object* expr = memory.stack.pop();
    Object* symbol = memory.stack.pop();

    Object* exprValue = expr->eval(environment);
    environment.addObject(symbol->getString(), exprValue);
    return ObjectVoid::allocate();
}

//
// (car <aCons>) -> CAR(aCons)
//
Object* builtinCar(int numArgs, Environment& environment) {
   if( numArgs != 1 ) {
        popFromStack(numArgs);
        throw runtime_error("car expects exactly 1 argument ");
   }

   Object* theCell = memory.stack.pop();
   return theCell->getCar();
}

//
// (cdr <aCons>) -> CDR(aCons)
//
Object* builtinCdr(int numArgs, Environment& environment) {
   if( numArgs != 1 ) {
        popFromStack(numArgs);
        throw runtime_error("cdr expects exactly 1 argument ");
   }

   Object* theCell = memory.stack.pop();
   return theCell->getCdr();
}

Environment::Environment() {
    parent = nullptr;
    addObject("+", ObjectBuiltInFunction::allocate(builtinPlus));
    addObject("-", ObjectBuiltInFunction::allocate(builtinMinus));
    addObject("*", ObjectBuiltInFunction::allocate(builtinTimes));
    addObject("=", ObjectBuiltInFunction::allocate(builtinEquals));
    addObject("<", ObjectBuiltInFunction::allocate(builtinLessThan));
    addObject(">", ObjectBuiltInFunction::allocate(builtinGreaterThan));
    addObject("car", ObjectBuiltInFunction::allocate(builtinCar));
    addObject("cdr", ObjectBuiltInFunction::allocate(builtinCdr));
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