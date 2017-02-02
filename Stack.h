#ifndef STACK_H
#define STACK_H

#include <stack>
#include <list>

class Object;

class Stack {
public:
   Stack();
   Stack(const Stack& orig);
   virtual ~Stack();
   Object * push(Object * object);
   Object * pop();
   Object * peek() const;
   std::list<Object*> pop(unsigned int amount);
   int size() const;
   
private:
   std::stack<Object *> inputStack;
};

#endif /* STACK_H */

