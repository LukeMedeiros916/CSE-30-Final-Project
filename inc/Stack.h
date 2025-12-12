#ifndef STACK_H
#define STACK_H

#include <LinkedList.h>
#include <stdexcept>

template <class T>
class Stack : public LinkedList<T> {
    void append(T value) {}
    void prepend(T value) {}

    T removeFirst() {
        throw std::logic_error("Cannot call removeFirst on Stack");
    }

    T removeLast(){
        throw std::logic_error("Cannot call removeLast on stack");
    }
    
public:
    void push(T value) {
        LinkedList<T>::prepend(value);
    }

    T pop() {
        return LinkedList<T>::removeFirst();
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