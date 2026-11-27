#pragma once

#include <iostream>
#include <string>
#include <iomanip>
#include "../DSA/Graph.h"
#include "../DSA/DynamicArray.h"
#include "ProblemManager.h"
using namespace std;

/*
Feature 8: Graph — Skill Dependency System
Models prerequisite concepts, dynamically unlocks advanced topics as prerequisites are completed,
and provides recommended problems based on unlocked skills.
*/

class SkillGraphManager
{
    private:
        Graph skillGraph;
        DynamicArray<string> completedSkills;

    public:
        SkillGraphManager()
        {
            setupDefaultCurriculum();
        }

        void setupDefaultCurriculum()
        {
            // Core DSA Skill Graph Dependencies
            skillGraph.addPrerequisite("Arrays & Hashing", "Two Pointers");
            skillGraph.addPrerequisite("Two Pointers", "Sliding Window");
            skillGraph.addPrerequisite("Arrays & Hashing", "Binary Search");
            skillGraph.addPrerequisite("Arrays & Hashing", "Stack & Queue");
            skillGraph.addPrerequisite("Stack & Queue", "Linked List");
            skillGraph.addPrerequisite("Linked List", "Trees");
            skillGraph.addPrerequisite("Trees", "Binary Search Tree");
            skillGraph.addPrerequisite("Trees", "Heap & Priority Queue");
            skillGraph.addPrerequisite("Trees", "Graphs");
            skillGraph.addPrerequisite("Graphs", "Dynamic Programming");
        }

        void addPrerequisite(const string& prereq, const string& dependent)
        {
            skillGraph.addPrerequisite(prereq, dependent);
        }

        bool completeSkill(const string& skillName)
        {
            for (int i = 0; i < completedSkills.getSize(); i++)
            {
                if (completedSkills[i] == skillName)
                {
                    cout << "[SkillSystem] Skill '" << skillName << "' is already completed.\n";
                    return false;
                }
            }

            completedSkills.pushBack(skillName);
            cout << "[SkillSystem] Completed skill: '" << skillName << "'! New dependencies evaluated.\n";
            return true;
        }

        bool isSkillUnlocked(const string& skillName) const
        {
            return skillGraph.isUnlocked(skillName, completedSkills);
        }

        DynamicArray<string> getUnlockedSkills() const
        {
            return skillGraph.getUnlockedTopics(completedSkills);
        }

        DynamicArray<string> getCompletedSkills() const
        {
            return completedSkills;
        }

        DynamicArray<string> getCurriculumOrder() const
        {
            return skillGraph.topologicalSort();
        }

        // Recommend problems for currently unlocked skills
        DynamicArray<Problem> getRecommendedProblems(ProblemManager& problemManager) const
        {
            DynamicArray<Problem> recommendations;
            DynamicArray<string> unlocked = getUnlockedSkills();

            for (int i = 0; i < unlocked.getSize(); i++)
            {
                string topic = unlocked[i];
                DynamicArray<Problem> topicProblems = problemManager.filterProblems("", topic);

                // If no exact topic match, also try substring match
                if (topicProblems.isEmpty())
                {
                    for (int p = 0; p < problemManager.getProblemCount(); p++)
                    {
                        Problem prob = problemManager.getProblemAt(p);
                        if (topic.find(prob.getTopic()) != string::npos ||
                            prob.getTopic().find(topic) != string::npos)
                        {
                            recommendations.pushBack(prob);
                        }
                    }
                }
                else
                {
                    for (int j = 0; j < topicProblems.getSize(); j++)
                    {
                        recommendations.pushBack(topicProblems[j]);
                    }
                }
            }

            return recommendations;
        }

        void displaySkillTree() const
        {
            DynamicArray<string> allSkills = skillGraph.getAllVertices();
            DynamicArray<string> unlocked = getUnlockedSkills();

            cout << "\n======================== SKILL DEPENDENCY SYSTEM ========================\n";
            cout << " Prerequisites and topic unlocks powered by Directed Acyclic Graph (DAG):\n";
            cout << "-------------------------------------------------------------------------\n";

            for (int i = 0; i < allSkills.getSize(); i++)
            {
                string skill = allSkills[i];
                bool isCompleted = false;
                for (int c = 0; c < completedSkills.getSize(); c++)
                {
                    if (completedSkills[c] == skill)
                    {
                        isCompleted = true;
                        break;
                    }
                }

                bool isAvail = !isCompleted && isSkillUnlocked(skill);

                string statusIcon = isCompleted ? "[COMPLETED]" : (isAvail ? "[UNLOCKED]" : "[LOCKED]");

                DynamicArray<string> prereqs = skillGraph.getPrerequisites(skill);
                string prereqStr = "";
                for (int p = 0; p < prereqs.getSize(); p++)
                {
                    prereqStr += prereqs[p] + (p + 1 < prereqs.getSize() ? ", " : "");
                }
                if (prereqStr.empty()) prereqStr = "None (Root topic)";

                cout << " " << left << setw(14) << statusIcon
                     << setw(26) << skill
                     << "Prerequisites: " << prereqStr << "\n";
            }

            cout << "-------------------------------------------------------------------------\n";
            cout << " Recommended Next Topics to Study:\n ";
            if (unlocked.isEmpty())
            {
                cout << "All available topics completed or locked!\n";
            }
            else
            {
                for (int u = 0; u < unlocked.getSize(); u++)
                {
                    cout << "-> " << unlocked[u] << (u + 1 < unlocked.getSize() ? "  " : "");
                }
                cout << "\n";
            }
            cout << "=========================================================================\n\n";
        }
};
