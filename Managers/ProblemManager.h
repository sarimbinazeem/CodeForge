#pragma once

#include <iostream>
#include <string>
#include <iomanip>
#include "../Models/Problem.h"
#include "../DSA/DynamicArray.h"
#include "../DSA/HashTable.h"
#include "../DSA/BinarySearchTree.h"

using namespace std;

/*
ProblemManager:
- Dynamic Array: Powers Problem Explorer (filtering, sorting, pagination) [Feature 1]
- Hash Table: Powers Instant Problem Lookup (O(1) search by ID, Title, Tag) [Feature 5]
- Binary Search Tree: Powers Learning Roadmap (ordered progression, range queries) [Feature 7]
*/

class ProblemManager
{
    private:
        // Feature 1: Dynamic Array problem storage
        DynamicArray<Problem> problems;

        // Feature 5: Hash Table indices for O(1) instant lookup
        HashTable<int, int> idToIndex;              // problemID -> array index
        HashTable<string, int> titleToIndex;        // problemTitle -> array index
        HashTable<string, DynamicArray<int>> tagIndex; // tag -> array of problem IDs

        // Feature 7: Binary Search Tree for structured learning progression
        BinarySearchTree<int, Problem> roadmapTree; // difficultyRating -> Problem

        void rebuildIndices()
        {
            idToIndex.clear();
            titleToIndex.clear();
            tagIndex.clear();
            roadmapTree.clear();

            for (int i = 0; i < problems.getSize(); i++)
            {
                int id = problems[i].getProblemID();
                string title = problems[i].getTitle();
                string tag = problems[i].getTag();
                int rating = problems[i].getDifficultyRating();

                idToIndex.insert(id, i);
                titleToIndex.insert(title, i);

                DynamicArray<int> taggedIDs;
                if (tagIndex.search(tag, taggedIDs))
                {
                    taggedIDs.pushBack(id);
                    tagIndex.insert(tag, taggedIDs);
                }
                else
                {
                    DynamicArray<int> newIDs;
                    newIDs.pushBack(id);
                    tagIndex.insert(tag, newIDs);
                }

                // Insert into BST Roadmap with unique key offset if needed
                roadmapTree.insert(rating * 1000 + id, problems[i]);
            }
        }

    public:
        ProblemManager() {}

        // Add problem & update all indices (Dynamic Array, Hash Table, BST)
        bool addProblem(const Problem& problem)
        {
            if (findProblemByID(problem.getProblemID()) != nullptr)
            {
                cout << "Problem ID " << problem.getProblemID() << " already exists!\n";
                return false;
            }

            int newIndex = problems.getSize();
            problems.pushBack(problem);

            // Update Hash Tables
            idToIndex.insert(problem.getProblemID(), newIndex);
            titleToIndex.insert(problem.getTitle(), newIndex);

            string tag = problem.getTag();
            DynamicArray<int> ids;
            if (tagIndex.search(tag, ids))
            {
                ids.pushBack(problem.getProblemID());
                tagIndex.insert(tag, ids);
            }
            else
            {
                DynamicArray<int> newIDs;
                newIDs.pushBack(problem.getProblemID());
                tagIndex.insert(tag, newIDs);
            }

            // Update BST Roadmap
            roadmapTree.insert(problem.getDifficultyRating() * 1000 + problem.getProblemID(), problem);

            return true;
        }

        // ================= FEATURE 5: HASH TABLE INSTANT LOOKUP =================
        // O(1) Instant Problem Lookup by ID
        Problem* findProblemByID(int id)
        {
            int index = -1;
            if (idToIndex.search(id, index))
            {
                if (index >= 0 && index < problems.getSize())
                {
                    return &problems[index];
                }
            }
            return nullptr;
        }

        // O(1) Instant Problem Lookup by Title
        Problem* findProblemByTitle(const string& title)
        {
            int index = -1;
            if (titleToIndex.search(title, index))
            {
                if (index >= 0 && index < problems.getSize())
                {
                    return &problems[index];
                }
            }
            return nullptr;
        }

        // O(1) Instant Tag Search
        DynamicArray<Problem> findProblemsByTag(const string& tag)
        {
            DynamicArray<Problem> result;
            DynamicArray<int> ids;
            if (tagIndex.search(tag, ids))
            {
                for (int i = 0; i < ids.getSize(); i++)
                {
                    Problem* p = findProblemByID(ids[i]);
                    if (p != nullptr)
                    {
                        result.pushBack(*p);
                    }
                }
            }
            return result;
        }

