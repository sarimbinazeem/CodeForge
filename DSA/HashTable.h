#pragma once

#include <iostream>
#include <string>
#include "LinkedList.h"
#include "DynamicArray.h"
using namespace std;

// Hash helper functions for int and string keys
template <typename Key>
struct HashHelper
{
    static int hash(const Key& key, int cap)
    {
        long long k = (long long)key;
        int idx = (int)(k % cap);
        if (idx < 0) idx += cap;
        return idx;
    }
};

template <>
struct HashHelper<string>
{
    static int hash(const string& key, int cap)
    {
        unsigned long long h = 5381;
        for (char c : key)
        {
            h = ((h << 5) + h) + (unsigned char)c; // djb2 hash
        }
        int idx = (int)(h % (unsigned long long)cap);
        if (idx < 0) idx += cap;
        return idx;
    }
};

/*
 Hash Table — Instant Problem Lookup
Supports O(1) average lookup by ID, title, and tags
*/

template <typename K, typename V>
class HashTable
{
    private:
        struct Entry
        {
            K key;
            V value;

            Entry() : key(K()), value(V()) {}
            Entry(K k, V v) : key(k), value(v) {}

            bool operator==(const Entry& other) const
            {
                return key == other.key;
            }
        };

        LinkedList<Entry>* table;
        int capacity;
        int size;

        int hashFunction(const K& key) const
        {
            return HashHelper<K>::hash(key, capacity);
        }

        void resize()
        {
            int newCap = capacity * 2;
            LinkedList<Entry>* newTable = new LinkedList<Entry>[newCap];

            for (int i = 0; i < capacity; i++)
            {
                for (int j = 0; j < table[i].getSize(); j++)
                {
                    Entry entry = table[i].getAt(j);
                    int newIdx = HashHelper<K>::hash(entry.key, newCap);
                    newTable[newIdx].insertAtEnd(entry);
                }
            }

            delete[] table;
            table = newTable;
            capacity = newCap;
        }

    public:
        HashTable(int initialCapacity = 16)
        {
            capacity = initialCapacity > 0 ? initialCapacity : 16;
            size = 0;
            table = new LinkedList<Entry>[capacity];
        }

        HashTable(const HashTable& other)
        {
            capacity = other.capacity;
            size = other.size;
            table = new LinkedList<Entry>[capacity];

            for (int i = 0; i < capacity; i++)
            {
                table[i] = other.table[i];
            }
        }

        HashTable& operator=(const HashTable& other)
        {
            if (this != &other)
            {
                delete[] table;
                capacity = other.capacity;
                size = other.size;
                table = new LinkedList<Entry>[capacity];

                for (int i = 0; i < capacity; i++)
                {
                    table[i] = other.table[i];
                }
            }
            return *this;
        }

        ~HashTable()
        {
            delete[] table;
        }

        void insert(const K& key, const V& value)
        {
            int index = hashFunction(key);

            // Update if key already exists
            for (int i = 0; i < table[index].getSize(); i++)
            {
                Entry entry = table[index].getAt(i);
                if (entry.key == key)
                {
                    table[index].deleteAt(i);
                    table[index].insertAt(i, Entry(key, value));
                    return;
                }
            }

            // Insert new entry
            table[index].insertAtEnd(Entry(key, value));
            size++;

            if ((double)size / capacity > 0.75)
            {
                resize();
            }
        }

        bool search(const K& key, V& result) const
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

        bool contains(const K& key) const
        {
            V dummy;
            return search(key, dummy);
        }

        bool remove(const K& key)
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

        DynamicArray<K> getAllKeys() const
        {
            DynamicArray<K> keys;
            for (int i = 0; i < capacity; i++)
            {
                for (int j = 0; j < table[i].getSize(); j++)
                {
                    keys.pushBack(table[i].getAt(j).key);
                }
            }
            return keys;
        }

        DynamicArray<V> getAllValues() const
        {
            DynamicArray<V> values;
            for (int i = 0; i < capacity; i++)
            {
                for (int j = 0; j < table[i].getSize(); j++)
                {
                    values.pushBack(table[i].getAt(j).value);
                }
            }
            return values;
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
            capacity = 16;
            size = 0;
            table = new LinkedList<Entry>[capacity];
        }
};
