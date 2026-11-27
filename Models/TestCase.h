#pragma once

#include <string>
using namespace std;

/*
Every test case has an input and an expected output
*/

class TestCase
{
    private:
        string input;
        string expectedOutput;

    public:
        TestCase()
        {
            input = "";
            expectedOutput = "";
        }

        TestCase(string input, string expectedOutput)
        {
            this->input = input;
            this->expectedOutput = expectedOutput;
        }

        string getInput() const
        {
            return input;
        }

        string getExpectedOutput() const
        {
            return expectedOutput;
        }
};
