

#include "Object.h"
#include "Environment.h"
#include "Stack.h"

Object* ObjectCons::eval(Environment& env, Stack& stack) {
	Object* func = car->eval( env, stack );
	if( func->tag == TAG_BUILTINFUNCTION ) {
		int initStackSize = stack.size();
		Object* restArgs = cdr;
		while( restArgs ) {
			Object* unevaluated = restArgs->carValue();
//			unevaluated->foo();
//			Object* evaluated = unevaluated->eval( env, stack );
			stack.push( unevaluated );
			restArgs = restArgs->cdrValue();
		}
		int args = stack.size() - initStackSize;
		return func->functionValue()( args, stack );
	}
	else {
		 return nullptr;
	}
}

Object* ObjectSymbol::eval(Environment& env, Stack& stack) {
	return env.getObject( s );
}