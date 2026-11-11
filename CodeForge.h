
#pragma once

#include <iostream>
#include <string>

#include "Managers/ProblemManager.h"
#include "Managers/SubmissionManager.h"

#include "Storage/StorageManager.h"

#include "Judge/JudgeEngine.h"

using namespace std;

class CodeForgeSystem
{
    private:
        ProblemManager problemManager;
        SubmissionManager submissionManager;
        StorageManager storageManager;
        JudgeEngine judgeEngine;

    public:
        // Load saved data when CodeForge starts.
        void initialize()
        {
            cout << "\n===== INITIALIZING CODEFORGE =====\n";

            //load problems
            if (storageManager.loadProblems(problemManager))
            {
                cout << "Problems loaded successfully!\n";
            }
            else
            {
                cout << "No saved problems found or loading failed.\n";
            }

            //load submissions
            if (storageManager.loadSubmission(submissionManager))
            {
                cout << "Submissions loaded successfully!\n";
            }
            else
            {
                cout << "No saved submissions found or loading failed.\n";
            }

            cout << "\nCodeForge initialization completed!\n";
        }


        // Save all application data.
        bool saveData()
        {
            bool problems = storageManager.saveProblems(problemManager);
            bool submissions = storageManager.saveSubmission(submissionManager);

            if (problems && submissions)
            {
                cout << "All data saved successfully!\n";
                return true;
            }

            cout << "Some data could not be saved!\n";
            return false;
        }

        // Add a problem and save it
        bool addProblem(const Problem& problem)
        {
            if (!problemManager.addProblem(problem))
            {
                return false;
            }
            saveData();
            return true;
        }

        // Submit a solution and judge it.
        bool submitSolution(int submissionID,int problemID,string sourceCode,string language,JudgeResult& result)
        {
            // Find the requested problem.
            Problem* problem = problemManager.findProblemByID(problemID);

            if (problem == nullptr)
            {
                cout << "Problem not found!\n";
                return false;
            }

            // Create a submission.
            Submission submission(submissionID,problemID,sourceCode,language);

            // Store the submission before judging.
            if (!submissionManager.addSubmission(submission))
            {
                return false;
            }

            // Judge the submitted solution.
            result = judgeEngine.judgeSubmission(*problem, submission);

            // Update the stored submission's verdict.
            submissionManager.updateSubmissionStatus(submissionID, submission.getStatus() );

            // Save the updated submission.
            saveData();

            return true;
        }


        // Display all  problems.
        void displayProblems() const
        {
            problemManager.displayAllProblems();
        }


        // Display all  submissions.
        void displaySubmissions() const
        {
            submissionManager.displaySubmissions();
        }

        ProblemManager& getProblemManager()
        {
            return problemManager;
        }

        SubmissionManager& getSubmissionManager()
        {
            return submissionManager;
        }
};
