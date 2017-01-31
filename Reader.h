#ifndef READER_H
#define READER_H

#include "Object.h"

class Reader {
public:
    Reader();
    Reader(const Reader& orig);
    virtual ~Reader();
    
    Object* read(FILE* input);
private:
    char peekChar;
    
    void unreadChar(char ch);
    char nextChar(FILE *input);
    bool isWhiteSpace(char c);
    bool isDigit(char c);
    void skipToEOL(FILE *input);
    char skipWhiteSpace(FILE *input);
    Object* readList(FILE *input);
    Object* readString(FILE *input);
    Object* readNumber(FILE *input, char charAlready);
    Object* readSymbol(FILE *input);
};

#endif /* READER_H */

