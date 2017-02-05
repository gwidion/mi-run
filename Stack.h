#ifndef STACK_H
#define STACK_H

#include <vector>
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
    Object * top() const;
    std::list<Object*> pop(unsigned int amount);
    int size() const;
    void mark();

    auto begin() const;
    auto begin();
    auto end() const;
    auto end();

private:
    std::vector<Object *> data;
};

#endif /* STACK_H */

