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

list<Object*> Stack::pop(unsigned int amount) {
   list<Object*> popped;
   for (unsigned int i = 0; i < amount; i++) {
      popped.push_front(this->pop());
   }
   return popped;
}

Object * Stack::peek() const {
   return inputStack.top();
}
int Stack::size() const {
	return inputStack.size();
}