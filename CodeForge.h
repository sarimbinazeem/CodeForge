#pragma once

#include <iostream>
#include <string>

#include "Managers/ProblemManager.h"
#include "Managers/SubmissionManager.h"
#include "Managers/UndoRedoManager.h"
#include "Managers/SubmissionQueueManager.h"
#include "Managers/LeaderboardManager.h"
#include "Managers/SkillGraphManager.h"
#include "Storage/StorageManager.h"
#include "Judge/JudgeEngine.h"

using namespace std;

/*
CodeForgeSystem:
Unified Facade implementing all 8 Data Structure Features:
1. Dynamic Array — Problem Explorer (filtering, sorting, pagination)
2. Linked List   — Submission History (navigable bidirectional cursor, progression)
3. Stack         — Code Undo System (interactive undo/redo with dual stacks)
4. Queue         — Submission Processing (FIFO queue: Queued -> Running -> Verdict)
5. Hash Table    — Instant Problem Lookup (O(1) search by ID, Title, Tag)
6. Heap          — Live Leaderboard (priority queue ranking top performers)
7. BST           — Learning Roadmap (ordered progression, range queries)
8. Graph         — Skill Dependency System (DAG prerequisites & topic unlock)
*/

class CodeForgeSystem
{
    private:
        ProblemManager problemManager;
        SubmissionManager submissionManager;
        StorageManager storageManager;
        JudgeEngine judgeEngine;

        // New Integrated Feature Managers
        UndoRedoManager undoRedoManager;
        SubmissionQueueManager queueManager;
        LeaderboardManager leaderboardManager;
        SkillGraphManager skillGraphManager;

    public:
        CodeForgeSystem() {}

        // Load saved data when CodeForge starts
        void initialize()
        {
            cout << "\n============================== INITIALIZING CODEFORGE ==============================\n";

            // Load problems
            if (storageManager.loadProblems(problemManager))
            {
                cout << "[Storage] Problems loaded successfully! (Count: " << problemManager.getProblemCount() << ")\n";
            }
            else
            {
                cout << "[Storage] No saved problems found or initial start. Populating defaults...\n";
                seedDefaultProblems();
            }

            // Load submissions
            if (storageManager.loadSubmission(submissionManager))
            {
                cout << "[Storage] Submissions loaded successfully! (Count: " << submissionManager.getSubmissionCount() << ")\n";
            }
            else
            {
                cout << "[Storage] No previous submissions found.\n";
            }

            // Load real leaderboard from storage
            if (storageManager.loadLeaderboard(leaderboardManager))
            {
                cout << "[Storage] Leaderboard loaded successfully! (Real Users: " << leaderboardManager.getUserCount() << ")\n";
            }
            else
            {
                cout << "[Storage] Clean leaderboard initialized.\n";
            }

            cout << "========================== CODEFORGE READY WITH ALL 8 FEATURES =====================\n\n";
        }

