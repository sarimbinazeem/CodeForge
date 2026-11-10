#pragma once
#include <string>
#include <iostream>
using namespace std;

/*
When a user submits their solution to a problem, CodeForge needs to record:
- Submission ID
- Problem ID
- Submitted C++ source code
- Programming language
- Submission status
*/

class Submission
{
    private:
        int submissionID;
        int problemID;
        string sourceCode;
        string language;
        string status;

    public:
        Submission()
        {
            submissionID = 0;
            problemID = 0;

            sourceCode = "";
            language = "C++";
            status = "Pending";
        }

        Submission(int submissionID, int problemID,string sourceCode,string language)
        {
            this->submissionID = submissionID;
            this->problemID = problemID;
            this->sourceCode = sourceCode;
            this->language = language;

            status = "Pending";
        }

        // Getters
        int getSubmissionID() const
        {
            return submissionID;
        }

        int getProblemID() const
        {
            return problemID;
        }

        string getSourceCode() const
        {
            return sourceCode;
        }

        string getLanguage() const
        {
            return language;
        }

        string getStatus() const
        {
            return status;
        }

        void setStatus(string status)
        {
            this->status = status;
        }

        void display() const
        {
            cout << "Submission ID: " << submissionID<<endl;
            cout << "Problem ID: " << problemID<<endl;
            cout << "Language: " << language<<endl;
            cout << "Status: " << status<<endl;
            cout << "Source Code: " << sourceCode<<endl;
        }
};
