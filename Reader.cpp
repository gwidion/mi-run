#include "Reader.h"

#include <string.h>

using namespace std;

extern Memory memory;

Reader::Reader() {
    peekChar = 0;
}

Reader::Reader(const Reader& orig) { }

Reader::~Reader() { }

void Reader::unreadChar(char ch) {
    if (peekChar != 0) {
        //		error("cannot push back 2 characters");
    }
    peekChar = ch;
}

char Reader::nextChar(FILE *input) {
    char nextChar;

    if (peekChar != 0) {
        nextChar = peekChar;
        peekChar = 0;
        return nextChar;
    }
    for (;;) {
        nextChar = fgetc(input);
        if (nextChar >= 0)
            return nextChar;
        if (feof(input)) {
            return -1;
        }
        if (errno != EINTR) {
            throw runtime_error("read error");
        } else {
            // read again.
        }
    }
    // not reached; but to make the compiler happy...
    return 0;
}

bool Reader::isWhiteSpace(char c) {
    return ((c == ' ') || (c == '\t') || (c == '\n') || (c == '\r')) ?
            true : false;
}

bool Reader::isDigit(char c) {
    return (c >= '0') && (c <= '9');
}

void Reader::skipToEOL(FILE *input) {
    int c;

    for (;;) {
        c = nextChar(input);
        if (c == '\n') return;
    }
}

char Reader::skipWhiteSpace(FILE *input) {
    char c;

    do {
        c = nextChar(input);
        if (c == ';') {
            skipToEOL(input);
            c = ' '; // to get isWhiteSpace tricked.
        }
    } while (isWhiteSpace(c));
    return c;
}

Object* Reader::readList(FILE *input) {
    char nextChar = skipWhiteSpace(input);

    if (nextChar == ')')
        return ObjectNil::allocate();

    unreadChar(nextChar);
    memory.stack.push(read(input)); //car
    memory.stack.push(readList(input)); //cdr
    return ObjectCons::fromStack(); //cons
}

Object* Reader::readString(FILE *input) {
    int currentBufferSize = 64;
    int currentStringLen = 0;
    char *buffer = (char *) (malloc(currentBufferSize));

    for (;;) {
        char ch = nextChar(input);
        if (ch == '\\') {
            ch = nextChar();

            if (ch == -1) {
               runtime_error("unterminated string");
            }
            switch (ch) {
            case 'n':
               ch = '\n';
               break;
            case 'r':
               ch = '\r';
               break;
            case 't':
               ch = '\t';
               break;
            default:
               break;
            }
        }
        else if (ch == '\"') {
            Object* newStringObject;

            buffer[currentStringLen] = '\0';
            newStringObject = ObjectString::allocate(buffer);
            free(buffer);
            return (newStringObject);
        }

        buffer[currentStringLen] = ch;
        currentStringLen++;
        if (currentStringLen == currentBufferSize) {
            int newBufferSize = currentBufferSize * 2;
            buffer = (char *) realloc(buffer, newBufferSize);
            currentBufferSize = newBufferSize;
        }
    }
}

Object* Reader::readNumber(FILE *input, char charAlready) {
    long intVal;
    char c;

    intVal = charAlready - '0';
    while (isDigit(c = nextChar(input))) {
        intVal = intVal * 10 + (c - '0');
    }
    // I have read one too many
    unreadChar(c);
    return ObjectInt::allocate(intVal);
}

Object* Reader::readSymbol(FILE *input) {
    int currentBufferSize = 64;
    int currentSymbolLen = 0;
    char *buffer = (char *) (malloc(currentBufferSize));

    while (true) {
        char ch = nextChar(input);
        if (isWhiteSpace(ch)
                || (ch == '(')
                || (ch == ')')) {
            Object* newSymbolObject;

            unreadChar(ch);
            buffer[currentSymbolLen] = '\0';
            if (buffer[0] == '#') {
                if(strcmp(buffer, "#t") == 0) {
                     free(buffer);
                     return ObjectTrue::allocate();
                }
                if(strcmp(buffer, "#f") == 0) {
                     free(buffer);
                     return ObjectFalse::allocate();
                }
                if(strcmp(buffer, "#void") == 0) {
                     free(buffer);
                     return ObjectVoid::allocate();
                }
            }
            newSymbolObject = ObjectSymbol::allocate(buffer);
            free(buffer);
            return (newSymbolObject);
        }

        buffer[currentSymbolLen] = ch;
        currentSymbolLen++;
        if (currentSymbolLen == currentBufferSize) {
            int newBufferSize = currentBufferSize * 2;
            buffer = (char *) realloc(buffer, newBufferSize);
            currentBufferSize = newBufferSize;
        }
    }
}

Object* Reader::read(FILE* input) {
    Object* someObject;
    char nextChar;

    while (true) {
        nextChar = skipWhiteSpace(input);

        if (nextChar == -1) { // end-of-file
            return ObjectNil::allocate();
        }
        //
        //		DEBUGCODE(debugReader,{
        //			printf("the next char is %x (%c)\n", nextChar, nextChar);
        //		});

        switch (nextChar) {
            case '"':
                someObject = readString(input);
                return someObject;

            case '(':
                someObject = readList(input);
                return someObject;

            case '\'':
            {
                memory.stack.push(read(input));
                memory.stack.push(ObjectNil::allocate());
                return ObjectCons::fromStack();
            }
            case ')':
                std::cout << "')' unexpected";
            case '.':
                std::cout << "'.' unexpected";
            default:
                if (isDigit(nextChar)) {
                    someObject = readNumber(input, nextChar);
                    return someObject;
                }
                unreadChar(nextChar);
                someObject = readSymbol(input);
                return someObject;
        }
    }
}