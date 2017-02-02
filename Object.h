#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>

class Environment;
class Object;
class Stack;

typedef Object* (*ObjectFunction)(int, Stack&);

enum tagEnum {
    TAG_INT,
    TAG_TRUE,
    TAG_FALSE,
    TAG_NIL,
    TAG_VOID,
    TAG_EOF,
    TAG_CONS,
    TAG_STRING,
    TAG_SYMBOL,
    TAG_BUILTINFUNCTION,
    TAG_BUILTINSYNTAX,
    TAG_USERDEFINEDFUNCTION,
};

class Object {
public:
    Object();
    Object( tagEnum t ) : tag( t ) {}
    virtual void print() { std::cout << ""; }
    virtual Object* eval(Environment& env, Stack& stack) { return nullptr; }
    virtual void foo() { std::cout << "object class" << std::endl; }
    
    virtual Object* carValue()  { std::cout << "chyba, neni car" << std::endl; }
    virtual Object* cdrValue()  { std::cout << "chyba, neni cdr" << std::endl; }
    virtual int intValue()            { std::cout << "chyba, neni int" << std::endl; }
    virtual std::string stringValue() { std::cout << "chyba, neni string" << std::endl; }
    virtual ObjectFunction functionValue() { std::cout << "chyba, neni funkce" << std::endl; }
    virtual std::string symbolValue() { std::cout << "chyba, neni symbol" << std::endl; }
    virtual bool boolValue() { std::cout << "chyba, neni bool" << std::endl; }
    tagEnum tag;
protected:
    
};

class ObjectInt : public Object {
public:
    ObjectInt( int x ) : Object( TAG_INT ), intVal( x ) {}
    virtual void print() override { std::cout << intVal; }
    virtual int intValue() override { return intVal; }
    virtual Object* eval(Environment& env, Stack& stack) override { return this; }
    
    virtual void foo() override { std::cout << "object int class" << std::endl; }

private:
    int intVal;
};

class ObjectBultInSyntax : public Object {
public:
    ObjectBultInSyntax( ) : Object( TAG_BUILTINSYNTAX ) {}
    virtual void print() override { std::cout << c; }
private:
    char c;
};

class ObjectBuiltInFunction : public Object {
public:
    ObjectBuiltInFunction( ObjectFunction f ) : Object( TAG_BUILTINFUNCTION ) {
        functionCode = f;
    }
    virtual ObjectFunction functionValue() override { return functionCode; }

    virtual void print() override {}
private:
    ObjectFunction functionCode;
};

class ObjectSymbol : public Object {
public:
    ObjectSymbol( const char* a ) : Object( TAG_SYMBOL ) {
        s = a;
    }
    virtual Object* eval(Environment& env, Stack& stack);
    virtual std::string stringValue() override { return s; }
    virtual void print() override { std::cout << s; }
private:
    std::string s;
};

class ObjectString : public Object {
public:
    ObjectString( const char* a ) : Object( TAG_STRING ) {
        s = a;
    }
    virtual std::string stringValue() override { return s; }
    virtual void print() override { std::cout << s; }
private:
    std::string s;
};

class ObjectCons : public Object {
public:
    ObjectCons( Object* ca, Object* cd ) : Object( TAG_CONS ) {
        car = ca;
        cdr = cd;
    }
    virtual Object* carValue() override { return car; }
    virtual Object* cdrValue() override { return cdr; }
    virtual Object* eval(Environment& env, Stack& stack) override;
private:
    Object* car;
    Object* cdr;
};

class ObjectTrue : public Object {
public:
    ObjectTrue() : Object( TAG_TRUE ) {}
    bool boolValue() override { return true; }
};

class ObjectFalse : public Object {
public:
    ObjectFalse() : Object( TAG_FALSE ) {}
    bool boolValue() override { return false; }
};

class ObjectNil : public Object {
public:
    ObjectNil() : Object( TAG_NIL ) {}

};

#endif /* OBJECT_H */
