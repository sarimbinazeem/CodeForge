
#pragma once

#include <iostream>
#include "LinkedList.h"
using namespace std;

/*
Applying Stack through Linked List
*/

template <typename T>
class Stack
{
private:
    LinkedList<T> list;

public:

    // Push an element onto the stack
    void push(T value)
    {
        list.insertAtBeginning(value);
    }

    // Remove the top element  (Last in first out)
    T pop()
    {
        if (list.isEmpty())
            return T();

        T value = list.getAt(0);

        list.deleteFromBeginning();

        return value;
    }

    // Get the top element without removing it
    T top() const
    {
        if (list.isEmpty())
            return T();

        T value = list.getAt(0);

        return value;
    }


    bool isEmpty() const
    {
        return list.isEmpty();
    }

    int getSize() const
    {
        return list.getSize();
    }

    void display() const
    {
        list.display();
    }

    void clear()
    {
        list.clear();
    }
};
