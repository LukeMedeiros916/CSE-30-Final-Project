#ifndef STACK_H
#define STACK_H

#include <LinkedList.h>
#include <stdexcept>

template <class T>
class Stack {
    LinkedList<T> list;
    
public:
    void push(T value) {
        list.prepend(value);
    }

    T pop() {
        if (list.size() == 0) throw std::logic_error("Stack is empty");
        return list.removeFirst();
    }

    T peek() {
        return list[0];
    }

    bool isEmpty() {
        return list.size() == 0;
    }

    friend struct Graph;
};
// Test
#endif