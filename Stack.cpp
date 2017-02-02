#include "Stack.h"
#include "Object.h"

using namespace std;

Stack::Stack() { }

Stack::Stack(const Stack& orig) { }

Stack::~Stack() { }

Object * Stack::push(Object * object) {
   inputStack.push(object);
   return this->peek();
}

Object * Stack::pop() {
   Object * top = inputStack.top();
   inputStack.pop();
   return top;
}

Object * Stack::peek() const {
   return inputStack.top();
}
