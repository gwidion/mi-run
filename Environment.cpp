#include "Environment.h"
#include "Stack.h"

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
    return new ObjectInt(sum);
}

//
// (- <number>...)
// (- 5) returns -5
// (- 3 2) return 1
// (- 3 2 2) return -1
//
Object* builtinMinus(int numArgs, Stack& stack) {
	if( numArgs == 0 ) {
//		error
		return nullptr;
	}
	else if( numArgs == 1 ) {
		Object* nextArg = stack.pop();
		int intValue = nextArg->intValue();
		return new ObjectInt( -intValue );
		
	}
	else {
		Object* firstArg = stack.pop();
		long int result = firstArg->intValue();
		for (int i = 1; i < numArgs; i++) {
			Object* nextArg = stack.pop();
			result -= nextArg->intValue();
		}
		return new ObjectInt(result);
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
    return new ObjectInt(sum);
}



Environment::Environment() {
	parent = nullptr;
	objectMap["+"] = new ObjectBuiltInFunction( builtinPlus );
	objectMap["-"] = new ObjectBuiltInFunction( builtinMinus );
	objectMap["*"] = new ObjectBuiltInFunction( builtinTimes );
}