#include <iostream>
#include <string>
#include "TestCase.h"
using namespace std;

/*
Every Problem has a title , description, difficulty , and topic (a general topic which it asseses)
A problem ID also exists if there are many problems
Also it contains all the test cases that the problem's solution should satisfy
*/

class Problem
{
    private:
        int problemID;
        string title;
        string description;
        string difficulty;
        string topic;

        // Dynamically allocated array of testcases
        TestCase* testCases;
        int count;

    public:
        Problem()
        {
            problemID = 0;
            title = "";
            description = "";
            difficulty = "";
            topic = "";

            testCases = nullptr;
            count = 0;
        }

        Problem(int id, string title,string description,string difficulty,string topic)
        {
            problemID = id;
            this->title = title;
            this->description = description;
            this->difficulty = difficulty;
            this->topic = topic;

            testCases = nullptr;
            count = 0;
        }

        
        ~Problem()
        {
            delete[] testCases;
        }

        // Add a test case dynamically
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

        int getTestCaseCount() const
        {
            return count;
        }

        //a particular test case
        TestCase getTestCase(int index) const
        {
            if (index < 0 || index >= count)
            {
                return TestCase();
            }

            return testCases[index];
        }

        // Display problem details
        void displayProblem() const
        {
            cout << "Problem ID: " << problemID<<endl;
            cout << "Title: " << title<<endl;
            cout << "Description: " << description<<endl;
            cout << "Difficulty: " << difficulty<<endl;
            cout << "Topic: " << topic<<endl;

            cout << "Test Cases: " << count<<endl;
            
            for (int i = 0; i < count; i++)
            {
                cout << "Test Case " << i + 1<<endl;
                cout << "Input: "<< testCases[i].getInput()<<endl;

                cout << "Expected Output: "<< testCases[i].getExpectedOutput()<<endl;
            }

            cout<<endl;
        }
};
