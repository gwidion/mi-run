#include "Object.h"
#include "Environment.h"
#include "Stack.h"

extern Memory memory;

using namespace std;

unsigned int Object::size() const {
    throw runtime_error("Object should be abstract");
    return sizeof (Object);
}

bool Object::isMarked() const {
    return marked;
}

void Object::mark() {
    marked = true;
}

void Object::unMark() {
    marked = false;
}

Object * ObjectCons::eval(Environment & environment) {
    Object* func = car->eval(environment);
    if (func && func->tag == TAG_BUILTINFUNCTION) {
        int initStackSize = memory.stack.size();
        Object* restArgs = cdr;
        while (restArgs) {
            Object* unevaluated = restArgs->carValue();
            //			unevaluated->foo();
            //			Object* evaluated = unevaluated->eval( env );
            memory.stack.push(unevaluated);
            restArgs = restArgs->cdrValue();
        }
        int args = memory.stack.size() - initStackSize;
        return func->functionValue()(args, environment);
    } else {
        return nullptr;
    }
}

Object * ObjectSymbol::eval(Environment& environment) {
    return environment.getObject(s);
}

unsigned int ObjectInt::size() const {
    return sizeof (ObjectInt);
}

ObjectInt * ObjectInt::allocate(int x) {
    Object * y = memory.allocate(sizeof (ObjectInt));
    return new (y) ObjectInt(x);
}

unsigned int ObjectBultInSyntax::size() const {
    return sizeof (ObjectBultInSyntax);
}

ObjectBultInSyntax * ObjectBultInSyntax::allocate() {
    Object * x = memory.allocate(sizeof (ObjectBultInSyntax));
    return new (x) ObjectBultInSyntax();
}

unsigned int ObjectBuiltInFunction::size() const {
    return sizeof (ObjectBuiltInFunction);
}

ObjectBuiltInFunction * ObjectBuiltInFunction::allocate(ObjectFunction f) {
    Object * x = memory.allocate(sizeof (ObjectBuiltInFunction));
    return new (x) ObjectBuiltInFunction(f);
}

unsigned int ObjectSymbol::size() const {
    return sizeof (ObjectSymbol);
}

ObjectSymbol * ObjectSymbol::allocate(const char * a) {
    Object * x = memory.allocate(sizeof (ObjectSymbol));
    return new (x) ObjectSymbol(a);
}

unsigned int ObjectString::size() const {
    return sizeof (ObjectString);
}

ObjectString * ObjectString::allocate(const char* a) {
    Object * x = memory.allocate(sizeof (ObjectString));
    return new (x) ObjectString(a);
}

unsigned int ObjectCons::size() const {
    return sizeof (ObjectCons);
}

ObjectCons * ObjectCons::allocate(Object* ca, Object * cd) {
    Object * x = memory.allocate(sizeof (ObjectCons));
    return new (x) ObjectCons(ca, cd);
}

void ObjectCons::mark() {
    if (car)
        car->mark();
    if (cdr)
        cdr->mark();
}

unsigned int ObjectTrue::size() const {
    return sizeof (ObjectTrue);
}

ObjectTrue * ObjectTrue::allocate() {
    Object * x = memory.allocate(sizeof (ObjectTrue));
    return new (x) ObjectTrue();
}

unsigned int ObjectFalse::size() const {
    return sizeof (ObjectFalse);
}

ObjectFalse * ObjectFalse::allocate() {
    Object * x = memory.allocate(sizeof (ObjectFalse));
    return new (x) ObjectFalse();
}

unsigned int ObjectNil::size() const {
    return sizeof (ObjectNil);
}

ObjectNil * ObjectNil::allocate() {
    Object * x = memory.allocate(sizeof (ObjectNil));
    return new (x) ObjectNil();
}

unsigned int ObjectVoid::size() const {
    return sizeof (ObjectVoid);
}

ObjectVoid * ObjectVoid::allocate() {
    Object * x = memory.allocate(sizeof (ObjectVoid));
    return new (x) ObjectVoid();
}