        // ================= FEATURE 1: DYNAMIC ARRAY PROBLEM EXPLORER =================
        // Filtering
        DynamicArray<Problem> filterProblems(const string& difficulty, const string& topic = "", const string& tag = "") const
        {
            DynamicArray<Problem> filtered;
            for (int i = 0; i < problems.getSize(); i++)
            {
                const Problem& p = problems[i];
                bool matchDiff = difficulty.empty() || difficulty == "All" || p.getDifficulty() == difficulty;
                bool matchTopic = topic.empty() || topic == "All" || p.getTopic() == topic;
                bool matchTag = tag.empty() || tag == "All" || p.getTag() == tag;

                if (matchDiff && matchTopic && matchTag)
                {
                    filtered.pushBack(p);
                }
            }
            return filtered;
        }

        // Sorting
        void sortProblems(DynamicArray<Problem>& list, const string& sortBy = "id", bool ascending = true) const
        {
            int n = list.getSize();
            for (int i = 0; i < n - 1; i++)
            {
                for (int j = 0; j < n - i - 1; j++)
                {
                    bool shouldSwap = false;
                    if (sortBy == "title")
                    {
                        shouldSwap = ascending ? (list[j].getTitle() > list[j + 1].getTitle())
                                               : (list[j].getTitle() < list[j + 1].getTitle());
                    }
                    else if (sortBy == "difficulty" || sortBy == "rating")
                    {
                        shouldSwap = ascending ? (list[j].getDifficultyRating() > list[j + 1].getDifficultyRating())
                                               : (list[j].getDifficultyRating() < list[j + 1].getDifficultyRating());
                    }
                    else // default: sort by ID
                    {
                        shouldSwap = ascending ? (list[j].getProblemID() > list[j + 1].getProblemID())
                                               : (list[j].getProblemID() < list[j + 1].getProblemID());
                    }

                    if (shouldSwap)
                    {
                        Problem temp = list[j];
                        list[j] = list[j + 1];
                        list[j + 1] = temp;
                    }
                }
            }
        }

        // Pagination
        DynamicArray<Problem> paginateProblems(const DynamicArray<Problem>& list, int pageNumber, int pageSize) const
        {
            DynamicArray<Problem> pageResult;
            if (pageNumber < 1) pageNumber = 1;
            if (pageSize < 1) pageSize = 1;

            int startIndex = (pageNumber - 1) * pageSize;
            int endIndex = startIndex + pageSize;

            for (int i = startIndex; i < endIndex && i < list.getSize(); i++)
            {
                pageResult.pushBack(list[i]);
            }

            return pageResult;
        }

        // Full Explorer Pipeline
        DynamicArray<Problem> exploreProblems(const string& difficulty = "", const string& topic = "",
                                              const string& sortBy = "id", bool ascending = true,
                                              int pageNumber = 1, int pageSize = 5) const
        {
            DynamicArray<Problem> result = filterProblems(difficulty, topic);
            sortProblems(result, sortBy, ascending);
            return paginateProblems(result, pageNumber, pageSize);
        }

        void displayExplorer(const string& difficulty = "", const string& topic = "",
                             const string& sortBy = "id", bool ascending = true,
                             int pageNumber = 1, int pageSize = 5) const
        {
            DynamicArray<Problem> allFiltered = filterProblems(difficulty, topic);
            sortProblems(allFiltered, sortBy, ascending);
            DynamicArray<Problem> pageItems = paginateProblems(allFiltered, pageNumber, pageSize);

            int totalItems = allFiltered.getSize();
            int totalPages = (totalItems + pageSize - 1) / (pageSize > 0 ? pageSize : 1);
            if (totalPages == 0) totalPages = 1;

            cout << "\n============================== PROBLEM EXPLORER ==============================\n";
            cout << " Filter: [Difficulty: " << (difficulty.empty() ? "All" : difficulty)
                 << " | Topic: " << (topic.empty() ? "All" : topic) << "]\n";
            cout << " Sort: [" << sortBy << " (" << (ascending ? "ASC" : "DESC") << ")]\n";
            cout << " Page " << pageNumber << " of " << totalPages << " (Total problems: " << totalItems << ")\n";
            cout << "------------------------------------------------------------------------------\n";
            cout << left << setw(6)  << "ID"
                        << setw(24) << "Title"
                        << setw(14) << "Difficulty"
                        << setw(10) << "Rating"
                        << setw(22) << "Topic"
                        << setw(16) << "Tag" << "\n";
            cout << "--------------------------------------------------------------------------------------\n";

            if (pageItems.isEmpty())
            {
                cout << " No problems found matching current criteria.\n";
            }
            else
            {
                for (int i = 0; i < pageItems.getSize(); i++)
                {
                    const Problem& p = pageItems[i];
                    cout << left << setw(6)  << p.getProblemID()
                                << setw(24) << (p.getTitle().length() > 22 ? p.getTitle().substr(0, 20) + ".." : p.getTitle())
                                << setw(14) << p.getDifficulty()
                                << setw(10) << p.getDifficultyRating()
                                << setw(22) << p.getTopic()
                                << setw(16) << p.getTag() << "\n";
                }
            }
            cout << "======================================================================================\n\n";
        }

