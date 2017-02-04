#include "Object.h"
#include "Environment.h"
#include "Stack.h"

extern Memory memory;

Object* ObjectCons::eval(Environment& env) {
   Object* func = car->eval(env);
   if (func->tag == TAG_BUILTINFUNCTION) {
      int initStackSize = memory.stack.size();
      Object* restArgs = cdr;
      while (restArgs) {
         Object* unevaluated = restArgs->carValue();
         //			unevaluated->foo();
         //			Object* evaluated = unevaluated->eval( env, stack );
         memory.stack.push(unevaluated);
         restArgs = restArgs->cdrValue();
      }
      int args = memory.stack.size() - initStackSize;
      return func->functionValue()(args, memory.stack);
   } else {
      return nullptr;
   }
}

Object* ObjectSymbol::eval(Environment& env) {
   return env.getObject(s);
}

unsigned int Object::size() const {
   throw "Object should be abstract";
   return sizeof (Object);
}

unsigned int ObjectInt::size() const {
   return sizeof (ObjectInt);
}

ObjectInt * ObjectInt::allocate(int x) {
   return new (memory.allocate(sizeof (ObjectInt))) ObjectInt(x);
}

unsigned int ObjectBultInSyntax::size() const {
   return sizeof (ObjectBultInSyntax);
}

ObjectBultInSyntax * ObjectBultInSyntax::allocate() {
   return new (memory.allocate(sizeof (ObjectBultInSyntax))) ObjectBultInSyntax();
}

unsigned int ObjectBuiltInFunction::size() const {
   return sizeof (ObjectBuiltInFunction);
}

ObjectBuiltInFunction * ObjectBuiltInFunction::allocate(ObjectFunction f) {
   return new (memory.allocate(sizeof (ObjectBuiltInFunction))) ObjectBuiltInFunction(f);
}

unsigned int ObjectSymbol::size() const {
   return sizeof (ObjectSymbol);
}

ObjectSymbol * ObjectSymbol::allocate(const char * a) {
   return new (memory.allocate(sizeof (ObjectSymbol))) ObjectSymbol(a);
}

unsigned int ObjectString::size() const {
   return sizeof (ObjectString);
}

ObjectString * ObjectString::allocate(const char* a) {
   return new (memory.allocate(sizeof (ObjectString))) ObjectString(a);
}

unsigned int ObjectCons::size() const {
   return sizeof (ObjectCons);
}

ObjectCons * ObjectCons::allocate(Object* ca, Object* cd) {
   return new (memory.allocate(sizeof (ObjectCons))) ObjectCons(ca, cd);
}

unsigned int ObjectTrue::size() const {
   return sizeof (ObjectTrue);
}

ObjectTrue * ObjectTrue::allocate() {
   return new (memory.allocate(sizeof (ObjectTrue))) ObjectTrue();
}

unsigned int ObjectFalse::size() const {
   return sizeof (ObjectFalse);
}

ObjectFalse * ObjectFalse::allocate() {
   return new (memory.allocate(sizeof (ObjectFalse))) ObjectFalse();
}

unsigned int ObjectNil::size() const {
   return sizeof (ObjectNil);
}

ObjectNil * ObjectNil::allocate() {
   return new (memory.allocate(sizeof (ObjectNil))) ObjectNil();
}

unsigned int ObjectVoid::size() const {
   return sizeof (ObjectVoid);
}

ObjectVoid * ObjectVoid::allocate() {
   return new (memory.allocate(sizeof (ObjectVoid))) ObjectVoid();
}