        void seedDefaultProblems()
        {
            // Problem 501: Addition
            Problem p501(501, "Addition",
                         "Given two integers a and b, compute and print their sum.\n\nInput Format:\nTwo space-separated integers a and b.\n\nOutput Format:\nPrint the sum of a and b.",
                         "Easy", "Basic Programming", "basics", 1000);
            p501.addTestCase(TestCase("2 3", "5"));
            p501.addTestCase(TestCase("10 -4", "6"));
            p501.addTestCase(TestCase("0 0", "0"));
            problemManager.addProblem(p501);

            // Problem 502: Reverse Array
            Problem p502(502, "Reverse Array",
                         "Given an array of integers of size n, reverse the array in-place and print its elements separated by spaces.\n\nInput Format:\nFirst line: integer n\nSecond line: n integers\n\nOutput Format:\nReversed array elements separated by spaces.",
                         "Medium", "Arrays & Hashing", "array", 1400);
            p502.addTestCase(TestCase("5\n1 2 3 4 5", "5 4 3 2 1"));
            p502.addTestCase(TestCase("4\n10 20 30 40", "40 30 20 10"));
            problemManager.addProblem(p502);

            // Problem 800: Add Two Numbers
            Problem p800(800, "Add Two Numbers",
                         "Read two integers x and y from standard input and output their sum.\n\nInput Format:\nTwo integers x and y.\n\nOutput Format:\nSingle integer representing x + y.",
                         "Easy", "Basic Programming", "basics", 1050);
            p800.addTestCase(TestCase("100 200", "300"));
            p800.addTestCase(TestCase("-50 50", "0"));
            problemManager.addProblem(p800);

            // Problem 1: Two Sum
            Problem p1(1, "Two Sum",
                       "Given an array of integers nums and an integer target, return indices of the two numbers such that they add up to target.",
                       "Easy", "Arrays & Hashing", "array", 1100);
            p1.addTestCase(TestCase("4\n2 7 11 15\n9", "0 1"));
            p1.addTestCase(TestCase("3\n3 2 4\n6", "1 2"));
            problemManager.addProblem(p1);

            // Problem 2: Valid Parentheses
            Problem p2(2, "Valid Parentheses",
                       "Given a string s containing just characters '(', ')', '{', '}', '[' and ']', determine if input string is valid.",
                       "Easy", "Stack & Queue", "stack", 1150);
            p2.addTestCase(TestCase("()[]{}", "true"));
            p2.addTestCase(TestCase("(]", "false"));
            problemManager.addProblem(p2);

            // Problem 3: Reverse Linked List
            Problem p3(3, "Reverse Linked List",
                       "Given the head of a singly linked list, reverse the list, and return the reversed list.",
                       "Easy", "Linked List", "linked-list", 1200);
            p3.addTestCase(TestCase("5\n1 2 3 4 5", "5 4 3 2 1"));
            p3.addTestCase(TestCase("2\n1 2", "2 1"));
            problemManager.addProblem(p3);

            // Problem 4: Binary Search
            Problem p4(4, "Binary Search",
                       "Given an array of integers nums which is sorted in ascending order, and an integer target, search target in nums.",
                       "Easy", "Binary Search", "binary-search", 1300);
            p4.addTestCase(TestCase("6\n-1 0 3 5 9 12\n9", "4"));
            p4.addTestCase(TestCase("6\n-1 0 3 5 9 12\n2", "-1"));
            problemManager.addProblem(p4);

            // Problem 5: Maximum Subarray
            Problem p5(5, "Maximum Subarray",
                       "Given an integer array nums, find the subarray with the largest sum, and return its sum.",
                       "Medium", "Dynamic Programming", "dp", 1600);
            p5.addTestCase(TestCase("9\n-2 1 -3 4 -1 2 1 -5 4", "6"));
            p5.addTestCase(TestCase("1\n1", "1"));
            problemManager.addProblem(p5);

            // Problem 6: Number of Islands
            Problem p6(6, "Number of Islands",
                       "Given an m x n 2D binary grid representing a map of '1's (land) and '0's (water), return the number of islands.",
                       "Medium", "Graphs", "graphs", 1850);
            p6.addTestCase(TestCase("4 5\n1 1 1 1 0\n1 1 0 1 0\n1 1 0 0 0\n0 0 0 0 0", "1"));
            problemManager.addProblem(p6);

            saveData();
        }

        void seedDefaultUsers()
        {
            // Empty: Only real users who register or submit solutions will appear on the leaderboard
        }

        // Save all application data
        bool saveData()
        {
            bool problemsSaved = storageManager.saveProblems(problemManager);
            bool submissionsSaved = storageManager.saveSubmission(submissionManager);
            bool leaderboardSaved = storageManager.saveLeaderboard(leaderboardManager);

            if (problemsSaved && submissionsSaved && leaderboardSaved)
            {
                cout << "[Storage] All data persisted successfully.\n";
                return true;
            }

            cout << "[Storage] Some data could not be saved!\n";
            return false;
        }

