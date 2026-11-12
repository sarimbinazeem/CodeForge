
#pragma once

#include <iostream>
#include "json.hpp"

#include "../Models/Problem.h"
#include "../Models/Submission.h"
#include "../Models/JudgeResult.h"

using namespace std;
using json = nlohmann::json;

class JSONHandler
{
public:

    // Convert a TestCase into JSON
    static json testCaseToJSON(const TestCase& testCase)
    {
        json data;

        data["input"] = testCase.getInput();
        data["expectedOutput"] = testCase.getExpectedOutput();

        return data;
    }

    // Convert a Problem into JSON
    static json problemToJSON(const Problem& problem)
    {
        json data;

        data["problemID"] = problem.getProblemID();
        data["title"] = problem.getTitle();
        data["description"] = problem.getDescription();
        data["difficulty"] = problem.getDifficulty();
        data["topic"] = problem.getTopic();

        // Only expose the number of test cases.
        // Actual test cases and expected outputs remain private.
        data["testCaseCount"] = problem.getTestCaseCount();

        return data;
    }
    // Convert a JudgeResult into JSON
    static json judgeResultToJSON(const JudgeResult& result)
    {
        json data;

        data["verdict"] = result.getVerdict();
        data["passedTestCases"] = result.getPassedTestCases();
        data["totalTestCases"] = result.getTotalTestCases();
        data["executionTime"] = result.getExecutionTime();
        data["memoryUsed"] = result.getMemoryUsed();
        data["errorMessage"] = result.getError();

        return data;
    }
    // Convert a Submission into JSON
    static json submissionToJSON(const Submission& submission)
    {
        json data;

        data["submissionID"] = submission.getSubmissionID();
        data["problemID"] = submission.getProblemID();
        data["sourceCode"] = submission.getSourceCode();
        data["language"] = submission.getLanguage();
        data["status"] = submission.getStatus();

        return data;
    }
};
