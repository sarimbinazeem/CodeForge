#pragma once
#include <string>
#include <iostream>
using namespace std;

class JudgeResult
{
private:
    string verdict; // accepted , wrong answer, compilation error, runtime error, time limit, 
    int passed;
    int totalTestCases;
    double executionTime;
    double memoryUsed;
    string error;

public:

    // Default constructor
    JudgeResult()
    {
        verdict = "Pending";
        passed = 0;
        totalTestCases = 0;
        executionTime = 0.0;
        memoryUsed = 0.0;
        error = "";
    }

    // Parameterized constructor
    JudgeResult(string verdict,int passed,int totalTestCases,double executionTime,double memoryUsed,string error)
    {
        this->verdict = verdict;
        this->passed = passed;
        this->totalTestCases = totalTestCases;
        this->executionTime = executionTime;
        this->memoryUsed = memoryUsed;
        this->error = error;
    }

    // Getters
    string getVerdict() const
    {
        return verdict;
    }

    int getPassedTestCases() const
    {
        return passed;
    }

    int getTotalTestCases() const
    {
        return totalTestCases;
    }

    double getExecutionTime() const
    {
        return executionTime;
    }

    double getMemoryUsed() const
    {
        return memoryUsed;
    }

    string getError() const
    {
        return error;
    }

    // Display result
    void displayResult() const
    {
        cout << "Verdict: " << verdict<<endl;
        cout << "Test Cases Passed: "<< passed << "/"<< totalTestCases<<endl;
        cout << "Execution Time: "<< executionTime << " ms\n";
        cout << "Memory Used: "<< memoryUsed << " KB\n";
        if (error != "")
        {
            cout << "\nError: " << error;
        }
        cout << "\n";
    }
};
