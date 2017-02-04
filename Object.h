#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include "Memory.h"

class Environment;
class Object;
class Stack;

typedef Object* (*ObjectFunction)(int, Environment&);

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

    virtual void print(std::ostream & output = std::cout) const {
        output << "[an Object]";
    }

    void memoryPrint(std::ostream & output) const;

    virtual Object* eval(Environment& env) {
        return nullptr;
    }

    virtual Object* carValue() const {
        throw std::runtime_error("chyba, neni car");
    }

    virtual Object* cdrValue() const {
        throw std::runtime_error("chyba, neni cdr");
    }

    virtual int intValue() const {
        throw std::runtime_error("chyba, neni int");
    }

    virtual std::string stringValue() const {
        throw std::runtime_error("chyba, neni string");
    }

    virtual ObjectFunction functionValue() const {
        throw std::runtime_error("chyba, neni funkce");
    }

    virtual std::string symbolValue() const {
        throw std::runtime_error("chyba, neni symbol");
    }

    virtual bool boolValue() const {
        throw std::runtime_error("chyba, neni bool");
    }
    virtual unsigned int size() const = 0;

    bool isMarked() const;
    virtual void mark();
    void unMark();

protected:
    bool marked = false;
};

class ObjectInt : public Object {
public:

    ObjectInt(int x) : Object(TAG_INT), intVal(x) { }

    static ObjectInt * allocate(int x);

    virtual void print(std::ostream & output = std::cout) const override {
        output << intVal;
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

    virtual void print(std::ostream & output = std::cout) const override {
        output << c;
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

    virtual void print(std::ostream & output = std::cout) const override {
        output << "[an ObjectBuiltInFunction]";
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

    virtual void print(std::ostream & output = std::cout) const override {
        output << s;
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

    virtual void print(std::ostream & output = std::cout) const override {
        output << s;
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

    Object* setCar(Object* ca) {
        car = ca;
    }

    Object* setCdr(Object* cd) {
        cdr = cd;
    }

    virtual void print(std::ostream & output = std::cout) const override {
        output << "[an ObjectCons]";
    }

    virtual Object* eval(Environment& env) override;
    virtual unsigned int size() const override;
    virtual void mark() override;

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

    void print(std::ostream & output = std::cout) const override {
        output << "#t";
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

    void print(std::ostream & output = std::cout) const override {
        output << "#f";
    }
};

class ObjectNil : public Object {
public:

    ObjectNil() : Object(TAG_NIL) { }

    static ObjectNil * allocate();

    virtual unsigned int size() const override;

    void print(std::ostream & output = std::cout) const override {
        output << "nil";
    }
};

class ObjectVoid : public Object {
public:

    ObjectVoid() : Object(TAG_VOID) { }

    static ObjectVoid * allocate();

    virtual unsigned int size() const override;

    virtual void print(std::ostream & output = std::cout) const override {
        output << "[a Void (scary, right?)]";
    }
};

#endif /* OBJECT_H */
