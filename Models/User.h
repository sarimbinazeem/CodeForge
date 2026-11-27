#pragma once

#include <iostream>
#include <string>
using namespace std;

/*
User Model for Live Leaderboard (Feature 6)
Tracks score, solved problems count, attempts, and acceptance rate
*/

class User
{
    private:
        int userID;
        string username;
        int score;
        int solvedCount;
        int attemptCount;

    public:
        User()
        {
            userID = 0;
            username = "Anonymous";
            score = 0;
            solvedCount = 0;
            attemptCount = 0;
        }

        User(int id, string name, int initialScore = 0, int solved = 0, int attempts = 0)
        {
            userID = id;
            username = name;
            score = initialScore;
            solvedCount = solved;
            attemptCount = attempts;
        }

        int getUserID() const { return userID; }
        string getUsername() const { return username; }
        int getScore() const { return score; }
        int getSolvedCount() const { return solvedCount; }
        int getAttemptCount() const { return attemptCount; }

        void addScore(int points)
        {
            score += points;
        }

        void recordAttempt(bool accepted, int pointsReward = 100)
        {
            attemptCount++;
            if (accepted)
            {
                solvedCount++;
                score += pointsReward;
            }
        }

        double getAcceptanceRate() const
        {
            if (attemptCount == 0) return 0.0;
            return (solvedCount * 100.0) / attemptCount;
        }

        bool operator<(const User& other) const
        {
            if (score != other.score)
            {
                return score < other.score;
            }
            if (solvedCount != other.solvedCount)
            {
                return solvedCount < other.solvedCount;
            }
            return attemptCount > other.attemptCount; // fewer attempts preferred on tie
        }

        bool operator>(const User& other) const
        {
            return other < *this;
        }

        bool operator==(const User& other) const
        {
            return userID == other.userID;
        }

        void displayUser() const
        {
            cout << "ID: " << userID
                 << " | User: " << username
                 << " | Score: " << score
                 << " | Solved: " << solvedCount
                 << " | Attempts: " << attemptCount
                 << " | Acceptance: " << getAcceptanceRate() << "%"
                 << endl;
        }
};
