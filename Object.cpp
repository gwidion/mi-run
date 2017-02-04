#include "Object.h"
#include "Environment.h"
#include "Stack.h"

extern Memory memory;

using namespace std;

void Object::memoryPrint() const {
    cout << "DEBUG: allocated ";
    this->typePrint();
    cout << " from " << reinterpret_cast<uintptr_t> (this) << " to " << reinterpret_cast<uintptr_t> (this) + this->size() << " ( " << this->size() << " B )" << endl;
}

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
    ObjectInt * z = new (y) ObjectInt(x);
    z->memoryPrint();
    return z;
}

unsigned int ObjectBultInSyntax::size() const {
    return sizeof (ObjectBultInSyntax);
}

ObjectBultInSyntax * ObjectBultInSyntax::allocate() {
    Object * y = memory.allocate(sizeof (ObjectBultInSyntax));
    ObjectBultInSyntax * z = new (y) ObjectBultInSyntax();
    z->memoryPrint();
    return z;
}

unsigned int ObjectBuiltInFunction::size() const {
    return sizeof (ObjectBuiltInFunction);
}

ObjectBuiltInFunction * ObjectBuiltInFunction::allocate(ObjectFunction f) {
    Object * y = memory.allocate(sizeof (ObjectBuiltInFunction));
    ObjectBuiltInFunction * z = new (y) ObjectBuiltInFunction(f);
    z->memoryPrint();
    return z;
}

unsigned int ObjectSymbol::size() const {
    return sizeof (ObjectSymbol);
}

ObjectSymbol * ObjectSymbol::allocate(const char * a) {
    Object * y = memory.allocate(sizeof (ObjectSymbol));
    ObjectSymbol * z = new (y) ObjectSymbol(a);
    z->memoryPrint();
    return z;
}

unsigned int ObjectString::size() const {
    return sizeof (ObjectString);
}

ObjectString * ObjectString::allocate(const char* a) {
    Object * y = memory.allocate(sizeof (ObjectString));
    ObjectString * z = new (y) ObjectString(a);
    z->memoryPrint();
    return z;
}

unsigned int ObjectCons::size() const {
    return sizeof (ObjectCons);
}

ObjectCons * ObjectCons::allocate(Object* ca, Object * cd) {
    Object * y = memory.allocate(sizeof (ObjectCons));
    ObjectCons * z = new (y) ObjectCons(ca, cd);
    z->memoryPrint();
    return z;
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
    Object * y = memory.allocate(sizeof (ObjectTrue));
    ObjectTrue * z = new (y) ObjectTrue();
    z->memoryPrint();
    return z;
}

unsigned int ObjectFalse::size() const {
    return sizeof (ObjectFalse);
}

ObjectFalse * ObjectFalse::allocate() {
    Object * y = memory.allocate(sizeof (ObjectFalse));
    ObjectFalse * z = new (y) ObjectFalse();
    z->memoryPrint();
    return z;
}

unsigned int ObjectNil::size() const {
    return sizeof (ObjectNil);
}

ObjectNil * ObjectNil::allocate() {
    Object * y = memory.allocate(sizeof (ObjectNil));
    ObjectNil * z = new (y) ObjectNil();
    z->memoryPrint();
    return z;
}

unsigned int ObjectVoid::size() const {
    return sizeof (ObjectVoid);
}

ObjectVoid * ObjectVoid::allocate() {
    Object * y = memory.allocate(sizeof (ObjectVoid));
    ObjectVoid * z = new (y) ObjectVoid();
    z->memoryPrint();
    return z;
}
