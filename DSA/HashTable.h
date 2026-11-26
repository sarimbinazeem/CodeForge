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
            Node* prev;

            Node(T value)
            {
                data = value;
                next = nullptr;
                prev = nullptr;
            }
        };

        Node* head;
        Node* tail;
        int size;

        // Navigation cursor for history browsing
        Node* cursor;

    public:
        // Constructor
        LinkedList()
        {
            head = nullptr;
            tail = nullptr;
            size = 0;
            cursor = nullptr;
        }

        // Deep copy constructor
        LinkedList(const LinkedList& other)
        {
            head = nullptr;
            tail = nullptr;
            size = 0;
            cursor = nullptr;

            Node* current = other.head;
            while (current != nullptr)
            {
                insertAtEnd(current->data);
                current = current->next;
            }
        }

        // Deep copy assignment operator
        LinkedList& operator=(const LinkedList& other)
        {
            if (this != &other)
            {
                clear();
                Node* current = other.head;
                while (current != nullptr)
                {
                    insertAtEnd(current->data);
                    current = current->next;
                }
            }
            return *this;
        }

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
                head->prev = newNode;
                head = newNode;
            }

            size++;
            if (cursor == nullptr)
            {
                cursor = head;
            }
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
                newNode->prev = tail;
                tail = newNode;
            }

            size++;
            if (cursor == nullptr)
            {
                cursor = tail;
            }
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

            if (index == size)
            {
                insertAtEnd(value);
                return true;
            }

            Node* current = head;
            for (int i = 0; i < index - 1; i++)
            {
                current = current->next;
            }

            Node* newNode = new Node(value);
            newNode->next = current->next;
            newNode->prev = current;
            if (current->next != nullptr)
            {
                current->next->prev = newNode;
            }
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

            if (head != nullptr)
            {
                head->prev = nullptr;
            }
            else
            {
                tail = nullptr;
            }

            if (cursor == temp)
            {
                cursor = head;
            }

            delete temp;
            size--;
            return true;
        }

        bool deleteFromEnd()
        {
            if (head == nullptr)
                return false;

            if (head == tail)
            {
                if (cursor == head)
                {
                    cursor = nullptr;
                }
                delete head;
                head = nullptr;
                tail = nullptr;
            }
            else
            {
                Node* temp = tail;
                tail = tail->prev;
                tail->next = nullptr;

                if (cursor == temp)
                {
                    cursor = tail;
                }

                delete temp;
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
            for (int i = 0; i < index; i++)
            {
                current = current->next;
            }

            if (cursor == current)
            {
                cursor = current->next != nullptr ? current->next : current->prev;
            }

            current->prev->next = current->next;
            current->next->prev = current->prev;

            delete current;
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
            Node* current = head;
            Node* temp = nullptr;

            while (current != nullptr)
            {
                temp = current->prev;
                current->prev = current->next;
                current->next = temp;
                current = current->prev;
            }

            if (temp != nullptr)
            {
                tail = head;
                head = temp->prev;
            }
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
            tail = nullptr;
            cursor = nullptr;
            size = 0;
        }

        // ===== NAVIGATION CURSOR API (Submission History) =====
        void resetCursorToBeginning()
        {
            cursor = head;
        }

        void resetCursorToEnd()
        {
            cursor = tail;
        }

        bool hasCursorCurrent() const
        {
            return cursor != nullptr;
        }

        bool hasCursorNext() const
        {
            return cursor != nullptr && cursor->next != nullptr;
        }

        bool hasCursorPrev() const
        {
            return cursor != nullptr && cursor->prev != nullptr;
        }

        bool moveCursorNext()
        {
            if (hasCursorNext())
            {
                cursor = cursor->next;
                return true;
            }
            return false;
        }

        bool moveCursorPrev()
        {
            if (hasCursorPrev())
            {
                cursor = cursor->prev;
                return true;
            }
            return false;
        }

        T getCursorData() const
        {
            if (cursor != nullptr)
            {
                return cursor->data;
            }
            return T();
        }
};
