#include "Stack.h"

using namespace std;

Stack::Stack() { }

Stack::Stack(const Stack& orig) { }

Stack::~Stack() { }

Object * Stack::push(Object * object) {
   stack.push(object);
   return this->peek();
}

Object * Stack::pop() {
   return stack.pop();
}

Object * Stack::peek() {
   return stack.top();
}
