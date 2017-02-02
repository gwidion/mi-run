#ifndef STACK_H
#define STACK_H

#include <stack>

class Object;

class Stack {
public:
   Stack();
   Stack(const Stack& orig);
   virtual ~Stack();
   Object * push(Object * object);
   Object * pop();
   Object * peek() const;
private:
   std::stack<Object *> inputStack;
};

#endif /* STACK_H */

