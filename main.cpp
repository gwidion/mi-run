#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "Environment.h"
#include "Object.h"
#include "Reader.h"
#include "Stack.h"

using namespace std;

int main() {
	//    initializeMemory();
	//    initializeSymbolTable();
	//    initializeWellKnownObjects();
	//    initializeGlobalEnvironment();
	//    defineWellknownBindings();
	//    initializeStack();

	Object a();
	ObjectInt i( 1 );
	ObjectBuiltInFunction bif( nullptr );
	ObjectCons c( nullptr, nullptr );
	ObjectString s( "" );

	cout << "kabelja4 and bliznjan runtime system for scheme" << endl;
//   FILE* fp = fopen ("input.txt", "r");

	Reader reader;
	Environment environment;
	Stack stack;
	
	Object* expression;
	Object* result;

	// main loop with read - evaluate - print
	while(true) {
//		if(feof(cin)) {
//			return;
//		}
		cout << "> ";
//		expression = reader.read(fp);
		expression = reader.read(stdin);
		
//		if (expression == SCM_EOF) {
//			return;
//		}

		result = expression->eval( environment, stack );
		result->print();
		cout << endl;
	}

	// only reached on EOF
	return 0;
}