#pragma once

#include <iostream>
#include "LinkedList.h"
using namespace std;

/*
FIFO Queue implementation using Linked List
Used for Submission Processing Queue (Feature 4)
*/

template <typename T>
class Queue
{
    private:
        LinkedList<T> list;

    public:
        Queue() {}

        // Add an element at the rear
        void enqueue(T value)
        {
            list.insertAtEnd(value);
        }

        // Remove the front element (FIFO)
        T dequeue()
        {
            if (list.isEmpty())
                return T();

            T value = list.getAt(0);
            list.deleteFromBeginning();
            return value;
        }

        // Get the front element without removing it
        T front() const
        {
            if (list.isEmpty())
                return T();

            return list.getAt(0);
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
