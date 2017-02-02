#include "Stack.h"

using namespace std;

Stack::Stack() { }

Stack::Stack(const Stack& orig) { }

Stack::~Stack() { }

Object * Stack::push(Object * const object) {
   stack.push(object);
   return this->peek();
}

Object * Stack::pop() {
   Object * top = stack.top();
   stack.pop();
   return top;
}

Object * Stack::peek() const {
   return stack.top();
}
