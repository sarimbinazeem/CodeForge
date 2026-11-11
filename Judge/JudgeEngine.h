
#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include "../Models/Problem.h"
#include "../Models/Submission.h"
#include "../Models/JudgeResult.h"
#include "Compiler.h"
#include "ProgramRunner.h"

using namespace std;

class JudgeEngine
{
    private:
        //Every judge consists of a compiler and runner
        Compiler compiler;
        ProgramRunner runner;

        
    public:
        // Remove the unnecessary spaces
        string normalizeOutput(string output)
        {
            stringstream stream(output);

            string word;
            string normalized = "";

            while (stream >> word)
            {
                if (!normalized.empty())
                {
                    normalized += " ";
                }

                normalized += word;
            }

            return normalized;
        }

        JudgeResult judgeSubmission(Problem& problem, Submission& submission)
        {
            int totalCases = problem.getTestCaseCount();
            int passedCases = 0;

            double executionTime = 0.0;
            double memoryUsed = 0.0;

            string errorMessage;
            string output;

            // Check if test cases exist
            if (totalCases == 0)
            {
                submission.setStatus("No Test Cases");

                return JudgeResult("No Test Cases",0,0,0.0,0.0,"This problem has no test cases.");
            }

            // Compile the code
            submission.setStatus("Compiling");

            bool compiled = compiler.compile(submission.getSourceCode(),errorMessage);

            if (!compiled)
            {
                submission.setStatus("Compilation Error");

                return JudgeResult("Compilation Error",0,totalCases,executionTime,memoryUsed,errorMessage);
            }

            //  Execute every test case.
            for (int i = 0; i < totalCases; i++)
            {
                TestCase test = problem.getTestCase(i);

                bool executed = runner.run( test.getInput(), output,errorMessage );

                // Runtime Errors
                if (!executed)
                {
                    submission.setStatus("Runtime Error");
                    return JudgeResult("Runtime Error",passedCases,totalCases,executionTime,memoryUsed,errorMessage);
                }

                //  Compare actual and expected output.
                string actualOutput = normalizeOutput(output);
                string expectedOutput = normalizeOutput(test.getExpectedOutput());

                if (actualOutput == expectedOutput)
                {
                    passedCases++;
                }
                else
                {
                    submission.setStatus("Wrong Answer");
                    return JudgeResult("Wrong Answer",passedCases,totalCases,executionTime,memoryUsed,"Test case " + to_string(i + 1) + " failed. Expected: " + expectedOutput + " | Actual: " + actualOutput);
                }
            }

            // All test cases passed.
            submission.setStatus("Accepted");
            return JudgeResult("Accepted",passedCases,totalCases,executionTime,memoryUsed,"");
        }
};
