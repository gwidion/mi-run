#ifndef STACK_H
#define STACK_H

#include <stack>
#include "Object.h"

using namespace std;

class Stack {
public:
   Stack();
   Stack(const Stack& orig);
   virtual ~Stack();
   Object * push(Object * const object);
   Object * pop();
   Object * peek() const;
private:
   stack<Object *> stack;
};

#endif /* STACK_H */