        // ================= FEATURE 7: BST LEARNING ROADMAP =================
        // Get complete roadmap in ascending difficulty order via BST In-Order Traversal
        DynamicArray<Problem> getLearningRoadmap() const
        {
            return roadmapTree.inOrderTraversal();
        }

        // Get problems within specific difficulty rating range via BST Range Query
        DynamicArray<Problem> getRoadmapRange(int minRating, int maxRating) const
        {
            return roadmapTree.rangeQuery(minRating * 1000, (maxRating + 1) * 1000 - 1);
        }

        void displayLearningRoadmap() const
        {
            DynamicArray<Problem> roadmap = getLearningRoadmap();

            cout << "\n========================= CODEFORGE LEARNING ROADMAP =========================\n";
            cout << " Structured progression path powered by Binary Search Tree In-Order Traversal:\n";
            cout << "------------------------------------------------------------------------------\n";

            if (roadmap.isEmpty())
            {
                cout << " No roadmap steps available. Add problems to populate roadmap.\n";
            }
            else
            {
                for (int i = 0; i < roadmap.getSize(); i++)
                {
                    const Problem& p = roadmap[i];
                    cout << " Step " << setw(2) << right << (i + 1) << " | "
                         << "Rating: " << setw(4) << right << p.getDifficultyRating() << " | "
                         << setw(8) << left << p.getDifficulty() << " | "
                         << "ID " << setw(3) << left << p.getProblemID() << " : "
                         << setw(22) << left << p.getTitle() << " [" << p.getTopic() << "]\n";
                }
            }
            cout << "==============================================================================\n\n";
        }

        // ================= PREVIOUS INTERFACE METHODS (BACKWARD COMPATIBLE) =================
        void displayAllProblems() const
        {
            if (problems.isEmpty())
            {
                cout << "No problems available.\n";
                return;
            }

            for (int i = 0; i < problems.getSize(); i++)
            {
                cout << "\n========== PROBLEM " << i + 1 << " ==========\n";
                problems[i].displayProblem();
            }
        }

        void filterByDifficulty(const string& difficulty) const
        {
            DynamicArray<Problem> filtered = filterProblems(difficulty);
            if (filtered.isEmpty())
            {
                cout << "No problems found for this difficulty.\n";
                return;
            }
            for (int i = 0; i < filtered.getSize(); i++)
            {
                filtered[i].displayProblem();
            }
        }

        void filterByTopic(const string& topic) const
        {
            DynamicArray<Problem> filtered = filterProblems("", topic);
            if (filtered.isEmpty())
            {
                cout << "No problems found for this topic.\n";
                return;
            }
            for (int i = 0; i < filtered.getSize(); i++)
            {
                filtered[i].displayProblem();
            }
        }

        bool updateProblem(int id, const string& title, const string& description, const string& difficulty, const string& topic)
        {
            Problem* problem = findProblemByID(id);
            if (problem == nullptr)
            {
                cout << "Problem not found!\n";
                return false;
            }

            problem->updateDetails(title, description, difficulty, topic);
            rebuildIndices();
            cout << "Problem updated successfully!\n";
            return true;
        }

        bool deleteProblem(int id)
        {
            int index = -1;
            for (int i = 0; i < problems.getSize(); i++)
            {
                if (problems[i].getProblemID() == id)
                {
                    index = i;
                    break;
                }
            }

            if (index == -1)
            {
                cout << "Problem not found!\n";
                return false;
            }

            problems.removeAt(index);
            rebuildIndices();
            cout << "Problem deleted successfully!\n";
            return true;
        }

        int getProblemCount() const
        {
            return problems.getSize();
        }

        Problem getProblemAt(int index) const
        {
            if (index < 0 || index >= problems.getSize())
                return Problem();

            return problems[index];
        }
};
