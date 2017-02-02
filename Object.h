#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <stack>

class Environment;

enum tagEnum {
    TAG_INT,
    TAG_FLOAT,
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
    virtual Object* eval(Environment& env) { return nullptr; }
    
    
    virtual int intValue()            { std::cout << "chyba, neni int" << std::endl; return -1; }
    virtual std::string stringValue() { std::cout << "chyba, neni string" << std::endl; return "ERROR"; }
    tagEnum tag;
protected:
    
};

typedef Object* (*ObjectFunction)();

class ObjectInt : public Object {
public:
    ObjectInt( int x ) : Object( TAG_INT ), intVal( x ) {}
    virtual void print() override { std::cout << intVal; }
    virtual int intValue() override { return intVal; }

private:
    int intVal;
};

class ObjectBultInSyntax : public Object {
public:
    ObjectBultInSyntax( ) : Object() {}
    virtual void print() override { std::cout << c; }
private:
    char c;
};

class ObjectBuiltInFunction : public Object {
public:
    ObjectBuiltInFunction( ObjectFunction f ) : Object( TAG_BUILTINFUNCTION ), function( f ) {}
    virtual void print() override {}
private:
    ObjectFunction function;
};

class ObjectSymbol : public Object {
public:
    ObjectSymbol( const char* a ) : Object( TAG_SYMBOL ) {
        s = a;
    }
    virtual Object* eval(Environment& env);
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
    virtual Object* eval(Environment& env) override;
private:
    Object* car;
    Object* cdr;
};

#endif /* OBJECT_H */

