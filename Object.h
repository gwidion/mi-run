#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include "Memory.h"

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
    //    TAG_USERDEFINEDFUNCTION,
};

class Object {
public:
    Object();

    Object(tagEnum t) : tag(t) { }

    tagEnum tag;

    bool freed = false;

    virtual void print() const {
        std::cout << "[an Object]";
    }

    virtual Object* eval(Environment& env) {
        return nullptr;
    }

    virtual Object* carValue() const {
        std::cout << "chyba, neni car" << std::endl;
    }

    virtual Object* cdrValue() const {
        std::cout << "chyba, neni cdr" << std::endl;
    }

    virtual int intValue() const {
        std::cout << "chyba, neni int" << std::endl;
    }

    virtual std::string stringValue() const {
        std::cout << "chyba, neni string" << std::endl;
    }

    virtual ObjectFunction functionValue() const {
        std::cout << "chyba, neni funkce" << std::endl;
    }

    virtual std::string symbolValue() const {
        std::cout << "chyba, neni symbol" << std::endl;
    }

    virtual bool boolValue() const {
        std::cout << "chyba, neni bool" << std::endl;
    }
    virtual unsigned int size() const = 0;

    bool isMarked() const;
    void mark();
    void unMark();

protected:
    bool marked = false;
};

class ObjectInt : public Object {
public:

    ObjectInt(int x) : Object(TAG_INT), intVal(x) { }

    static ObjectInt * allocate(int x);

    virtual void print() const override {
        std::cout << intVal;
    }

    virtual int intValue() const override {
        return intVal;
    }

    virtual Object* eval(Environment& env) override {
        return this;
    }
    virtual unsigned int size() const override;


private:
    int intVal;
};

class ObjectBultInSyntax : public Object {
public:

    ObjectBultInSyntax() : Object(TAG_BUILTINSYNTAX) { }

    static ObjectBultInSyntax * allocate();

    virtual void print() const override {
        std::cout << c;
    }
    virtual unsigned int size() const override;

private:
    char c;
};

class ObjectBuiltInFunction : public Object {
public:

    ObjectBuiltInFunction(ObjectFunction f) : Object(TAG_BUILTINFUNCTION) {
        functionCode = f;
    }

    static ObjectBuiltInFunction * allocate(ObjectFunction f);

    virtual ObjectFunction functionValue() const override {
        return functionCode;
    }

    virtual void print() const override {
        std::cout << "[an ObjectBuiltInFunction]";
    }
    virtual unsigned int size() const override;

private:
    ObjectFunction functionCode;
};

class ObjectSymbol : public Object {
public:

    ObjectSymbol(const char* a) : Object(TAG_SYMBOL) {
        s = a;
    }

    static ObjectSymbol * allocate(const char * a);

    virtual Object* eval(Environment& env);

    virtual std::string stringValue() const override {
        return s;
    }

    virtual void print() const override {
        std::cout << s;
    }
    virtual unsigned int size() const override;

private:
    std::string s;
};

class ObjectString : public Object {
public:

    ObjectString(const char* a) : Object(TAG_STRING) {
        s = a;
    }

    static ObjectString * allocate(const char* a);

    virtual std::string stringValue() const override {
        return s;
    }

    virtual void print() const override {
        std::cout << s;
    }
    virtual unsigned int size() const override;

private:
    std::string s;
};

class ObjectCons : public Object {
public:

    ObjectCons(Object* ca, Object* cd) : Object(TAG_CONS) {
        car = ca;
        cdr = cd;
    }

    static ObjectCons * allocate(Object* ca, Object* cd);

    virtual Object* carValue() const override {
        return car;
    }

    virtual Object* cdrValue() const override {
        return cdr;
    }

    virtual void print() const override {
        std::cout << "[an ObjectCons]";
    }

    virtual Object* eval(Environment& env) override;
    virtual unsigned int size() const override;

private:
    Object* car;
    Object* cdr;
};

class ObjectTrue : public Object {
public:

    ObjectTrue() : Object(TAG_TRUE) { }

    static ObjectTrue * allocate();

    bool boolValue() const override {
        return true;
    }
    virtual unsigned int size() const override;

    void print() const override {
        std::cout << "#t";
    }

};

class ObjectFalse : public Object {
public:

    ObjectFalse() : Object(TAG_FALSE) { }

    static ObjectFalse * allocate();

    bool boolValue() const override {
        return false;
    }
    virtual unsigned int size() const override;

    void print() const override {
        std::cout << "#f";
    }
};

class ObjectNil : public Object {
public:

    ObjectNil() : Object(TAG_NIL) { }

    static ObjectNil * allocate();

    virtual unsigned int size() const override;

    void print() const override {
        std::cout << "nil";
    }
};

class ObjectVoid : public Object {
public:

    ObjectVoid() : Object(TAG_VOID) { }

    static ObjectVoid * allocate();

    virtual unsigned int size() const override;

    virtual void print() const override {
        std::cout << "[a Void (scary, right?)]";
    }
};

#endif /* OBJECT_H */
