#pragma once

#include <iostream>
#include <iomanip>
#include "../Models/User.h"
#include "../DSA/DynamicArray.h"
#include "../DSA/Heap.h"
using namespace std;

/*
Feature 6: Heap — Live Leaderboard
Maintains user performance and efficiently retrieves top performers using a custom MaxHeap / Priority Queue.
*/

class LeaderboardManager
{
    private:
        DynamicArray<User> users;

        int findUserIndex(int userID) const
        {
            for (int i = 0; i < users.getSize(); i++)
            {
                if (users[i].getUserID() == userID)
                {
                    return i;
                }
            }
            return -1;
        }

    public:
        LeaderboardManager() {}

        bool registerUser(int userID, const string& username, int initialScore = 0, int solved = 0, int attempts = 0)
        {
            if (findUserIndex(userID) != -1)
            {
                return false;
            }

            users.pushBack(User(userID, username, initialScore, solved, attempts));
            return true;
        }

        int registerOrFindUser(const string& username)
        {
            for (int i = 0; i < users.getSize(); i++)
            {
                if (users[i].getUsername() == username)
                {
                    return users[i].getUserID();
                }
            }
            int newID = 100 + users.getSize() + 1;
            users.pushBack(User(newID, username, 0, 0, 0));
            return newID;
        }

        bool recordSubmission(int userID, bool accepted, int pointsReward = 100)
        {
            int idx = findUserIndex(userID);
            if (idx == -1)
            {
                registerUser(userID, "User_" + to_string(userID));
                idx = findUserIndex(userID);
            }

            users[idx].recordAttempt(accepted, pointsReward);
            return true;
        }

        bool getUser(int userID, User& outUser) const
        {
            int idx = findUserIndex(userID);
            if (idx != -1)
            {
                outUser = users[idx];
                return true;
            }
            return false;
        }

        // Feature 6: MaxHeap extraction of top K performers
        DynamicArray<User> getTopK(int k = 10) const
        {
            MaxHeap<User> heap;
            for (int i = 0; i < users.getSize(); i++)
            {
                heap.push(users[i]);
            }

            return heap.getTopK(k);
        }

        void displayLeaderboard(int topK = 10) const
        {
            DynamicArray<User> topUsers = getTopK(topK);

            cout << "\n============================= LIVE LEADERBOARD =============================\n";
            cout << " Top performers ranked via custom MaxHeap Priority Queue:\n";
            cout << "-----------------------------------------------------------------------------\n";
            cout << left << setw(6)  << "Rank"
                        << setw(8)  << "User ID"
                        << setw(18) << "Username"
                        << setw(10) << "Score"
                        << setw(10) << "Solved"
                        << setw(10) << "Attempts"
                        << setw(12) << "Acc Rate" << "\n";
            cout << "-----------------------------------------------------------------------------\n";

            if (topUsers.isEmpty())
            {
                cout << " No users registered on leaderboard.\n";
            }
            else
            {
                for (int i = 0; i < topUsers.getSize(); i++)
                {
                    const User& u = topUsers[i];
                    cout << left << setw(6)  << (to_string(i + 1) + ".")
                                << setw(8)  << u.getUserID()
                                << setw(18) << u.getUsername()
                                << setw(10) << u.getScore()
                                << setw(10) << u.getSolvedCount()
                                << setw(10) << u.getAttemptCount()
                                << fixed << setprecision(1) << u.getAcceptanceRate() << "%" << "\n";
                }
            }
            cout << "=============================================================================\n\n";
        }

        int getUserCount() const
        {
            return users.getSize();
        }

        User getUserAt(int index) const
        {
            if (index < 0 || index >= users.getSize())
                return User();
            return users[index];
        }
};
