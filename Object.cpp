#include "Object.h"
#include "Environment.h"
#include "Stack.h"

extern Memory memory;

using namespace std;

void Object::memoryPrint() const {
    cout << "DEBUG: allocated ";
    this->typePrint();
    cout << " from " << reinterpret_cast<const void*> (this) << " to " << reinterpret_cast<const void*> (reinterpret_cast<const char *> (this) + this->size()) << " ( " << this->size() << " B )" << endl;
}

Object* Object::eval(Environment& env) {
    return ObjectNil::allocate();
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

bool Object::isNil() const {
    return false;
}

bool Object::isNotNil() const {
    return true;
}

Object * ObjectSymbol::eval(Environment& environment) {
    return environment.getObject(s);
}

unsigned int ObjectInt::size() const {
    return sizeof (ObjectInt);
}

ObjectInt * ObjectInt::allocate(int x) {
    ObjectInt * object = new (memory.allocate(sizeof (ObjectInt))) ObjectInt(x);
    object->memoryPrint();
    return object;
}

unsigned int ObjectBultInSyntax::size() const {
    return sizeof (ObjectBultInSyntax);
}

ObjectBultInSyntax * ObjectBultInSyntax::allocate() {
    ObjectBultInSyntax * object = new (memory.allocate(sizeof (ObjectBultInSyntax))) ObjectBultInSyntax();
    object->memoryPrint();
    return object;
}

unsigned int ObjectBuiltInFunction::size() const {
    return sizeof (ObjectBuiltInFunction);
}

ObjectBuiltInFunction * ObjectBuiltInFunction::allocate(ObjectFunction f) {
    ObjectBuiltInFunction * object = new (memory.allocate(sizeof (ObjectBuiltInFunction))) ObjectBuiltInFunction(f);
    object->memoryPrint();
    return object;
}

unsigned int ObjectSymbol::size() const {
    return sizeof (ObjectSymbol);
}

ObjectSymbol * ObjectSymbol::allocate(const char * a) {
    ObjectSymbol * object = new (memory.allocate(sizeof (ObjectSymbol))) ObjectSymbol(a);
    object->memoryPrint();
    return object;
}

unsigned int ObjectString::size() const {
    return sizeof (ObjectString);
}

ObjectString * ObjectString::allocate(const char* a) {
    ObjectString * object = new (memory.allocate(sizeof (ObjectString))) ObjectString(a);
    object->memoryPrint();
    return object;
}

Object * ObjectCons::eval(Environment & environment) {
    Object* func = car->eval(environment);
    if (func && func->tag == TAG_BUILTINFUNCTION) {
        int initStackSize = memory.stack.size();
        Object* restArgs = cdr;
        while (restArgs->isNotNil()) {
            Object* unevaluated = restArgs->carValue();
            //			unevaluated->foo();
            //			Object* evaluated = unevaluated->eval( env );
            memory.stack.push(unevaluated);
            restArgs = restArgs->cdrValue();
        }
        int args = memory.stack.size() - initStackSize;
        return func->functionValue()(args, environment);
    } else
        return ObjectNil::allocate();
}

unsigned int ObjectCons::size() const {
    return sizeof (ObjectCons);
}

//ObjectCons * ObjectCons::allocate(Object* ca, Object * cd) {
//    ObjectCons * object = new (memory.allocate(sizeof (ObjectCons))) ObjectCons(ca, cd);
//    object->memoryPrint();
//    return object;
//}

ObjectCons * ObjectCons::fromStack() {
    Object * address = memory.allocate(sizeof (ObjectCons));
    Object * cd = memory.stack.pop();
    ObjectCons * object = new (address) ObjectCons(memory.stack.pop(), cd);
    if (object == object->car)
        throw "allocated same for const and car";
    if (object == object->cdr)
        throw "allocated same for const and cdr";
    if (object->car == object->cdr)
        throw "allocated same for car and cdr";
    object->memoryPrint();
    return object;
}

void ObjectCons::mark() {
    Object::mark();
    car->mark();
    cdr->mark();
}

int ObjectCons::intValue(Environment & environment) {
    return this->eval(environment)->intValue(environment);
}

unsigned int ObjectTrue::size() const {
    return sizeof (ObjectTrue);
}

ObjectTrue * ObjectTrue::allocate() {
    ObjectTrue * object = new (memory.allocate(sizeof (ObjectTrue))) ObjectTrue();
    object->memoryPrint();
    return object;
}

unsigned int ObjectFalse::size() const {
    return sizeof (ObjectFalse);
}

ObjectFalse * ObjectFalse::allocate() {
    ObjectFalse * object = new (memory.allocate(sizeof (ObjectFalse))) ObjectFalse();
    object->memoryPrint();
    return object;
}

unsigned int ObjectNil::size() const {
    return sizeof (ObjectNil);
}

ObjectNil * ObjectNil::allocate() {
    ObjectNil * object = new (memory.allocate(sizeof (ObjectNil))) ObjectNil();
    object->memoryPrint();
    return object;
}

bool ObjectNil::isNil() const {
    return true;
}

bool ObjectNil::isNotNil() const {
    return false;
}

unsigned int ObjectVoid::size() const {
    return sizeof (ObjectVoid);
}

ObjectVoid * ObjectVoid::allocate() {
    ObjectVoid * object = new (memory.allocate(sizeof (ObjectVoid))) ObjectVoid();
    object->memoryPrint();
    return object;
}
