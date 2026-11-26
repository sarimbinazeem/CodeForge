#pragma once

#include <iostream>
#include "DynamicArray.h"
using namespace std;

/*
Feature 6: Heap — Live Leaderboard
MaxHeap / PriorityQueue implementation for ranking users by score/verdict
*/

template <typename T>
struct DefaultLess
{
    bool operator()(const T& a, const T& b) const
    {
        return a < b;
    }
};

template <typename T, typename Compare = DefaultLess<T>>
class MaxHeap
{
    private:
        DynamicArray<T> heap;
        Compare comp;

        int parent(int i) const
        {
            return (i - 1) / 2;
        }

        int leftChild(int i) const
        {
            return 2 * i + 1;
        }

        int rightChild(int i) const
        {
            return 2 * i + 2;
        }

        void siftUp(int index)
        {
            while (index > 0 && comp(heap[parent(index)], heap[index]))
            {
                T temp = heap[index];
                heap[index] = heap[parent(index)];
                heap[parent(index)] = temp;

                index = parent(index);
            }
        }

        void siftDown(int index)
        {
            int maxIndex = index;
            int l = leftChild(index);
            int r = rightChild(index);

            if (l < heap.getSize() && comp(heap[maxIndex], heap[l]))
            {
                maxIndex = l;
            }

            if (r < heap.getSize() && comp(heap[maxIndex], heap[r]))
            {
                maxIndex = r;
            }

            if (index != maxIndex)
            {
                T temp = heap[index];
                heap[index] = heap[maxIndex];
                heap[maxIndex] = temp;

                siftDown(maxIndex);
            }
        }

    public:
        MaxHeap() {}

        void push(const T& value)
        {
            heap.pushBack(value);
            siftUp(heap.getSize() - 1);
        }

        T pop()
        {
            if (heap.isEmpty())
            {
                return T();
            }

            T root = heap[0];
            heap[0] = heap[heap.getSize() - 1];
            heap.removeAt(heap.getSize() - 1);

            if (!heap.isEmpty())
            {
                siftDown(0);
            }

            return root;
        }

        T peek() const
        {
            if (heap.isEmpty())
            {
                return T();
            }
            return heap[0];
        }

        int getSize() const
        {
            return heap.getSize();
        }

        bool isEmpty() const
        {
            return heap.isEmpty();
        }

        void clear()
        {
            heap.clear();
        }

        // Retrieve top K items in descending rank without modifying this heap
        DynamicArray<T> getTopK(int k) const
        {
            DynamicArray<T> result;
            if (heap.isEmpty() || k <= 0)
            {
                return result;
            }

            MaxHeap<T, Compare> copyHeap = *this;
            int count = k < copyHeap.getSize() ? k : copyHeap.getSize();

            for (int i = 0; i < count; i++)
            {
                result.pushBack(copyHeap.pop());
            }

            return result;
        }
};
