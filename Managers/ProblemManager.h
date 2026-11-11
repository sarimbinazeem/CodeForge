#pragma once
#include <iostream>
#include <string>
#include "../Models/Problem.h"

using namespace std;

/*
Problem manager

it manages and keep track of all the problems that exists in the program
*/

class ProblemManager
{
    private:

        Problem* problems;
        int problemCount;
        int capacity;

        // Resize Dynamic Array
        void resize()
        {
            int newCapacity = capacity * 2;

            Problem* temp = new Problem[newCapacity];

            for (int i = 0; i < problemCount; i++)
            {
                temp[i] = problems[i];
            }

            delete[] problems;

            problems = temp;
            capacity = newCapacity;
        }

public:

    // Constructor
    ProblemManager()
    {
        capacity = 2;
        problemCount = 0;

        problems = new Problem[capacity];
    }

    // Prevent accidental shallow copying
    ProblemManager(const ProblemManager&) = delete;

    ProblemManager& operator=(const ProblemManager&) = delete;

    // Destructor
    ~ProblemManager()
    {
        delete[] problems;
    }

    // Add Problem
    bool addProblem(const Problem& problem)
    {
        if (findProblemByID(problem.getProblemID()) != nullptr)
        {
            cout << "Problem ID already exists!\n";
            return false;
        }

        if (problemCount == capacity)
        {
            resize();
        }

        problems[problemCount] = problem;

        problemCount++;

        cout << "Problem added successfully!\n";

        return true;
    }

    // Display All Problems
    void displayAllProblems() const
    {
        if (problemCount == 0)
        {
            cout << "No problems available.\n";
            return;
        }

        for (int i = 0; i < problemCount; i++)
        {
            cout << "\n========== PROBLEM "
                 << i + 1 << " ==========\n";

            problems[i].displayProblem();
        }
    }

    // Search by ID
    Problem* findProblemByID(int id)
    {
        for (int i = 0; i < problemCount; i++)
        {
            if (problems[i].getProblemID() == id)
            {
                return &problems[i];
            }
        }

        return nullptr;
    }

    // Search by Title
    Problem* findProblemByTitle(string title)
    {
        for (int i = 0; i < problemCount; i++)
        {
            if (problems[i].getTitle() == title)
            {
                return &problems[i];
            }
        }

        return nullptr;
    }

    // Filter by Difficulty
    void filterByDifficulty(string difficulty) const
    {
        bool found = false;

        for (int i = 0; i < problemCount; i++)
        {
            if (problems[i].getDifficulty() == difficulty)
            {
                problems[i].displayProblem();
                found = true;
            }
        }

        if (!found)
        {
            cout << "No problems found for this difficulty.\n";
        }
    }

    // Filter by Topic
    void filterByTopic(string topic) const
    {
        bool found = false;

        for (int i = 0; i < problemCount; i++)
        {
            if (problems[i].getTopic() == topic)
            {
                problems[i].displayProblem();
                found = true;
            }
        }

        if (!found)
        {
            cout << "No problems found for this topic.\n";
        }
    }

    // Update Problem
    bool updateProblem(int id,string title,string description,string difficulty,string topic)
    {
        Problem* problem = findProblemByID(id);

        if (problem == nullptr)
        {
            cout << "Problem not found!\n";
            return false;
        }

        problem->updateDetails( title,description,difficulty,topic);

        cout << "Problem updated successfully!\n";

        return true;
    }

    // Delete Problem
    bool deleteProblem(int id)
    {
        int index = -1;

        for (int i = 0; i < problemCount; i++)
        {
            if (problems[i].getProblemID() == id)
            {
                index = i;
                break;
            }
        }

        if (index == -1)
        {
            cout << "Problem not found!\n";
            return false;
        }

        for (int i = index; i < problemCount - 1; i++)
        {
            problems[i] = problems[i + 1];
        }

        problemCount--;
        problems[problemCount] = Problem();
        cout << "Problem deleted successfully!\n";
        return true;
    }

    int getProblemCount() const
    {
        return problemCount;
    }

    //returns copy of problem
    Problem getProblemAt(int index) const
    {
        if (index < 0 || index >= problemCount)
            return Problem();

        return problems[index];
    }

};
