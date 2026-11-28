#pragma once

#include <iostream>
#include <string>
#include "DynamicArray.h"
#include "Queue.h"
using namespace std;

/*
 Graph — Skill Dependency System
Directed Graph 
 prerequisites,unlocks, and curriculum progression
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
        DynamicArray<EdgeNode*> adjList;    // Outdegree: u is prerequisite of v
        DynamicArray<EdgeNode*> inList;     // Indegree: v has prerequisite u

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
            //clear outdegree
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
            //clear indegree
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
            //check for duplicate
            int existing = findVertexIndex(name);
            if (existing != -1) return existing;

            int idx = vertices.getSize();
            vertices.pushBack(name);
            //currently this vertex have no indegree or no outdegree
            adjList.pushBack(nullptr);
            inList.pushBack(nullptr);
            return idx;
        }

        bool addPrerequisite(const string& prerequisite, const string& dependent)
        {
            //connect two vertices
            int u = addVertex(prerequisite);
            int v = addVertex(dependent);

            //update the adjancey lsits
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
            //we get from indegrees
            DynamicArray<string> result;
            int v = findVertexIndex(topic);
            if (v == -1) return result;

            // here cur contains the list of vertex v that contains a list of every node 
            //that sends an edge towards v
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
            //we get from outdegrees
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

                // Skip if already completed (Already mastered topic wont need to be mastered again)
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

            //calculate indegrees
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

            //enqueu the nodes whose indgeree is 0
            Queue<int> q;
            for (int i = 0; i < numV; i++)
            {
                if (inDegree[i] == 0)
                {
                    q.enqueue(i);
                }
            }

            //now run the loop until empty
            while (!q.isEmpty())
            {
                //dequeue and put in final array
                int u = q.dequeue();
                order.pushBack(vertices[u]);

                //check the adjacent of this node and decrement by 1
                //if the neighbors become 0 then enququ them
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
