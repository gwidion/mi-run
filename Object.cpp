#include "Object.h"
#include "Environment.h"

Object* ObjectCons::eval(Environment& env) {
	Object* func = car->eval( env );
	if( func->tag == TAG_BUILTINSYNTAX ) {

		 std::stack<Object*> st;
		 st.push( cdr );
		 return func->eval( env );
	}
	else {
		 return nullptr;
	}
}

Object* ObjectSymbol::eval(Environment& env) {
	int args;
}