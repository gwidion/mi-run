

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

unsigned int Object::size() {
   throw "Object should be abstract";
   return sizeof(Object);
}


unsigned int ObjectInt::size() {
   return sizeof(ObjectInt);
}

unsigned int ObjectBultInSyntax::size() {
   return sizeof(ObjectBultInSyntax);
}

unsigned int ObjectBuiltInFunction::size() {
   return sizeof(ObjectBuiltInFunction);
}

unsigned int ObjectSymbol::size() {
   return sizeof(ObjectSymbol);
}

unsigned int ObjectString::size() {
   return sizeof(ObjectString);
}

unsigned int ObjectCons::size() {
   return sizeof(ObjectCons);
}

unsigned int ObjectTrue::size() {
   return sizeof(ObjectTrue);
}

unsigned int ObjectFalse::size() {
   return sizeof(ObjectFalse);
}

unsigned int ObjectNil::size() {
   return sizeof(ObjectNil);
}
