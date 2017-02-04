/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Reader.cpp
 * Author: Honza
 * 
 * Created on 31. ledna 2017, 14:30
 */

#include "Reader.h"

Reader::Reader() {
	peekChar = 0;
}

Reader::Reader(const Reader& orig) {
}

Reader::~Reader() {
}

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
//			error("read error");
			// not reached.
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
	Object* theCar;
	Object* theCdr;

	if (nextChar == ')') {
		return nullptr;
//		return ObjectNil::allocate();
	}
	unreadChar(nextChar);
	theCar = read(input);
	theCdr = readList(input);
	return ObjectCons::allocate(theCar, theCdr);
}

Object* Reader::readString(FILE *input) {
	int currentBufferSize = 64;
	int currentStringLen = 0;
	char *buffer = (char *) (malloc(currentBufferSize));

	for (;;) {
		char ch = nextChar(input);
		if (ch == '\"') {
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

	while(true) {
		char ch = nextChar(input);
		if (isWhiteSpace(ch)
				  || (ch == '(')
				  || (ch == ')')) {
			Object* newSymbolObject;

			unreadChar(ch);
			buffer[currentSymbolLen] = '\0';
			if (buffer[0] == '#') {
				switch (buffer[1]) {
					case 't':
						free(buffer);
						return ObjectTrue::allocate();
					case 'f':
						free(buffer);
						return ObjectFalse::allocate();
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

	while(true) {
		nextChar = skipWhiteSpace(input);

		if (nextChar == -1) { // end-of-file
			return nullptr;
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
				Object* expr;

				expr = read(input);
				return ObjectCons::allocate( expr, ObjectNil::allocate() );
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