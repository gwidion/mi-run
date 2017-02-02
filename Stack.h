#ifndef STACK_H
#define STACK_H

#include <stack>

using namespace std;

class Stack {
public:
   Stack();
   Stack(const Stack& orig);
   virtual ~Stack();
   Object * push(Object * object);
   Object * pop();
   Object * peek();
private:
   stack<Object *> stack;
};

#endif /* STACK_H */

