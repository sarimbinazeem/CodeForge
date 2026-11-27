#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "../Models/Problem.h"
#include "../Managers/ProblemManager.h"
#include "../Models/Submission.h"
#include "../Managers/SubmissionManager.h"

using namespace std;

/*
StorageManager:
Handles binary file persistence for problems and submissions
Ensures 100% backward compatibility with saved data formats
*/

class StorageManager
{
    private:
        string problemFile;
        string submissionFile;

    public:
        StorageManager()
        {
            problemFile = "Storage/problems.dat";
            submissionFile = "Storage/submissions.dat";
            filesystem::create_directories("Storage");
        }

        // Write a string to a binary file
        void writeString(ofstream& file, const string& value)
        {
            int length = (int)value.length();
            file.write((char*)&length, sizeof(length));
            if (length > 0)
            {
                file.write(value.c_str(), length);
            }
        }

        // Read a string from a binary file
        bool readString(ifstream& file, string& value)
        {
            int length;
            if (!file.read((char*)&length, sizeof(length)))
                return false;

            if (length < 0 || length > 10000000)
                return false;

            value = "";
            if (length > 0)
            {
                value.resize(length);
                if (!file.read(&value[0], length))
                    return false;
            }

            return true;
        }

        // Save all problems
        bool saveProblems(const ProblemManager& manager)
        {
            ofstream file(problemFile, ios::binary);
            if (!file.is_open())
                return false;

            int count = manager.getProblemCount();
            file.write((char*)&count, sizeof(count));

            for (int i = 0; i < count; i++)
            {
                Problem problem = manager.getProblemAt(i);

                int id = problem.getProblemID();
                file.write((char*)&id, sizeof(id));
                writeString(file, problem.getTitle());
                writeString(file, problem.getDescription());
                writeString(file, problem.getDifficulty());
                writeString(file, problem.getTopic());

                int testCount = problem.getTestCaseCount();
                file.write((char*)&testCount, sizeof(testCount));
                for (int j = 0; j < testCount; j++)
                {
                    TestCase test = problem.getTestCase(j);
                    writeString(file, test.getInput());
                    writeString(file, test.getExpectedOutput());
                }
            }

            file.close();
            return !file.fail();
        }

        // Save all submissions
        bool saveSubmission(const SubmissionManager& manager)
        {
            ofstream file(submissionFile, ios::binary);
            if (!file.is_open())
            {
                return false;
            }

            int count = manager.getSubmissionCount();
            file.write((char*)&count, sizeof(count));
            for (int i = 0; i < count; i++)
            {
                Submission sub = manager.getSubmissionAt(i);
                int subID = sub.getSubmissionID();
                int probID = sub.getProblemID();

                file.write((char*)&subID, sizeof(subID));
                file.write((char*)&probID, sizeof(probID));

                writeString(file, sub.getSourceCode());
                writeString(file, sub.getLanguage());
                writeString(file, sub.getStatus());
            }

            file.close();
            return !file.fail();
        }

        // Load problems from storage
        bool loadProblems(ProblemManager& manager)
        {
            ifstream file(problemFile, ios::binary);
            if (!file.is_open())
                return false;

            int count;
            if (!file.read((char*)&count, sizeof(count)))
                return false;

            if (count < 0 || count > 100000)
                return false;

            for (int i = 0; i < count; i++)
            {
                int id;
                if (!file.read((char*)&id, sizeof(id)))
                    return false;

                string title, description, difficulty, topic;
                if (!readString(file, title) || !readString(file, description) ||
                    !readString(file, difficulty) || !readString(file, topic))
                {
                    return false;
                }

                int testCount;
                if (!file.read((char*)&testCount, sizeof(testCount)))
                    return false;

                if (testCount < 0 || testCount > 100000)
                    return false;

                Problem problem(id, title, description, difficulty, topic);

                for (int j = 0; j < testCount; j++)
                {
                    string input, expectedOutput;
                    if (!readString(file, input) || !readString(file, expectedOutput))
                    {
                        return false;
                    }
                    problem.addTestCase(TestCase(input, expectedOutput));
                }

                if (!manager.addProblem(problem))
                {
                    cout << "Problem ID " << id << " already exists. Skipping.\n";
                }
            }

            file.close();
            return true;
        }

        // Load submissions from storage
        bool loadSubmission(SubmissionManager& manager)
        {
            ifstream file(submissionFile, ios::binary);
            if (!file.is_open())
            {
                return false;
            }

            int count;
            if (!file.read((char*)&count, sizeof(count)))
            {
                return false;
            }

            if (count < 0 || count > 100000)
            {
                return false;
            }

            for (int i = 0; i < count; i++)
            {
                int submissionID, problemID;
                if (!file.read((char*)&submissionID, sizeof(submissionID)) ||
                    !file.read((char*)&problemID, sizeof(problemID)))
                {
                    return false;
                }

                string sourceCode, language, status;
                if (!readString(file, sourceCode) || !readString(file, language) || !readString(file, status))
                {
                    return false;
                }

                Submission submission(submissionID, problemID, sourceCode, language);
                submission.setStatus(status);

                if (!manager.addSubmission(submission))
                {
                    cout << "Submission ID " << submissionID << " already exists. Skipping.\n";
                }
            }

            file.close();
            return true;
        }
};
