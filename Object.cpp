#include "Object.h"
#include "Environment.h"
#include "Stack.h"

extern Memory memory;

using namespace std;

void Object::memoryPrint() const {
	#ifdef DEBUG
    cout << "DEBUG: allocated ";
    this->typePrint();
    cout << " from " << reinterpret_cast<const void*> (this) << " to " << reinterpret_cast<const void*> (reinterpret_cast<const char *> (this) + this->size()) << " ( " << this->size() << " B )" << endl;
	#endif
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

ObjectInt * ObjectInt::allocate( int x ) {
    ObjectInt * object = new (memory.allocate(sizeof (ObjectInt))) ObjectInt(x);
    object->memoryPrint();
    return object;
}

unsigned int ObjectBuiltInSyntax::size() const {
    return sizeof (ObjectBuiltInSyntax);
}

ObjectBuiltInSyntax * ObjectBuiltInSyntax::allocate( ObjectFunction f ) {
   ObjectBuiltInSyntax * object = new (memory.allocate( sizeof (ObjectBuiltInSyntax ) ) ) ObjectBuiltInSyntax( f );
   object->memoryPrint();
   return object;
}

unsigned int ObjectBuiltInFunction::size() const {
    return sizeof (ObjectBuiltInFunction);
}

ObjectBuiltInFunction * ObjectBuiltInFunction::allocate( ObjectFunction f ) {
    ObjectBuiltInFunction * object = new (memory.allocate(sizeof (ObjectBuiltInFunction))) ObjectBuiltInFunction(f);
    object->memoryPrint();
    return object;
}

unsigned int ObjectSymbol::size() const {
    return sizeof (ObjectSymbol);
}

ObjectSymbol * ObjectSymbol::allocate( const char * a ) {
    ObjectSymbol * object = new (memory.allocate(sizeof (ObjectSymbol))) ObjectSymbol(a);
    object->memoryPrint();
    return object;
}

unsigned int ObjectString::size() const {
    return sizeof (ObjectString);
}

ObjectString * ObjectString::allocate( const char* a ) {
    ObjectString * object = new (memory.allocate(sizeof (ObjectString))) ObjectString(a);
    object->memoryPrint();
    return object;
}

Object* ObjectCons::eval(Environment& environment) {
   Object* func = car->eval( environment );
   if( func && func->tag == TAG_BUILTINFUNCTION ) {
      int args = 0;
      Object* restArgs = cdr;
      // push on stack evaluated argument
      while (restArgs->isNotNil()) {
         Object* unevaluated = restArgs->getCar( );
         Object* evaluated = unevaluated->eval( environment );
         memory.stack.push( evaluated );
         restArgs = restArgs->getCdr( );
         args++;
      }
      // run function code
      return func->getFunction( )( args, environment );
   }
   if( func && func->tag == TAG_BUILTINSYNTAX ) {
      int args = 0;
      Object* restArgs = cdr;
      // push on stack unevaluated argument
      while (restArgs->isNotNil()) {
         Object* unevaluated = restArgs->getCar( );
         memory.stack.push( unevaluated );
         restArgs = restArgs->getCdr( );
         args++;
      }
      // run function code
      return func->getFunction( )( args, environment );
   }
   else if( func && func->tag == TAG_USERDEFINEDFUNCTION ) {
      Object* formalArgs = func->getArgList( );
      Object* bodyList = func->getBodyList( );
      Environment newEnvironment( environment );

      // setup new environment
      Object* restFormalArgs = formalArgs;
      Object* restUnevaluatedArgs = cdr;
      while (restFormalArgs->isNotNil()) {
         // get formal argument and its passed value
         Object* theFormalArg = restFormalArgs->getCar( );
         Object* unevaluatedArg = restUnevaluatedArgs->getCar( );
         Object* argVal = unevaluatedArg->eval( environment );
         
         // bind it in new environment
         newEnvironment.addObject( theFormalArg->getString( ), argVal );

         // get next parameter
         restFormalArgs = restFormalArgs->getCdr( );
         restUnevaluatedArgs = restUnevaluatedArgs->getCdr( );
      }

      // then evaluate body in the new environment
      return bodyList->eval( newEnvironment );
   }
   else {
      return this;
   }
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

int ObjectCons::getInt(Environment & environment) {
    return this->eval(environment)->getInt(environment);
}

unsigned int ObjectTrue::size() const {
    return sizeof (ObjectTrue);
}

ObjectTrue * ObjectTrue::allocate( ) {
    ObjectTrue * object = new (memory.allocate(sizeof (ObjectTrue))) ObjectTrue();
    object->memoryPrint();
    return object;
}

unsigned int ObjectFalse::size() const {
    return sizeof (ObjectFalse);
}

ObjectFalse * ObjectFalse::allocate( ) {
    ObjectFalse * object = new (memory.allocate(sizeof (ObjectFalse))) ObjectFalse();
    object->memoryPrint();
    return object;
}

unsigned int ObjectNil::size() const {
    return sizeof (ObjectNil);
}

ObjectNil * ObjectNil::allocate( ) {
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

ObjectVoid * ObjectVoid::allocate( ) {
    ObjectVoid * object = new (memory.allocate(sizeof (ObjectVoid))) ObjectVoid();
    object->memoryPrint();
    return object;
}

unsigned int ObjectUserDefinedFunction::size( ) const {
   return sizeof (ObjectUserDefinedFunction);
}

ObjectUserDefinedFunction* ObjectUserDefinedFunction::allocate( Object* argList, Object* bodyList ) {
   return new (memory.allocate( sizeof (ObjectUserDefinedFunction) ) ) ObjectUserDefinedFunction( argList, bodyList );
}

void ObjectUserDefinedFunction::mark() {
    Object::mark();
    argList->mark();
    bodyList->mark();
}