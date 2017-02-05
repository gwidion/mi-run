#include "Stack.h"
#include "Object.h"

using namespace std;

Stack::Stack() { }

Stack::Stack(const Stack& orig) { }

Stack::~Stack() { }

Object * Stack::push(Object * object) {
    data.push_back(object);
    return this->peek();
}

Object * Stack::pop() {
    Object * top = this->peek();
    data.pop_back();
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
    return data.back();
}

Object * Stack::top() const {
    return data.back();
}

int Stack::size() const {
    return data.size();
}

void Stack::mark() {
    for (Object * item : data)
        item->mark();
}

auto Stack::begin() const {
    return data.begin();
}

auto Stack::begin() {
    return data.begin();
}

auto Stack::end() const {
    return data.end();
}

auto Stack::end() {
    return data.end();
}