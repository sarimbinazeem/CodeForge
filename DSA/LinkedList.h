
#pragma once

#include <iostream>
using namespace std;

template <typename T>
class LinkedList
{
    private:

        struct Node
        {
            T data;
            Node* next;

            Node(T value)
            {
                data = value;
                next = nullptr;
            }
        };

        Node* head;
        Node* tail;
        int size;

    public:

        // Constructor
        LinkedList()
        {
            head = nullptr;
            tail = nullptr;
            size = 0;
        }

        // Prevent unsafe shallow copying
        LinkedList(const LinkedList&) = delete;

        LinkedList& operator=(const LinkedList&) = delete;

        // Destructor
        ~LinkedList()
        {
            clear();
        }

        void insertAtBeginning(T value)
        {
            Node* newNode = new Node(value);

            if (head == nullptr)
            {
                head = newNode;
                tail = newNode;
            }
            else
            {
                newNode->next = head;
                head = newNode;
            }

            size++;
        }

        void insertAtEnd(T value)
        {
            Node* newNode = new Node(value);

            if (head == nullptr)
            {
                head = newNode;
                tail = newNode;
            }
            else
            {
                tail->next = newNode;
                tail = newNode;
            }

            size++;
        }  
        

        // Insert at a specific index
        bool insertAt(int index, T value)
        {
            if (index < 0 || index > size)
                return false;

            if (index == 0)
            {
                insertAtBeginning(value);
                return true;
            }

            Node* current = head;

            for (int i = 0; i < index - 1; i++)
            {
                current = current->next;
            }

            Node* newNode = new Node(value);

            newNode->next = current->next;
            current->next = newNode;

            size++;

            return true;
        }

        // Delete first node
        bool deleteFromBeginning()
        {
            if (head == nullptr)
                return false;

            Node* temp = head;

            head = head->next;

            delete temp;

            size--;

            if (head == nullptr)
                tail = nullptr;

            return true;
        }

        bool deleteFromEnd()
        {
            if (head == nullptr)
                return false;

            if (head == tail)
            {
                delete head;

                head = nullptr;
                tail = nullptr;
            }
            else
            {
                Node* current = head;

                while (current->next != tail)
                {
                    current = current->next;
                }

                delete tail;

                tail = current;
                tail->next = nullptr;
            }

            size--;

            return true;
        }

    
        bool deleteAt(int index)
        {
            if (index < 0 || index >= size)
                return false;

            if (index == 0)
            {
                return deleteFromBeginning();

            }

            if (index == size - 1)
            {
                return deleteFromEnd();

            }

            Node* current = head;

            for (int i = 0; i < index - 1; i++)
            {
                current = current->next;
            }

            Node* temp = current->next;
            current->next = temp->next;
            delete temp;
            size--;
            return true;
        }

        bool search(T value) const
        {
            Node* current = head;

            while (current != nullptr)
            {
                if (current->data == value)
                    return true;

                current = current->next;
            }

            return false;
        }

        void display() const
        {
            Node* current = head;

            while (current != nullptr)
            {
                cout << current->data << " -> ";

                current = current->next;
            }

            cout << "NULL" << endl;
        }

        // Reverse the linked list
        void reverse()
        {
            Node* previous = nullptr;
            Node* current = head;
            Node* nextNode = nullptr;

            while (current != nullptr)
            {
                nextNode = current->next;

                current->next = previous;

                previous = current;
                current = nextNode;
            }

            head = previous;
        }

        T getAt(int index) const
        {
            if (index < 0 || index >= size)
                return T();

            Node* current = head;

            for (int i = 0; i < index; i++)
            {
                current = current->next;
            }

            return current->data;
        }

        int getSize() const
        {
            return size;
        }

        bool isEmpty() const
        {
            return head == nullptr;
        }

        void clear()
        {
            Node* current = head;

            while (current != nullptr)
            {
                Node* temp = current;

                current = current->next;

                delete temp;
            }

            head = nullptr;
            size = 0;
        }
};
