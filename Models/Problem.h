#pragma once

#include <iostream>
#include <string>
#include "TestCase.h"
using namespace std;

/*
Problem Model:
Contains problem metadata, difficulty rating, topic, tags, and test cases
*/

class Problem
{
    private:
        int problemID;
        string title;
        string description;
        string difficulty;
        string topic;
        string tag;
        int difficultyRating;

        TestCase* testCases;
        int count;

    public:
        Problem()
        {
            problemID = 0;
            title = "";
            description = "";
            difficulty = "Easy";
            topic = "General";
            tag = "basics";
            difficultyRating = 1000;

            testCases = nullptr;
            count = 0;
        }

        Problem(int id, string title, string description, string difficulty, string topic)
        {
            problemID = id;
            this->title = title;
            this->description = description;
            this->difficulty = difficulty;
            this->topic = topic;
            this->tag = topic;

            if (difficulty == "Easy") difficultyRating = 1000 + (id * 25);
            else if (difficulty == "Medium") difficultyRating = 1600 + (id * 25);
            else if (difficulty == "Hard") difficultyRating = 2200 + (id * 25);
            else difficultyRating = 1200;

            testCases = nullptr;
            count = 0;
        }

        Problem(int id, string title, string description, string difficulty, string topic, string tag, int rating)
        {
            problemID = id;
            this->title = title;
            this->description = description;
            this->difficulty = difficulty;
            this->topic = topic;
            this->tag = tag;
            this->difficultyRating = rating;

            testCases = nullptr;
            count = 0;
        }

        // Rule of Three
        Problem(const Problem& other)
        {
            problemID = other.problemID;
            title = other.title;
            description = other.description;
            difficulty = other.difficulty;
            topic = other.topic;
            tag = other.tag;
            difficultyRating = other.difficultyRating;

            count = other.count;
            if (count > 0)
            {
                testCases = new TestCase[count];
                for (int i = 0; i < count; i++)
                {
                    testCases[i] = other.testCases[i];
                }
            }
            else
            {
                testCases = nullptr;
            }
        }

        Problem& operator=(const Problem& other)
        {
            if (this != &other)
            {
                delete[] testCases;

                problemID = other.problemID;
                title = other.title;
                description = other.description;
                difficulty = other.difficulty;
                topic = other.topic;
                tag = other.tag;
                difficultyRating = other.difficultyRating;

                count = other.count;
                if (count > 0)
                {
                    testCases = new TestCase[count];
                    for (int i = 0; i < count; i++)
                    {
                        testCases[i] = other.testCases[i];
                    }
                }
                else
                {
                    testCases = nullptr;
                }
            }

            return *this;
        }

        ~Problem()
        {
            delete[] testCases;
        }

        void addTestCase(TestCase test)
        {
            TestCase* temp = new TestCase[count + 1];

            for (int i = 0; i < count; i++)
            {
                temp[i] = testCases[i];
            }

            temp[count] = test;

            delete[] testCases;
            testCases = temp;
            count++;
        }

        // Getters
        int getProblemID() const
        {
            return problemID;
        }

        string getTitle() const
        {
            return title;
        }

        string getDescription() const
        {
            return description;
        }

        string getDifficulty() const
        {
            return difficulty;
        }

        string getTopic() const
        {
            return topic;
        }

        string getTag() const
        {
            return tag;
        }

        int getDifficultyRating() const
        {
            return difficultyRating;
        }

        int getTestCaseCount() const
        {
            return count;
        }

        TestCase getTestCase(int index) const
        {
            if (index < 0 || index >= count)
            {
                return TestCase();
            }

            return testCases[index];
        }

        void setTag(const string& t)
        {
            tag = t;
        }

        void setDifficultyRating(int rating)
        {
            difficultyRating = rating;
        }

        void displayProblem() const
        {
            cout << "Problem ID: " << problemID << endl;
            cout << "Title: " << title << endl;
            cout << "Description: " << description << endl;
            cout << "Difficulty: " << difficulty << " (Rating: " << difficultyRating << ")" << endl;
            cout << "Topic: " << topic << " | Tag: " << tag << endl;
            cout << "Test Cases: " << count << endl;

            for (int i = 0; i < count; i++)
            {
                cout << "  Test Case " << i + 1 << endl;
                cout << "    Input: " << testCases[i].getInput() << endl;
                cout << "    Expected Output: " << testCases[i].getExpectedOutput() << endl;
            }
            cout << endl;
        }

        void updateDetails(string newTitle, string newDescription, string newDifficulty, string newTopic)
        {
            title = newTitle;
            description = newDescription;
            difficulty = newDifficulty;
            topic = newTopic;
            if (newDifficulty == "Easy") difficultyRating = 1000;
            else if (newDifficulty == "Medium") difficultyRating = 1600;
            else if (newDifficulty == "Hard") difficultyRating = 2200;
        }
};
