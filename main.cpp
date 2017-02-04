#include <fstream>

#include "Environment.h"
#include "Object.h"
#include "Reader.h"

#include "compareFiles.h"

using namespace std;

extern Environment globalEnvironment;

void initializeObjects() {
    ObjectInt * i = ObjectInt::allocate(1);
    ObjectBuiltInFunction * bif = ObjectBuiltInFunction::allocate(nullptr);
    ObjectCons * c = ObjectCons::allocate(nullptr, nullptr);
    ObjectString * s = ObjectString::allocate("");
    ObjectTrue * t = ObjectTrue::allocate();
    ObjectFalse * f = ObjectFalse::allocate();
    ObjectNil * n = ObjectNil::allocate();
}

void repl(FILE * input = stdin, ostream & output = cout) {
    Reader reader;

    Object* expression;
    Object* result;

    // REPL
    while (true) {
        //		if(feof(cin)) {
        //			return;
        //		}
        output << "> ";
        expression = reader.read(input);
        if (expression == nullptr) {
            output << "empty input, ending";
            return;
        }

        result = expression->eval(globalEnvironment);
        if (result == nullptr) {
            output << "empty result, ending";
            return;
        }
        result->print(output);
        output << endl;
    }
}

void testing() {
    string testIn = "testIn.txt";
    string testOut = "testOut.txt";
    string testOutReference = "testOutReference.txt";
    // streams
    streambuf *backupOut = cout.rdbuf(); // back up cout's streambuf
    ofstream out(testOut.c_str());
    //cout.rdbuf(out.rdbuf());

    // run and compute
    FILE* fp = fopen(testIn.c_str(), "r");
    repl(fp, out);

    // checkResults
    int testResult = compareFiles(testOutReference, testOut);

    //cout.rdbuf(backupOut); // restore cout's original streambuf
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