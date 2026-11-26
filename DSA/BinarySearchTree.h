#pragma once

#include <iostream>
#include "DynamicArray.h"
using namespace std;

/*
Feature 7: Binary Search Tree — Learning Roadmap
Organizes problems by difficulty or ID; supports ordered traversal and range queries
*/

template <typename Key, typename Value>
class BinarySearchTree
{
    private:
        struct Node
        {
            Key key;
            Value value;
            Node* left;
            Node* right;

            Node(Key k, Value v) : key(k), value(v), left(nullptr), right(nullptr) {}
        };

        Node* root;
        int size;

        Node* insertHelper(Node* current, Key key, Value value)
        {
            if (current == nullptr)
            {
                size++;
                return new Node(key, value);
            }

            if (key < current->key)
            {
                current->left = insertHelper(current->left, key, value);
            }
            else if (key > current->key)
            {
                current->right = insertHelper(current->right, key, value);
            }
            else
            {
                // Update existing value
                current->value = value;
            }

            return current;
        }

        bool searchHelper(Node* current, Key key, Value& result) const
        {
            if (current == nullptr)
            {
                return false;
            }

            if (key == current->key)
            {
                result = current->value;
                return true;
            }
            else if (key < current->key)
            {
                return searchHelper(current->left, key, result);
            }
            else
            {
                return searchHelper(current->right, key, result);
            }
        }

        void inOrderHelper(Node* current, DynamicArray<Value>& result) const
        {
            if (current == nullptr) return;

            inOrderHelper(current->left, result);
            result.pushBack(current->value);
            inOrderHelper(current->right, result);
        }

        void rangeQueryHelper(Node* current, Key low, Key high, DynamicArray<Value>& result) const
        {
            if (current == nullptr) return;

            // If current key is greater than low, left subtree might have values
            if (current->key > low)
            {
                rangeQueryHelper(current->left, low, high, result);
            }

            // If current key is in range [low, high]
            if (current->key >= low && current->key <= high)
            {
                result.pushBack(current->value);
            }

            // If current key is less than high, right subtree might have values
            if (current->key < high)
            {
                rangeQueryHelper(current->right, low, high, result);
            }
        }

        void destroyTree(Node* current)
        {
            if (current != nullptr)
            {
                destroyTree(current->left);
                destroyTree(current->right);
                delete current;
            }
        }

        Node* copyTree(Node* otherNode)
        {
            if (otherNode == nullptr) return nullptr;

            Node* newNode = new Node(otherNode->key, otherNode->value);
            newNode->left = copyTree(otherNode->left);
            newNode->right = copyTree(otherNode->right);
            return newNode;
        }

    public:
        BinarySearchTree() : root(nullptr), size(0) {}

        BinarySearchTree(const BinarySearchTree& other)
        {
            root = copyTree(other.root);
            size = other.size;
        }

        BinarySearchTree& operator=(const BinarySearchTree& other)
        {
            if (this != &other)
            {
                clear();
                root = copyTree(other.root);
                size = other.size;
            }
            return *this;
        }

        ~BinarySearchTree()
        {
            clear();
        }

        void insert(Key key, Value value)
        {
            root = insertHelper(root, key, value);
        }

        bool search(Key key, Value& result) const
        {
            return searchHelper(root, key, result);
        }

        // Ordered traversal: delivers sequential learning progression from easiest to hardest
        DynamicArray<Value> inOrderTraversal() const
        {
            DynamicArray<Value> result;
            inOrderHelper(root, result);
            return result;
        }

        // Range query: retrieves all items with keys in [low, high]
        DynamicArray<Value> rangeQuery(Key low, Key high) const
        {
            DynamicArray<Value> result;
            rangeQueryHelper(root, low, high, result);
            return result;
        }

        int getSize() const
        {
            return size;
        }

        bool isEmpty() const
        {
            return root == nullptr;
        }

        void clear()
        {
            destroyTree(root);
            root = nullptr;
            size = 0;
        }
};
