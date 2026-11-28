#pragma once

#include <iostream>
using namespace std;

template <typename T>
class DynamicArray
{
    private:
        T* array;
        int size;
        int capacity;

    public:
        DynamicArray()
        {
            size = 0;
            capacity = 2;
            array = new T[capacity];
        }

        DynamicArray(int initialCapacity)
        {
            if (initialCapacity < 1)
            {
                initialCapacity = 1;
            }
            size = 0;
            capacity = initialCapacity;
            array = new T[capacity];
        }

        DynamicArray(const DynamicArray& other)
        {
            size = other.size;
            capacity = other.capacity;
            array = new T[capacity];

            for (int i = 0; i < size; i++)
            {
                array[i] = other.array[i];
            }
        }

        DynamicArray& operator=(const DynamicArray& other)
        {
            if (this != &other)
            {
                T* newArray = new T[other.capacity];

                for (int i = 0; i < other.size; i++)
                {
                    newArray[i] = other.array[i];
                }

                //delte old array
                delete[] array;

                array = newArray;
                size = other.size;
                capacity = other.capacity;
            }

            return *this;
        }

        ~DynamicArray()
        {
            delete[] array;
        }

        void resize()
        {
            int newCapacity = capacity * 2;
            T* newArray = new T[newCapacity];

            for (int i = 0; i < size; i++)
            {
                newArray[i] = array[i];
            }
            delete[] array;
            array = newArray;
            capacity = newCapacity;
        }

        // Add element at the end
        void pushBack(T value)
        {
            if (size == capacity)
            {
                resize();
            }

            array[size] = value;
            size++;
        }

        // Insert element at a specific index
        bool insertAt(int index, T value)
        {
            if (index < 0 || index > size)
                return false;

            if (size == capacity)
            {
                resize();
            }

            for (int i = size; i > index; i--)
            {
                array[i] = array[i - 1];
            }

            array[index] = value;
            size++;

            return true;
        }

        // Remove element at a specific index
        bool removeAt(int index)
        {
            if (index < 0 || index >= size)
                return false;

            for (int i = index; i < size - 1; i++)
            {
                array[i] = array[i + 1];
            }

            size--;
            array[size] = T();

            return true;
        }

        // Get element at an index (const)
        T getAt(int index) const
        {
            if (index < 0 || index >= size)
            {
                return T();
            }

            return array[index];
        }

        // Reference access
        T& operator[](int index)
        {
            return array[index];
        }

        const T& operator[](int index) const
        {
            return array[index];
        }

        // Update element at an index
        bool setAt(int index, T value)
        {
            if (index < 0 || index >= size)
            {
                return false;
            }

            array[index] = value;
            return true;
        }

        // Search for an element
        int search(T value) const
        {
            for (int i = 0; i < size; i++)
            {
                if (array[i] == value)
                {
                    return i;
                }
            }

            return -1;
        }

        // Display elements
        void display() const
        {
            for (int i = 0; i < size; i++)
            {
                cout << array[i] << " ";
            }

            cout << endl;
        }

        int getSize() const
        {
            return size;
        }

        int getCapacity() const
        {
            return capacity;
        }

        bool isEmpty() const
        {
            return size == 0;
        }

        // Remove all elements
        void clear()
        {
            for (int i = 0; i < size; i++)
            {
                array[i] = T();
            }

            size = 0;
        }
};
