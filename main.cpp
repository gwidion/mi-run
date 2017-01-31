#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "Object.h"
#include "Reader.h"

using namespace std;

int main() {
	//    initializeMemory();
	//    initializeSymbolTable();
	//    initializeWellKnownObjects();
	//    initializeGlobalEnvironment();
	//    defineWellknownBindings();
	//    initializeStack();


	cout << "kabelja4 and bliznjan runtime system for scheme" << endl;

	Reader reader;
	
	Object* expression;
	Object* result;

	// main loop with read - evaluate - print
	while(true) {
//		if(feof(cin)) {
//			return;
//		}
		cout << "> ";
		expression = reader.read(stdin);
		
//		if (expression == SCM_EOF) {
//			return;
//		}

		result = expression->eval();
		result->print();
		cout << endl;
	}

	// only reached on EOF
	return 0;
}