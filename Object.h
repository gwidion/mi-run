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
   TAG_USERDEFINEDFUNCTION,
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

    virtual Object* getCar() const {
        throw std::runtime_error("chyba, neni car");
    }

    virtual Object* getCdr() const {
        throw std::runtime_error("chyba, neni cdr");
    }

    virtual int getInt(Environment & environment) {
        throw std::runtime_error("chyba, neni int");
    }

    virtual std::string getString() const {
        throw std::runtime_error("chyba, neni string");
    }

    virtual ObjectFunction getFunction() const {
        throw std::runtime_error("chyba, neni funkce");
    }

    virtual std::string getSymbol() const {
        throw std::runtime_error("chyba, neni symbol");
    }

    virtual bool getBool() const {
        throw std::runtime_error("chyba, neni bool");
    }
    virtual Object* getArgList() const {
        throw std::runtime_error("chyba, neni user defined funkce");
    }
    
    virtual Object* getBodyList() const {
        throw std::runtime_error("chyba, neni user defined funkce");
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
   ObjectInt(int x) : Object(TAG_INT), intVal(x) {}
   static ObjectInt * allocate(int x);

    virtual void print() const override {
        std::cout << intVal;
    }

    virtual void typePrint() const override {
        std::cout << "[an Integer] ";
        this->print();
    }

    virtual int getInt(Environment & environment) override {
        return intVal;
    }

    virtual Object* eval(Environment& env) override {
        return this;
    }
    virtual unsigned int size() const override;

private:
   int intVal;
};

class ObjectBuiltInSyntax : public Object {
public:

   ObjectBuiltInSyntax(ObjectFunction f) : Object(TAG_BUILTINSYNTAX) { 
      functionCode = f; 
   }
   
   static ObjectBuiltInSyntax * allocate(ObjectFunction f);
   virtual ObjectFunction getFunction() const override { return functionCode; }

    virtual void typePrint() const override {
        std::cout << "[an ObjectBuiltInSyntax] ";
        this->print();
    }

    virtual unsigned int size() const override;

private:
   ObjectFunction functionCode;
};

class ObjectBuiltInFunction : public Object {
public:

    ObjectBuiltInFunction(ObjectFunction f) : Object(TAG_BUILTINFUNCTION) {
        functionCode = f;
    }

   static ObjectBuiltInFunction * allocate(ObjectFunction f);

    virtual ObjectFunction getFunction() const override {
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

    virtual std::string getString() const override {
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

    virtual std::string getString() const override {
        return s;
    }

    virtual void print() const override {
        std::cout << s;
    }

   virtual Object* eval(Environment& env) override { 
      return this; 
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

    virtual Object* getCar() const override {
        return car;
    }

    virtual Object* getCdr() const override {
        return cdr;
    }

    virtual void typePrint() const override {
        std::cout << "[a Const] ";
        this->print();
    }

    virtual Object* eval(Environment& env) override;
    virtual unsigned int size() const override;
    virtual void mark() override;
    virtual int getInt(Environment & environment) override;

private:
   Object* car;
   Object* cdr;
};

class ObjectTrue : public Object {
public:
   ObjectTrue() : Object(TAG_TRUE) {}
   static ObjectTrue * allocate();

    bool getBool() const override {
        return true;
    }
    virtual unsigned int size() const override;
   virtual Object* eval(Environment& env) override { 
      return this; 
   }
   
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
   ObjectFalse() : Object(TAG_FALSE) {}
   static ObjectFalse * allocate();

    bool getBool() const override {
        return false;
    }
    virtual unsigned int size() const override;

   virtual Object* eval(Environment& env) override { 
      return this; 
   }

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
   ObjectNil() : Object(TAG_NIL) {}
   static ObjectNil * allocate();
    virtual unsigned int size() const override;

   virtual Object* eval(Environment& env) override { 
      return this; 
   }
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
   ObjectVoid() : Object(TAG_VOID) {}
   static ObjectVoid * allocate();
   virtual unsigned int size() const override;
   virtual Object* eval(Environment& env) override { return this; }
};

class ObjectUserDefinedFunction : public Object {
public:
   ObjectUserDefinedFunction(Object* argL, Object* bodyL) : Object(TAG_USERDEFINEDFUNCTION) {
      argList = argL;
      bodyList = bodyL;
   }
   static ObjectUserDefinedFunction * allocate(Object* argList, Object* bodyList);
   virtual Object* getArgList() const override { 
      return argList; 
   }
   virtual Object* getBodyList() const override { 
      return bodyList; 
   }
    virtual unsigned int size() const override;

    virtual void typePrint() const override {
        std::cout << "[a Void (scary, right?)]";
    }
private:
   Object* argList;
   Object* bodyList;
};

#endif /* OBJECT_H */
