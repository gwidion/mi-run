#include <fstream>
#include <stdexcept>

#include "Environment.h"
#include "Object.h"
#include "Reader.h"

#include "compareFiles.h"
#include <chrono>

extern Environment globalEnvironment;

using namespace std;
using namespace std::chrono;

void repl(FILE* input = stdin) {
    Reader reader;

    Object* expression;
    Object* result;

    // REPL
    while (true) {
        cout << "> ";
        expression = reader.read(input);
        if (expression->isNil()) {
            cout << "empty input, ending" << endl;
            return;
        }
        try {
            result = expression->eval(globalEnvironment);
            if (result->isNil()) {
                cout << "empty result, ending" << endl;
                return;
            }
            result->print();
        } catch (runtime_error& err) {
            cout << err.what();
        }
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
    ofstream out(testOut.c_str());
    cout.rdbuf(out.rdbuf());

    // run and compute
    FILE* fp = fopen(testIn.c_str(), "r");
    high_resolution_clock::time_point start = high_resolution_clock::now();
    repl(fp);
    high_resolution_clock::time_point end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();
    cout << (string("DEBUG: IT TOOK ") + to_string(duration) + " us") << endl;

    // checkResults
    string testResult = compareFiles(testOut, testOutReference);

    // restore cout's original streambuf
    cout.rdbuf(backupOut);
    out.close();

    if (testResult.empty()) {
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