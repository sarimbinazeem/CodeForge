#pragma once

#include <string>
#include <iostream>
using namespace std;

/*
Submission Model:
Tracks ID, Problem ID, User ID, Source Code, Language, Verdict/Status
*/

class Submission
{
    private:
        int submissionID;
        int problemID;
        int userID;
        string username;
        string sourceCode;
        string language;
        string status; // "Queued", "Running", "Accepted", "Wrong Answer", "Compilation Error", "Runtime Error"

    public:
        Submission()
        {
            submissionID = 0;
            problemID = 0;
            userID = 1;
            username = "Guest";
            sourceCode = "";
            language = "C++";
            status = "Queued";
        }

        Submission(int submissionID, int problemID, string sourceCode, string language)
        {
            this->submissionID = submissionID;
            this->problemID = problemID;
            this->userID = 1;
            this->username = "Guest";
            this->sourceCode = sourceCode;
            this->language = language;
            this->status = "Queued";
        }

        Submission(int submissionID, int problemID, int userID, string username, string sourceCode, string language)
        {
            this->submissionID = submissionID;
            this->problemID = problemID;
            this->userID = userID;
            this->username = username;
            this->sourceCode = sourceCode;
            this->language = language;
            this->status = "Queued";
        }

        int getSubmissionID() const
        {
            return submissionID;
        }

        int getProblemID() const
        {
            return problemID;
        }

        int getUserID() const
        {
            return userID;
        }

        string getUsername() const
        {
            return username;
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

        void setUserID(int uid)
        {
            userID = uid;
        }

        void setUsername(string uname)
        {
            username = uname;
        }

        void display() const
        {
            cout << "Submission ID: " << submissionID << endl;
            cout << "Problem ID:    " << problemID << endl;
            cout << "User:          " << username << " (ID: " << userID << ")" << endl;
            cout << "Language:      " << language << endl;
            cout << "Status:        " << status << endl;
            cout << "Source Code Preview: " << sourceCode.substr(0, 80) << (sourceCode.length() > 80 ? "..." : "") << endl;
        }
};
