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

    virtual void print() const {
        //do nothing
    }

    virtual void typePrint() const {
        std::cout << "[an Object] ";
        this->print();
    }

    void memoryPrint() const;

    virtual Object* eval(Environment& env);

    virtual Object* carValue() const {
        throw std::runtime_error("chyba, neni car");
    }

    virtual Object* cdrValue() const {
        throw std::runtime_error("chyba, neni cdr");
    }

    virtual int intValue(Environment & environment) {
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

    virtual bool isNil() const;
    virtual bool isNotNil() const;

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

    virtual void print() const override {
        std::cout << intVal;
    }

    virtual void typePrint() const override {
        std::cout << "[an Integer] ";
        this->print();
    }

    virtual int intValue(Environment & environment) override {
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

    virtual void typePrint() const override {
        std::cout << "[an ObjectBuiltInSyntax] ";
        this->print();
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

    virtual void typePrint() const override {
        std::cout << "[a BuiltInFunction] ";
        this->print();
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

    virtual void typePrint() const override {
        std::cout << "[a Symbol] ";
        this->print();
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

    virtual void typePrint() const override {
        std::cout << "[a String] ";
        this->print();
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

    //    static ObjectCons * allocate(Object* ca, Object* cd);

    static ObjectCons * fromStack();

    virtual Object* carValue() const override {
        return car;
    }

    virtual Object* cdrValue() const override {
        return cdr;
    }

    virtual void typePrint() const override {
        std::cout << "[a Const] ";
        this->print();
    }

    virtual Object* eval(Environment& env) override;
    virtual unsigned int size() const override;
    virtual void mark() override;
    virtual int intValue(Environment & environment) override;

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

    virtual void typePrint() const override {
        std::cout << "[a Boolean] ";
        this->print();
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

    virtual void typePrint() const override {
        std::cout << "[a Boolean] ";
        this->print();
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

    virtual void typePrint() const override {
        this->print();
    }

    virtual bool isNil() const override;
    virtual bool isNotNil() const override;
};

class ObjectVoid : public Object {
public:

    ObjectVoid() : Object(TAG_VOID) { }

    static ObjectVoid * allocate();

    virtual unsigned int size() const override;

    virtual void typePrint() const override {
        std::cout << "[a Void (scary, right?)]";
    }
};

#endif /* OBJECT_H */
