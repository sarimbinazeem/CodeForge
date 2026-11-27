#pragma once

#include <iostream>
#include <string>
#include "json.hpp"

#include "../Models/Problem.h"
#include "../Models/Submission.h"
#include "../Models/JudgeResult.h"
#include "../Models/User.h"

using namespace std;
using json = nlohmann::json;

class JSONHandler
{
public:
    static json testCaseToJSON(const TestCase& testCase)
    {
        json data;
        data["input"] = testCase.getInput();
        data["expectedOutput"] = testCase.getExpectedOutput();
        return data;
    }

    static json problemToJSON(const Problem& problem)
    {
        json data;
        data["problemID"] = problem.getProblemID();
        data["title"] = problem.getTitle();
        data["description"] = problem.getDescription();
        data["difficulty"] = problem.getDifficulty();
        data["topic"] = problem.getTopic();
        data["tag"] = problem.getTag();
        data["rating"] = problem.getDifficultyRating();
        data["difficultyRating"] = problem.getDifficultyRating();
        data["testCaseCount"] = problem.getTestCaseCount();

        // Expose test cases with input and expected output for the UI
        json testList = json::array();
        for (int i = 0; i < problem.getTestCaseCount(); i++)
        {
            TestCase tc = problem.getTestCase(i);
            json t;
            t["input"] = tc.getInput();
            t["expectedOutput"] = tc.getExpectedOutput();
            testList.push_back(t);
        }
        data["testCases"] = testList;

        return data;
    }

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

    static json submissionToJSON(const Submission& submission)
    {
        json data;
        data["submissionID"] = submission.getSubmissionID();
        data["problemID"] = submission.getProblemID();
        data["userID"] = submission.getUserID();
        data["username"] = submission.getUsername();
        data["sourceCode"] = submission.getSourceCode();
        data["language"] = submission.getLanguage();
        data["status"] = submission.getStatus();
        return data;
    }

    static json userToJSON(const User& user, int rank = 0)
    {
        json data;
        if (rank > 0)
        {
            data["rank"] = rank;
        }
        data["userID"] = user.getUserID();
        data["username"] = user.getUsername();
        data["score"] = user.getScore();
        data["solvedCount"] = user.getSolvedCount();
        data["attemptCount"] = user.getAttemptCount();
        data["acceptanceRate"] = user.getAcceptanceRate();
        return data;
    }
};