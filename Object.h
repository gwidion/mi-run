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

   virtual void print() {
      std::cout << "";
   }

   virtual Object* eval(Environment& env, Stack& stack) {
      return nullptr;
   }

   virtual Object* carValue() {
      std::cout << "chyba, neni car" << std::endl;
   }

   virtual Object* cdrValue() {
      std::cout << "chyba, neni cdr" << std::endl;
   }

   virtual int intValue() {
      std::cout << "chyba, neni int" << std::endl;
   }

   virtual std::string stringValue() {
      std::cout << "chyba, neni string" << std::endl;
   }

   virtual ObjectFunction functionValue() {
      std::cout << "chyba, neni funkce" << std::endl;
   }

   virtual std::string symbolValue() {
      std::cout << "chyba, neni symbol" << std::endl;
   }

   virtual bool boolValue() {
      std::cout << "chyba, neni bool" << std::endl;
   }
   virtual unsigned int size() = 0;
   tagEnum tag;
protected:

};

class ObjectInt : public Object {
public:

   ObjectInt(int x) : Object(TAG_INT), intVal(x) { }
   
   static ObjectInt * allocate(int x);

   virtual void print() override {
      std::cout << intVal;
   }

   virtual int intValue() override {
      return intVal;
   }

   virtual Object* eval(Environment& env, Stack& stack) override {
      return this;
   }
   virtual unsigned int size() override;


private:
   int intVal;
};

class ObjectBultInSyntax : public Object {
public:

   ObjectBultInSyntax() : Object(TAG_BUILTINSYNTAX) { }
   
   static ObjectBultInSyntax * allocate();

   virtual void print() override {
      std::cout << c;
   }
   virtual unsigned int size() override;

private:
   char c;
};

class ObjectBuiltInFunction : public Object {
public:

   ObjectBuiltInFunction(ObjectFunction f) : Object(TAG_BUILTINFUNCTION) {
      functionCode = f;
   }
   
   static ObjectBuiltInFunction * allocate(ObjectFunction f);

   virtual ObjectFunction functionValue() override {
      return functionCode;
   }

   virtual void print() override { }
   virtual unsigned int size() override;

private:
   ObjectFunction functionCode;
};

class ObjectSymbol : public Object {
public:

   ObjectSymbol(const char* a) : Object(TAG_SYMBOL) {
      s = a;
   }
   
   static ObjectSymbol * allocate(const char * a);
   
   virtual Object* eval(Environment& env, Stack& stack);

   virtual std::string stringValue() override {
      return s;
   }

   virtual void print() override {
      std::cout << s;
   }
   virtual unsigned int size() override;

private:
   std::string s;
};

class ObjectString : public Object {
public:

   ObjectString(const char* a) : Object(TAG_STRING) {
      s = a;
   }
   
   static ObjectString * allocate(const char* a);

   virtual std::string stringValue() override {
      return s;
   }

   virtual void print() override {
      std::cout << s;
   }
   virtual unsigned int size() override;

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
   
   virtual Object* carValue() override {
      return car;
   }

   virtual Object* cdrValue() override {
      return cdr;
   }
   virtual Object* eval(Environment& env, Stack& stack) override;
   virtual unsigned int size() override;

private:
   Object* car;
   Object* cdr;
};

class ObjectTrue : public Object {
public:

   ObjectTrue() : Object(TAG_TRUE) { }
   
   static ObjectTrue * allocate();

   bool boolValue() override {
      return true;
   }
   virtual unsigned int size() override;

   void print() override {
      std::cout << "#t";
   }

};

class ObjectFalse : public Object {
public:

   ObjectFalse() : Object(TAG_FALSE) { }

   static ObjectFalse * allocate();

   bool boolValue() override {
      return false;
   }
   virtual unsigned int size() override;

   void print() override {
      std::cout << "#f";
   }
};

class ObjectNil : public Object {
public:

   ObjectNil() : Object(TAG_NIL) { }

   static ObjectNil * allocate();
   
   virtual unsigned int size() override;

   void print() override {
      std::cout << "nil";
   }
};

class ObjectVoid : public Object {
public:

   ObjectVoid() : Object(TAG_VOID) { }

   static ObjectVoid * allocate();
   
   virtual unsigned int size() override;

};

#endif /* OBJECT_H */
