#pragma once
#include <iostream>
#include "../Models/Submission.h"
#include "../DSA/LinkedList.h"
using namespace std;

class SubmissionManager
{
    private:
        LinkedList<Submission> submissions;

    public:
        bool addSubmission(const Submission& submission)
        {
            // duplicate  IDs
            for (int i = 0; i < submissions.getSize(); i++)
            {
                Submission current = submissions.getAt(i);

                if (current.getSubmissionID() == submission.getSubmissionID())
                {
                    cout << "Submission ID already exists!\n";
                    return false;
                }
            }

            submissions.insertAtEnd(submission);

            cout << "Submission added successfully!\n";

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

            for (int i = 0; i < submissions.getSize(); i++)
            {
                cout << "\n------------------------\n";

                Submission current = submissions.getAt(i);

                current.display();
            }
        }

        void displayByProblem(int problemID) const
        {
            bool found = false;

            for (int i = 0; i < submissions.getSize(); i++)
            {
                Submission current = submissions.getAt(i);

                if (current.getProblemID() == problemID)
                {
                    current.display();
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

        bool updateSubmissionStatus(int id, string newStatus)
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
        bool isEmpty() const
        {
            return submissions.isEmpty();
        }
        void clear()
        {
            submissions.clear();
        }
};
