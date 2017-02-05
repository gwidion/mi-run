#include <fstream>

#include "Environment.h"
#include "Object.h"
#include "Reader.h"

#include "compareFiles.h"

using namespace std;

extern Environment globalEnvironment;

//void initializeObjects() {
//    ObjectInt::allocate(1);
//    ObjectBuiltInFunction::allocate(nullptr);
//    ObjectCons::allocate(nullptr, nullptr);
//    ObjectString::allocate("");
//    ObjectTrue::allocate();
//    ObjectFalse::allocate();
//    ObjectNil::allocate();
//}

void repl(FILE * input = stdin) {
    Reader reader;

    Object* expression;
    Object* result;

    // REPL
    while (true) {
        //		if(feof(cin)) {
        //			return;
        //		}
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
    streambuf *backupOut = cout.rdbuf(); // back up cout's streambuf
    ofstream out(testOut.c_str());
    cout.rdbuf(out.rdbuf());

    // run and compute
    FILE* fp = fopen(testIn.c_str(), "r");
    repl(fp);

    // checkResults
    int testResult = compareFiles(testOutReference, testOut);

    cout.rdbuf(backupOut); // restore cout's original streambuf
    out.close();

    if (testResult == 0) {
        cout << "tests passed OK" << endl;
    } else {
        cout << "tests failed, first difference at line " << testResult << endl;
    }
}

int main() {
    //    initializeMemory();
    //    initializeSymbolTable();
    //    initializeWellKnownObjects();
    //    initializeGlobalEnvironment();
    //    defineWellknownBindings();
    //    initializeStack();

    // initializeObjects();

    cout << "kabelja4 and bliznjan runtime system for scheme" << endl;

    testing();

    repl();

    // only reached on EOF
    return 0;
}