        int registerOrFindUser(const string& username)
        {
            int uid = leaderboardManager.registerOrFindUser(username);
            saveData();
            return uid;
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

        // Synchronous submit solution (Feature backward-compatibility)
        bool submitSolution(int submissionID, int problemID, string sourceCode, string language, JudgeResult& result,
                            int userID = 1, string username = "Guest")
        {
            Problem* problem = problemManager.findProblemByID(problemID);
            if (problem == nullptr)
            {
                cout << "Problem not found!\n";
                return false;
            }

            Submission submission(submissionID, problemID, userID, username, sourceCode, language);

            if (!submissionManager.addSubmission(submission))
            {
                return false;
            }

            result = judgeEngine.judgeSubmission(*problem, submission);
            submissionManager.updateSubmissionStatus(submissionID, submission.getStatus());

            bool isAccepted = (submission.getStatus() == "Accepted");
            leaderboardManager.recordSubmission(userID, isAccepted, 100);

            saveData();
            return true;
        }

        // ================= FEATURE 1: DYNAMIC ARRAY — PROBLEM EXPLORER =================
        void displayProblemExplorer(const string& difficulty = "", const string& topic = "",
                                    const string& sortBy = "id", bool ascending = true,
                                    int page = 1, int pageSize = 5) const
        {
            problemManager.displayExplorer(difficulty, topic, sortBy, ascending, page, pageSize);
        }

        // ================= FEATURE 2: LINKED LIST — SUBMISSION HISTORY =================
        void startSubmissionHistoryBrowse(bool fromLatest = true)
        {
            if (fromLatest)
                submissionManager.startNavigationFromLatest();
            else
                submissionManager.startNavigationFromOldest();
        }

        bool historyNext() { return submissionManager.moveNext(); }
        bool historyPrevious() { return submissionManager.movePrevious(); }
        bool hasCurrentSubmission() const { return submissionManager.hasCurrent(); }
        Submission getCurrentSubmission() const { return submissionManager.getCurrent(); }

        void displayProblemProgression(int problemID) const
        {
            submissionManager.displayProblemProgression(problemID);
        }

        // ================= FEATURE 3: STACK — CODE UNDO SYSTEM =================
        void editorType(const string& code)
        {
            undoRedoManager.typeCode(code);
        }

        bool editorUndo()
        {
            return undoRedoManager.undo();
        }

        bool editorRedo()
        {
            return undoRedoManager.redo();
        }

        string getEditorCurrentCode() const
        {
            return undoRedoManager.getCurrentCode();
        }

        void displayEditor() const
        {
            undoRedoManager.displayEditorState();
        }

        // ================= FEATURE 4: QUEUE — SUBMISSION PROCESSING =================
        void enqueueSubmission(int submissionID, int problemID, string sourceCode, string language = "C++",
                               int userID = 1, string username = "Guest")
        {
            Submission sub(submissionID, problemID, userID, username, sourceCode, language);
            submissionManager.addSubmission(sub);
            queueManager.enqueueSubmission(sub);
        }

        bool processNextQueuedSubmission(JudgeResult& result)
        {
            bool res = queueManager.processNext(problemManager, submissionManager, judgeEngine, result);
            if (res)
            {
                saveData();
            }
            return res;
        }

        int processAllQueuedSubmissions()
        {
            int count = queueManager.processAll(problemManager, submissionManager, judgeEngine);
            if (count > 0)
            {
                saveData();
            }
            return count;
        }

        int getQueueSize() const
        {
            return queueManager.getQueueSize();
        }

        // ================= FEATURE 5: HASH TABLE — INSTANT PROBLEM LOOKUP =================
        Problem* lookupProblemByID(int id)
        {
            return problemManager.findProblemByID(id);
        }

        Problem* lookupProblemByTitle(const string& title)
        {
            return problemManager.findProblemByTitle(title);
        }

        DynamicArray<Problem> lookupProblemsByTag(const string& tag)
        {
            return problemManager.findProblemsByTag(tag);
        }

        // ================= FEATURE 6: HEAP — LIVE LEADERBOARD =================
        void recordUserProgress(int userID, bool accepted, int points = 100)
        {
            leaderboardManager.recordSubmission(userID, accepted, points);
        }

        void displayLeaderboard(int topK = 5) const
        {
            leaderboardManager.displayLeaderboard(topK);
        }

        // ================= FEATURE 7: BST — LEARNING ROADMAP =================
        void displayRoadmap() const
        {
            problemManager.displayLearningRoadmap();
        }

        DynamicArray<Problem> getRoadmapRange(int minRating, int maxRating) const
        {
            return problemManager.getRoadmapRange(minRating, maxRating);
        }

        // ================= FEATURE 8: GRAPH — SKILL DEPENDENCY SYSTEM =================
        void displaySkillTree() const
        {
            skillGraphManager.displaySkillTree();
        }

        bool completeSkillTopic(const string& skill)
        {
            return skillGraphManager.completeSkill(skill);
        }

        DynamicArray<Problem> getRecommendedProblems()
        {
            return skillGraphManager.getRecommendedProblems(problemManager);
        }

        // General displays
        void displayProblems() const { problemManager.displayAllProblems(); }
        void displaySubmissions() const { submissionManager.displaySubmissions(); }

        // Getters for individual managers
        ProblemManager& getProblemManager() { return problemManager; }
        SubmissionManager& getSubmissionManager() { return submissionManager; }
        UndoRedoManager& getUndoRedoManager() { return undoRedoManager; }
        SubmissionQueueManager& getQueueManager() { return queueManager; }
        LeaderboardManager& getLeaderboardManager() { return leaderboardManager; }
        SkillGraphManager& getSkillGraphManager() { return skillGraphManager; }
};
