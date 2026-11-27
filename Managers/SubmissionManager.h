#pragma once

#include <iostream>
#include <iomanip>
#include "../Models/Submission.h"
#include "../DSA/LinkedList.h"
#include "../DSA/DynamicArray.h"
using namespace std;

/*
Feature 2: Linked List — Submission History
Maintains a navigable, chronological history of submissions with bidirectional traversal,
attempt inspection, and progression tracking.
*/

class SubmissionManager
{
    private:
        LinkedList<Submission> submissions;

    public:
        SubmissionManager() {}

        bool addSubmission(const Submission& submission)
        {
            // Check for duplicate IDs
            for (int i = 0; i < submissions.getSize(); i++)
            {
                Submission current = submissions.getAt(i);
                if (current.getSubmissionID() == submission.getSubmissionID())
                {
                    cout << "Submission ID " << submission.getSubmissionID() << " already exists!\n";
                    return false;
                }
            }

            submissions.insertAtEnd(submission);
            return true;
        }

        bool findSubmissionByID(int id, Submission& result) const
        {
            for (int i = 0; i < submissions.getSize(); i++)
            {
                Submission current = submissions.getAt(i);
                if (current.getSubmissionID() == id)
                {
                    result = current;
                    return true;
                }
            }
            return false;
        }

        void displaySubmissions() const
        {
            if (submissions.isEmpty())
            {
                cout << "No submissions available!\n";
                return;
            }

            cout << "\n============================== SUBMISSION HISTORY ==============================\n";
            cout << left << setw(8)  << "Sub ID"
                        << setw(8)  << "Prob ID"
                        << setw(14) << "User"
                        << setw(10) << "Language"
                        << setw(20) << "Verdict/Status" << "\n";
            cout << "--------------------------------------------------------------------------------\n";

            for (int i = 0; i < submissions.getSize(); i++)
            {
                Submission current = submissions.getAt(i);
                cout << left << setw(8)  << current.getSubmissionID()
                            << setw(8)  << current.getProblemID()
                            << setw(14) << current.getUsername()
                            << setw(10) << current.getLanguage()
                            << setw(20) << current.getStatus() << "\n";
            }
            cout << "================================================================================\n\n";
        }

        void displayByProblem(int problemID) const
        {
            bool found = false;
            cout << "\n--- Submissions for Problem ID " << problemID << " ---\n";
            for (int i = 0; i < submissions.getSize(); i++)
            {
                Submission current = submissions.getAt(i);
                if (current.getProblemID() == problemID)
                {
                    current.display();
                    cout << "------------------------\n";
                    found = true;
                }
            }

            if (!found)
            {
                cout << "No submissions found for this problem!\n";
            }
        }

        bool deleteSubmission(int id)
        {
            for (int i = 0; i < submissions.getSize(); i++)
            {
                Submission current = submissions.getAt(i);
                if (current.getSubmissionID() == id)
                {
                    submissions.deleteAt(i);
                    cout << "Submission deleted successfully!\n";
                    return true;
                }
            }

            cout << "Submission not found!\n";
            return false;
        }

        bool updateSubmissionStatus(int id, const string& newStatus)
        {
            for (int i = 0; i < submissions.getSize(); i++)
            {
                Submission current = submissions.getAt(i);
                if (current.getSubmissionID() == id)
                {
                    current.setStatus(newStatus);
                    submissions.deleteAt(i);
                    submissions.insertAt(i, current);
                    return true;
                }
            }
            return false;
        }

        int getSubmissionCount() const
        {
            return submissions.getSize();
        }

        Submission getSubmissionAt(int index) const
        {
            if (index < 0 || index >= submissions.getSize())
                return Submission();

            return submissions.getAt(index);
        }

        bool isEmpty() const
        {
            return submissions.isEmpty();
        }

        void clear()
        {
            submissions.clear();
        }

        // ================= FEATURE 2: NAVIGABLE SUBMISSION HISTORY =================
        void startNavigationFromOldest()
        {
            submissions.resetCursorToBeginning();
        }

        void startNavigationFromLatest()
        {
            submissions.resetCursorToEnd();
        }

        bool moveNext()
        {
            return submissions.moveCursorNext();
        }

        bool movePrevious()
        {
            return submissions.moveCursorPrev();
        }

        bool hasCurrent() const
        {
            return submissions.hasCursorCurrent();
        }

        bool hasNext() const
        {
            return submissions.hasCursorNext();
        }

        bool hasPrevious() const
        {
            return submissions.hasCursorPrev();
        }

        Submission getCurrent() const
        {
            return submissions.getCursorData();
        }

        // Progression Tracker for a Problem
        DynamicArray<Submission> getProgressionHistory(int problemID) const
        {
            DynamicArray<Submission> history;
            for (int i = 0; i < submissions.getSize(); i++)
            {
                Submission s = submissions.getAt(i);
                if (s.getProblemID() == problemID)
                {
                    history.pushBack(s);
                }
            }
            return history;
        }

        void displayProblemProgression(int problemID) const
        {
            DynamicArray<Submission> attempts = getProgressionHistory(problemID);

            cout << "\n====================== PROGRESSION FOR PROBLEM " << problemID << " ======================\n";
            if (attempts.isEmpty())
            {
                cout << " No submission attempts recorded for Problem " << problemID << ".\n";
                cout << "=========================================================================\n\n";
                return;
            }

            cout << " Total Attempts: " << attempts.getSize() << "\n";
            cout << "-------------------------------------------------------------------------\n";

            bool isSolved = false;
            for (int i = 0; i < attempts.getSize(); i++)
            {
                const Submission& s = attempts[i];
                cout << " Attempt #" << (i + 1)
                     << " [Sub ID: " << s.getSubmissionID() << "] -> "
                     << "Verdict: " << s.getStatus() << " (" << s.getLanguage() << ")\n";

                if (s.getStatus() == "Accepted")
                {
                    isSolved = true;
                }
            }

            cout << "-------------------------------------------------------------------------\n";
            cout << " Final Status: " << (isSolved ? "SOLVED (ACCEPTED)" : "IN PROGRESS / UNSOLVED") << "\n";
            cout << "=========================================================================\n\n";
        }
};
