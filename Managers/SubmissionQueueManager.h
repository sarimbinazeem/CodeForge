#pragma once

#include <iostream>
#include <iomanip>
#include "../Models/Submission.h"
#include "../Models/JudgeResult.h"
#include "../DSA/Queue.h"
#include "../DSA/DynamicArray.h"
#include "ProblemManager.h"
#include "SubmissionManager.h"
#include "../Judge/JudgeEngine.h"

using namespace std;

/*
Feature 4: Queue — Submission Processing
Every submission enters a FIFO processing queue.
Evaluates submissions in order, progressing through statuses:
Queued -> Running -> Accepted / Rejected (Wrong Answer / Compilation Error / Runtime Error / TLE)
*/

class SubmissionQueueManager
{
    private:
        Queue<Submission> queue;

    public:
        SubmissionQueueManager() {}

        void enqueueSubmission(Submission sub)
        {
            sub.setStatus("Queued");
            queue.enqueue(sub);
            cout << "[Queue] Submission #" << sub.getSubmissionID() << " added to FIFO evaluation queue. [Status: Queued]\n";
        }

        bool processNext(ProblemManager& problemManager, SubmissionManager& submissionManager,
                         JudgeEngine& judgeEngine, JudgeResult& result)
        {
            if (queue.isEmpty())
            {
                cout << "[Queue] Evaluation queue is empty. No pending submissions.\n";
                return false;
            }

            Submission sub = queue.dequeue();
            int subID = sub.getSubmissionID();
            int probID = sub.getProblemID();

            cout << "\n[Queue] Processing Submission #" << subID << " for Problem #" << probID << "...\n";

            // Update status to Running
            sub.setStatus("Running");
            submissionManager.updateSubmissionStatus(subID, "Running");

            Problem* problem = problemManager.findProblemByID(probID);
            if (problem == nullptr)
            {
                sub.setStatus("Error (Problem Missing)");
                submissionManager.updateSubmissionStatus(subID, sub.getStatus());
                result = JudgeResult("Error", 0, 0, 0.0, 0.0, "Problem not found in problem manager.");
                return false;
            }

            // Execute judging
            result = judgeEngine.judgeSubmission(*problem, sub);

            // Update final verdict
            string finalStatus = sub.getStatus();
            submissionManager.updateSubmissionStatus(subID, finalStatus);

            cout << "[Queue] Finished Submission #" << subID << " -> Final Verdict: " << finalStatus << "\n";
            return true;
        }

        int processAll(ProblemManager& problemManager, SubmissionManager& submissionManager, JudgeEngine& judgeEngine)
        {
            int processedCount = 0;
            cout << "\n======================== PROCESSING QUEUED SUBMISSIONS ========================\n";
            while (!queue.isEmpty())
            {
                JudgeResult res;
                if (processNext(problemManager, submissionManager, judgeEngine, res))
                {
                    processedCount++;
                    res.displayResult();
                }
            }
            cout << "==============================================================================\n";
            cout << " Total Submissions Evaluated: " << processedCount << "\n\n";
            return processedCount;
        }

        int getQueueSize() const
        {
            return queue.getSize();
        }

        bool isEmpty() const
        {
            return queue.isEmpty();
        }

        void displayQueue() const
        {
            cout << "\n--- Current Pending Queue Size: " << queue.getSize() << " ---\n";
        }
};
