#include <fstream>

#include "Environment.h"
#include "Object.h"
#include "Reader.h"

#include "compareFiles.h"

extern Environment globalEnvironment;

using namespace std;

void repl( FILE* input = stdin ) {
	Reader reader;
	
	Object* expression;
	Object* result;

	// REPL
	while(true) {
		cout << "> ";
		expression = reader.read(input);
        if (expression->isNil()) {
			cout << "empty input, ending";
			return;
		}

        result = expression->eval(globalEnvironment);
        if (result->isNil()) {
			cout << "empty result, ending";
			return;
		}
		result->print();
		cout << endl;
	}
}

void testing() {
	string testIn = "testIn.txt";
	string testOut = "testOut.txt";
	string testOutReference = "testOutReference.txt";
	
   // streams
   // back up cout's streambuf
	streambuf *backupOut = cout.rdbuf();     
	ofstream out( testOut.c_str() );
	cout.rdbuf(out.rdbuf());
	
	// run and compute
   FILE* fp = fopen (testIn.c_str(), "r");
   repl( fp );
	
	// checkResults
    string testResult = compareFiles(testOutReference, testOut);
	
   // restore cout's original streambuf
	cout.rdbuf(backupOut);        
	out.close();
	
	if( testResult.empty() ) {
		cout << "tests passed OK" << endl;
    } else {
		cout << "tests failed, " << testResult << endl;
	}
}

int main() {	
	cout << "kabelja4 and bliznjan runtime system for scheme" << endl;
	testing();
	repl();

	return 0;
}