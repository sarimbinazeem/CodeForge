#pragma once

#include <iostream>
#include <string>
#include "DynamicArray.h"
#include "Queue.h"
using namespace std;

/*
Feature 8: Graph — Skill Dependency System
Directed Graph modeling prerequisites, unlocks, and curriculum progression
*/

class Graph
{
    private:
        struct EdgeNode
        {
            int toIndex;
            EdgeNode* next;
            EdgeNode(int to) : toIndex(to), next(nullptr) {}
        };

        DynamicArray<string> vertices;
        DynamicArray<EdgeNode*> adjList;    // outgoing: u -> v (u is prerequisite of v)
        DynamicArray<EdgeNode*> inList;     // incoming: v has prerequisite u

        int findVertexIndex(const string& name) const
        {
            for (int i = 0; i < vertices.getSize(); i++)
            {
                if (vertices[i] == name)
                {
                    return i;
                }
            }
            return -1;
        }

    public:
        Graph() {}

        ~Graph()
        {
            clear();
        }

        void clear()
        {
            for (int i = 0; i < adjList.getSize(); i++)
            {
                EdgeNode* cur = adjList[i];
                while (cur != nullptr)
                {
                    EdgeNode* temp = cur;
                    cur = cur->next;
                    delete temp;
                }
            }
            for (int i = 0; i < inList.getSize(); i++)
            {
                EdgeNode* cur = inList[i];
                while (cur != nullptr)
                {
                    EdgeNode* temp = cur;
                    cur = cur->next;
                    delete temp;
                }
            }
            vertices.clear();
            adjList.clear();
            inList.clear();
        }

        int addVertex(const string& name)
        {
            int existing = findVertexIndex(name);
            if (existing != -1) return existing;

            int idx = vertices.getSize();
            vertices.pushBack(name);
            adjList.pushBack(nullptr);
            inList.pushBack(nullptr);
            return idx;
        }

        bool addPrerequisite(const string& prerequisite, const string& dependent)
        {
            int u = addVertex(prerequisite);
            int v = addVertex(dependent);

            // Add u -> v in adjList
            EdgeNode* outEdge = new EdgeNode(v);
            outEdge->next = adjList[u];
            adjList[u] = outEdge;

            // Add u -> v in inList (v has incoming from u)
            EdgeNode* inEdge = new EdgeNode(u);
            inEdge->next = inList[v];
            inList[v] = inEdge;

            return true;
        }

        DynamicArray<string> getPrerequisites(const string& topic) const
        {
            DynamicArray<string> result;
            int v = findVertexIndex(topic);
            if (v == -1) return result;

            EdgeNode* cur = inList[v];
            while (cur != nullptr)
            {
                result.pushBack(vertices[cur->toIndex]);
                cur = cur->next;
            }
            return result;
        }

        DynamicArray<string> getDependents(const string& topic) const
        {
            DynamicArray<string> result;
            int u = findVertexIndex(topic);
            if (u == -1) return result;

            EdgeNode* cur = adjList[u];
            while (cur != nullptr)
            {
                result.pushBack(vertices[cur->toIndex]);
                cur = cur->next;
            }
            return result;
        }

        bool isUnlocked(const string& topic, const DynamicArray<string>& completedTopics) const
        {
            int v = findVertexIndex(topic);
            if (v == -1) return false;

            // Check if all prerequisites are in completedTopics
            EdgeNode* cur = inList[v];
            while (cur != nullptr)
            {
                string prereqName = vertices[cur->toIndex];
                bool found = false;
                for (int i = 0; i < completedTopics.getSize(); i++)
                {
                    if (completedTopics[i] == prereqName)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    return false;
                }
                cur = cur->next;
            }
            return true;
        }

        DynamicArray<string> getUnlockedTopics(const DynamicArray<string>& completedTopics) const
        {
            DynamicArray<string> result;
            for (int i = 0; i < vertices.getSize(); i++)
            {
                string topic = vertices[i];

                // Skip if already completed
                bool alreadyCompleted = false;
                for (int j = 0; j < completedTopics.getSize(); j++)
                {
                    if (completedTopics[j] == topic)
                    {
                        alreadyCompleted = true;
                        break;
                    }
                }

                if (!alreadyCompleted && isUnlocked(topic, completedTopics))
                {
                    result.pushBack(topic);
                }
            }
            return result;
        }

        // Topological Sort (Kahn's algorithm using custom Queue)
        DynamicArray<string> topologicalSort() const
        {
            DynamicArray<string> order;
            int numV = vertices.getSize();
            if (numV == 0) return order;

            DynamicArray<int> inDegree(numV);
            for (int i = 0; i < numV; i++)
            {
                int deg = 0;
                EdgeNode* cur = inList[i];
                while (cur != nullptr)
                {
                    deg++;
                    cur = cur->next;
                }
                inDegree.pushBack(deg);
            }

            Queue<int> q;
            for (int i = 0; i < numV; i++)
            {
                if (inDegree[i] == 0)
                {
                    q.enqueue(i);
                }
            }

            while (!q.isEmpty())
            {
                int u = q.dequeue();
                order.pushBack(vertices[u]);

                EdgeNode* cur = adjList[u];
                while (cur != nullptr)
                {
                    int v = cur->toIndex;
                    inDegree[v] = inDegree[v] - 1;
                    if (inDegree[v] == 0)
                    {
                        q.enqueue(v);
                    }
                    cur = cur->next;
                }
            }

            return order;
        }

        DynamicArray<string> getAllVertices() const
        {
            return vertices;
        }

        int getVertexCount() const
        {
            return vertices.getSize();
        }
};
