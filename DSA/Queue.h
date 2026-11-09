#pragma once

#include <iostream>
#include "LinkedList.h"
using namespace std;

template <typename T>
class Queue
{
    private:
        LinkedList<T> list;

    public:

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
