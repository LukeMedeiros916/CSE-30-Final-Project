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
        if (list.size() == 0) throw
    }

    T peek() {
        return LinkedList<T>::operator[](0);
    }

    bool isEmpty() {
        return LinkedList<T>::size() == 0;
    }

    friend struct Graph;
};
#endif