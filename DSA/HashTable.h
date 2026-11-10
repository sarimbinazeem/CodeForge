#pragma once
#include <iostream>
#include "LinkedList.h"
using namespace std;

template <typename T>
class HashTable{
    private:
        struct Entry{
            int key;
            T value;

            Entry():key(0){
                value = T();
            }
            Entry(int k, T v){
                key = k;
                value = v;
            }      
        };

        LinkedList <Entry>* table;
        int capacity;
        int size;

    public:

        HashTable()
        {
            capacity = 10;
            size = 0;

            table = new LinkedList<Entry>[capacity];
        }

        // Prevent unsafe copying
        HashTable(const HashTable&) = delete;
        HashTable& operator=(const HashTable&) = delete;       
        
        ~HashTable()
        {
            delete[] table;
        }        

        //hash function for index
        int hashFunction(int key) const{
            int index = key % capacity;
            if(index < 0 ){
                index += capacity;
            }
            return index;
        }

        //resizing and rehasing all the entries
        void resize()
        {
            int newCap = capacity *2;
            LinkedList <Entry> * newTable = new LinkedList<Entry>[newCap];

            for(int i = 0 ; i<capacity ; i++)
            {
                for(int j = 0 ; j < table[i].getSize() ; j++){
                    Entry entry = table[i].getAt(j); //get entry of that linked list
                    int newIdx = entry.key % newCap;
                    if(newIdx < 0){
                        newIdx += newCap;
                    }

                    //put that entry at new HASHED index
                    newTable[newIdx].insertAtEnd(entry);
                }
            }
            delete[] table;

            table = newTable;
            capacity = newCap;
        }

        void insert(int key, T value)
        {
            int index = hashFunction(key);

            // Update if the key already exists
            for (int i = 0; i < table[index].getSize(); i++)
            {
                Entry entry = table[index].getAt(i);

                if (entry.key == key)
                {
                    table[index].deleteAt(i);
                    table[index].insertAt(i, Entry(key, value));

                    cout << "Value updated successfully!\n";
                    return;
                }
            }

            // Insert new entry
            table[index].insertAtEnd(Entry(key, value));
            size++;
            // Check load factor
            if ((double)size / capacity > 0.75)
            {
                resize();
            }
        }

        bool search(int key,T& result) const
        {
            int index = hashFunction(key);

            for (int i = 0; i < table[index].getSize(); i++)
            {
                Entry entry = table[index].getAt(i);

                if (entry.key == key)
                {
                    result = entry.value;
                    return true;
                }
            }

            return false;
        }

        // Delete an entry
        bool remove(int key)
        {
            int index = hashFunction(key);

            for (int i = 0; i < table[index].getSize(); i++)
            {
                Entry entry = table[index].getAt(i);

                if (entry.key == key)
                {
                    table[index].deleteAt(i);
                    size--;

                    return true;
                }
            }

            return false;
        }        

        void display() const
        {
            for (int i = 0; i < capacity; i++)
            {
                cout << "[" << i << "] ";

                for (int j = 0; j < table[i].getSize(); j++)
                {
                    Entry entry = table[i].getAt(j);

                    cout << "(" << entry.key << ", " << entry.value << ") -> ";
                }
                cout << "NULL\n";
            }
        }

        bool isEmpty() const
        {
            return size == 0;
        }

        int getSize() const
        {
            return size;
        }

        int getCapacity() const
        {
            return capacity;
        }

        double getLoadFactor() const
        {
            return (double)size / capacity;
        }

        void clear()
        {
            delete[] table;

            capacity = 10;
            size = 0;

            table = new LinkedList<Entry>[capacity];
        }   
};