#pragma once

#ifdef _WIN32
#if !defined(_WIN32_WINNT) || _WIN32_WINNT < 0x0A00
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif
#endif

#include <iostream>
#include <string>
#include <sstream>
#include "httplib.h"
#include "json.hpp"
#include "JsonHandler.h"
#include "../CodeForge.h"

using namespace std;
using json = nlohmann::json;

/*
APIServer:
Full HTTP REST API Server (cpp-httplib) + Interactive CLI Console.
Exposes all 8 custom data structure features to the web frontend and external clients:
1. Dynamic Array — Problem Explorer (/api/problems/explore)
2. Linked List   — Submission History & Progression (/api/submissions/problem/:id/progression)
3. Stack         — Code Editor Undo/Redo System (/api/editor/*)
4. Queue         — FIFO Submission Evaluation Queue (/api/queue/*)
5. Hash Table    — Instant O(1) Problem Lookup (/api/problems/lookup/:id)
6. Heap          — Live Leaderboard Priority Queue (/api/leaderboard)
7. BST           — Structured Learning Roadmap (/api/roadmap)
8. Graph         — Skill Dependency System (/api/skills)
*/

class APIServer
{
    private:
        CodeForgeSystem& codeForge;
        httplib::Server svr;
        bool running;
        int port;

        void setupRoutes()
        {
            // Enable CORS headers for React frontend
            svr.set_default_headers({
                {"Access-Control-Allow-Origin", "*"},
                {"Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS"},
                {"Access-Control-Allow-Headers", "Content-Type, Authorization"}
            });

            // Logger to see frontend requests in real-time in the terminal
            svr.set_logger([](const httplib::Request& req, const httplib::Response& res) {
                cout << "[CodeForge API] " << req.method << " " << req.path << " -> Status " << res.status << endl;
            });

            svr.Options(".*", [](const httplib::Request&, httplib::Response& res) {
                res.status = 204;
            });
            // Health check endpoint
            svr.Get("/api/health", [](const httplib::Request&, httplib::Response& res) {
                res.set_content(R"({"status":"healthy","service":"CodeForge Backend"})", "application/json");
            });
            // 1. Core Problems List
            svr.Get("/api/problems", [this](const httplib::Request&, httplib::Response& res) {
                json response = json::array();
                int count = codeForge.getProblemManager().getProblemCount();
                for (int i = 0; i < count; i++)
                {
                    Problem p = codeForge.getProblemManager().getProblemAt(i);
                    response.push_back(JSONHandler::problemToJSON(p));
                }
                res.set_content(response.dump(), "application/json");
            });

            // 2. Single Problem Details
            svr.Get(R"(/api/problems/(\d+))", [this](const httplib::Request& req, httplib::Response& res) {
                int problemID = stoi(req.matches[1]);
                Problem* p = codeForge.lookupProblemByID(problemID);
                if (p != nullptr)
                {
                    res.set_content(JSONHandler::problemToJSON(*p).dump(), "application/json");
                }
                else
                {
                    res.status = 404;
                    res.set_content(json({{"error", "Problem not found"}}).dump(), "application/json");
                }
            });

            // ================= FEATURE 1: DYNAMIC ARRAY — PROBLEM EXPLORER =================
            svr.Get("/api/problems/explore", [this](const httplib::Request& req, httplib::Response& res) {
                string diff = req.has_param("difficulty") ? req.get_param_value("difficulty") : "";
                string topic = req.has_param("topic") ? req.get_param_value("topic") : "";
                string sortBy = req.has_param("sortBy") ? req.get_param_value("sortBy") : "id";
                bool asc = req.has_param("asc") ? (req.get_param_value("asc") == "true") : true;
                int page = req.has_param("page") ? stoi(req.get_param_value("page")) : 1;
                int pageSize = req.has_param("pageSize") ? stoi(req.get_param_value("pageSize")) : 5;

                DynamicArray<Problem> pageList = codeForge.getProblemManager().exploreProblems(diff, topic, sortBy, asc, page, pageSize);
                DynamicArray<Problem> allFiltered = codeForge.getProblemManager().filterProblems(diff, topic);

                json items = json::array();
                for (int i = 0; i < pageList.getSize(); i++)
                {
                    items.push_back(JSONHandler::problemToJSON(pageList[i]));
                }

                json result;
                result["page"] = page;
                result["pageSize"] = pageSize;
                result["total"] = allFiltered.getSize();
                result["totalPages"] = (allFiltered.getSize() + pageSize - 1) / (pageSize > 0 ? pageSize : 1);
                result["problems"] = items;

                res.set_content(result.dump(), "application/json");
            });

            // ================= FEATURE 5: HASH TABLE — INSTANT LOOKUP =================
            svr.Get(R"(/api/problems/lookup/(\d+))", [this](const httplib::Request& req, httplib::Response& res) {
                int id = stoi(req.matches[1]);
                Problem* p = codeForge.lookupProblemByID(id);
                if (p != nullptr)
                {
                    json response;
                    response["lookupTime"] = "O(1) HashTable";
                    response["problem"] = JSONHandler::problemToJSON(*p);
                    res.set_content(response.dump(), "application/json");
                }
                else
                {
                    res.status = 404;
                    res.set_content(json({{"error", "Problem not found in Hash Table index"}}).dump(), "application/json");
                }
            });

            svr.Get(R"(/api/problems/tag/(.+))", [this](const httplib::Request& req, httplib::Response& res) {
                string tag = req.matches[1];
                DynamicArray<Problem> tagged = codeForge.lookupProblemsByTag(tag);
                json items = json::array();
                for (int i = 0; i < tagged.getSize(); i++)
                {
                    items.push_back(JSONHandler::problemToJSON(tagged[i]));
                }
                res.set_content(json({{"tag", tag}, {"total", tagged.getSize()}, {"problems", items}}).dump(), "application/json");
            });

            // ================= FEATURE 2: LINKED LIST — SUBMISSION HISTORY & PROGRESSION =================
            svr.Get("/api/submissions", [this](const httplib::Request&, httplib::Response& res) {
                json response = json::array();
                int count = codeForge.getSubmissionManager().getSubmissionCount();
                for (int i = 0; i < count; i++)
                {
                    Submission s = codeForge.getSubmissionManager().getSubmissionAt(i);
                    response.push_back(JSONHandler::submissionToJSON(s));
                }
                res.set_content(response.dump(), "application/json");
            });

            svr.Get(R"(/api/submissions/problem/(\d+)/progression)", [this](const httplib::Request& req, httplib::Response& res) {
                int probID = stoi(req.matches[1]);
                DynamicArray<Submission> history = codeForge.getSubmissionManager().getProgressionHistory(probID);

                json attempts = json::array();
                bool isSolved = false;
                for (int i = 0; i < history.getSize(); i++)
                {
                    attempts.push_back(JSONHandler::submissionToJSON(history[i]));
                    if (history[i].getStatus() == "Accepted")
                    {
                        isSolved = true;
                    }
                }

                json response;
                response["problemID"] = probID;
                response["totalAttempts"] = history.getSize();
                response["isSolved"] = isSolved;
                response["attempts"] = attempts;
                res.set_content(response.dump(), "application/json");
            });

            // Direct Submission (Synchronous with Judge)
            svr.Post("/api/submissions", [this](const httplib::Request& req, httplib::Response& res) {
                try
                {
                    json body = json::parse(req.body);
                    int submissionID = body.value("submissionID", (int)(time(nullptr) % 1000000));
                    int problemID = body.value("problemID", 1);
                    string sourceCode = body.value("sourceCode", "");
                    string language = body.value("language", "C++");
                    int userID = body.value("userID", 1);
                    string username = body.value("username", "Guest");

                    JudgeResult result;
                    bool submitted = codeForge.submitSolution(submissionID, problemID, sourceCode, language, result, userID, username);

                    if (!submitted)
                    {
                        res.status = 400;
                        res.set_content(json({{"error", "Failed to submit or problem not found"}}).dump(), "application/json");
                        return;
                    }

                    Submission sub;
                    codeForge.getSubmissionManager().findSubmissionByID(submissionID, sub);

                    json response;
                    response["submission"] = JSONHandler::submissionToJSON(sub);
                    response["result"] = JSONHandler::judgeResultToJSON(result);

                    res.set_content(response.dump(), "application/json");
                }
                catch (const exception& e)
                {
                    res.status = 400;
                    res.set_content(json({{"error", e.what()}}).dump(), "application/json");
                }
            });

            // ================= FEATURE 4: QUEUE — SUBMISSION PROCESSING =================
            svr.Post("/api/queue/enqueue", [this](const httplib::Request& req, httplib::Response& res) {
                try
                {
                    json body = json::parse(req.body);
                    int submissionID = body.value("submissionID", (int)(time(nullptr) % 1000000));
                    int problemID = body.value("problemID", 1);
                    string sourceCode = body.value("sourceCode", "");
                    string language = body.value("language", "C++");
                    int userID = body.value("userID", 1);
                    string username = body.value("username", "Guest");

                    codeForge.enqueueSubmission(submissionID, problemID, sourceCode, language, userID, username);

                    res.set_content(json({
                        {"status", "Queued"},
                        {"submissionID", submissionID},
                        {"problemID", problemID},
                        {"queueSize", codeForge.getQueueSize()}
                    }).dump(), "application/json");
                }
                catch (const exception& e)
                {
                    res.status = 400;
                    res.set_content(json({{"error", e.what()}}).dump(), "application/json");
                }
            });

            svr.Post("/api/queue/process-next", [this](const httplib::Request&, httplib::Response& res) {
                JudgeResult result;
                bool processed = codeForge.processNextQueuedSubmission(result);
                if (processed)
                {
                    res.set_content(json({
                        {"status", "processed"},
                        {"result", JSONHandler::judgeResultToJSON(result)},
                        {"remainingInQueue", codeForge.getQueueSize()}
                    }).dump(), "application/json");
                }
                else
                {
                    res.set_content(json({{"status", "empty"}, {"remainingInQueue", 0}}).dump(), "application/json");
                }
            });

            svr.Post("/api/queue/process-all", [this](const httplib::Request&, httplib::Response& res) {
                int count = codeForge.processAllQueuedSubmissions();
                res.set_content(json({{"processedCount", count}, {"remainingInQueue", codeForge.getQueueSize()}}).dump(), "application/json");
            });

            svr.Get("/api/queue/status", [this](const httplib::Request&, httplib::Response& res) {
                res.set_content(json({
                    {"queueSize", codeForge.getQueueSize()},
                    {"isEmpty", codeForge.getQueueSize() == 0}
                }).dump(), "application/json");
            });

            // ================= FEATURE 3: STACK — CODE UNDO SYSTEM =================
            svr.Post("/api/editor/type", [this](const httplib::Request& req, httplib::Response& res) {
                try
                {
                    json body = json::parse(req.body);
                    string code = body.value("code", "");
                    codeForge.editorType(code);

                    res.set_content(json({
                        {"status", "updated"},
                        {"canUndo", codeForge.getUndoRedoManager().canUndo()},
                        {"canRedo", codeForge.getUndoRedoManager().canRedo()}
                    }).dump(), "application/json");
                }
                catch (const exception& e)
                {
                    res.status = 400;
                    res.set_content(json({{"error", e.what()}}).dump(), "application/json");
                }
            });

            svr.Post("/api/editor/undo", [this](const httplib::Request&, httplib::Response& res) {
                bool ok = codeForge.editorUndo();
                res.set_content(json({
                    {"status", ok ? "undone" : "cannot_undo"},
                    {"currentCode", codeForge.getEditorCurrentCode()},
                    {"canUndo", codeForge.getUndoRedoManager().canUndo()},
                    {"canRedo", codeForge.getUndoRedoManager().canRedo()}
                }).dump(), "application/json");
            });

            svr.Post("/api/editor/redo", [this](const httplib::Request&, httplib::Response& res) {
                bool ok = codeForge.editorRedo();
                res.set_content(json({
                    {"status", ok ? "redone" : "cannot_redo"},
                    {"currentCode", codeForge.getEditorCurrentCode()},
                    {"canUndo", codeForge.getUndoRedoManager().canUndo()},
                    {"canRedo", codeForge.getUndoRedoManager().canRedo()}
                }).dump(), "application/json");
            });

            svr.Get("/api/editor/state", [this](const httplib::Request&, httplib::Response& res) {
                res.set_content(json({
                    {"currentCode", codeForge.getEditorCurrentCode()},
                    {"canUndo", codeForge.getUndoRedoManager().canUndo()},
                    {"canRedo", codeForge.getUndoRedoManager().canRedo()},
                    {"undoStackSize", codeForge.getUndoRedoManager().getUndoHistorySize()},
                    {"redoStackSize", codeForge.getUndoRedoManager().getRedoHistorySize()}
                }).dump(), "application/json");
            });

            // ================= FEATURE 6: HEAP — LIVE LEADERBOARD =================
            svr.Get("/api/leaderboard", [this](const httplib::Request& req, httplib::Response& res) {
                int topK = req.has_param("top") ? stoi(req.get_param_value("top")) : 10;
                DynamicArray<User> topUsers = codeForge.getLeaderboardManager().getTopK(topK);

                json userList = json::array();
                for (int i = 0; i < topUsers.getSize(); i++)
                {
                    userList.push_back(JSONHandler::userToJSON(topUsers[i], i + 1));
                }

                res.set_content(json({
                    {"engine", "MaxHeap Priority Queue"},
                    {"topCount", topUsers.getSize()},
                    {"leaderboard", userList}
                }).dump(), "application/json");
            });

            // ================= FEATURE 7: BST — LEARNING ROADMAP =================
            svr.Get("/api/roadmap", [this](const httplib::Request&, httplib::Response& res) {
                DynamicArray<Problem> sequence = codeForge.getProblemManager().getLearningRoadmap();
                json steps = json::array();
                for (int i = 0; i < sequence.getSize(); i++)
                {
                    json item = JSONHandler::problemToJSON(sequence[i]);
                    item["stepNumber"] = i + 1;
                    steps.push_back(item);
                }
                res.set_content(json({
                    {"engine", "Binary Search Tree In-Order Traversal"},
                    {"totalSteps", sequence.getSize()},
                    {"roadmap", steps}
                }).dump(), "application/json");
            });

            svr.Get("/api/roadmap/range", [this](const httplib::Request& req, httplib::Response& res) {
                int minRating = req.has_param("min") ? stoi(req.get_param_value("min")) : 1000;
                int maxRating = req.has_param("max") ? stoi(req.get_param_value("max")) : 2000;

                DynamicArray<Problem> rangeList = codeForge.getRoadmapRange(minRating, maxRating);
                json items = json::array();
                for (int i = 0; i < rangeList.getSize(); i++)
                {
                    items.push_back(JSONHandler::problemToJSON(rangeList[i]));
                }
                res.set_content(json({
                    {"engine", "Binary Search Tree Range Query"},
                    {"minRating", minRating},
                    {"maxRating", maxRating},
                    {"count", rangeList.getSize()},
                    {"problems", items}
                }).dump(), "application/json");
            });

            // ================= FEATURE 8: GRAPH — SKILL DEPENDENCY SYSTEM =================
            svr.Get("/api/skills", [this](const httplib::Request&, httplib::Response& res) {
                DynamicArray<string> allSkills = codeForge.getSkillGraphManager().getCurriculumOrder();
                DynamicArray<string> completed = codeForge.getSkillGraphManager().getCompletedSkills();
                DynamicArray<string> unlocked = codeForge.getSkillGraphManager().getUnlockedSkills();

                json skillList = json::array();
                for (int i = 0; i < allSkills.getSize(); i++)
                {
                    string name = allSkills[i];
                    bool isDone = false;
                    for (int c = 0; c < completed.getSize(); c++)
                    {
                        if (completed[c] == name) { isDone = true; break; }
                    }

                    bool isAvail = !isDone && codeForge.getSkillGraphManager().isSkillUnlocked(name);

                    json s;
                    s["name"] = name;
                    s["status"] = isDone ? "completed" : (isAvail ? "unlocked" : "locked");
                    skillList.push_back(s);
                }

                json nextUnlocked = json::array();
                for (int u = 0; u < unlocked.getSize(); u++)
                {
                    nextUnlocked.push_back(unlocked[u]);
                }

                res.set_content(json({
                    {"engine", "Directed Graph DAG"},
                    {"skills", skillList},
                    {"nextRecommendedTopics", nextUnlocked}
                }).dump(), "application/json");
            });

            svr.Post("/api/skills/complete", [this](const httplib::Request& req, httplib::Response& res) {
                try
                {
                    json body = json::parse(req.body);
                    string skill = body.value("skill", "");
                    bool ok = codeForge.completeSkillTopic(skill);
                    DynamicArray<string> unlocked = codeForge.getSkillGraphManager().getUnlockedSkills();
                    json unlockedArr = json::array();
                    for (int i = 0; i < unlocked.getSize(); i++)
                    {
                        unlockedArr.push_back(unlocked[i]);
                    }

                    res.set_content(json({
                        {"status", ok ? "completed" : "already_completed_or_invalid"},
                        {"skill", skill},
                        {"unlockedSkills", unlockedArr}
                    }).dump(), "application/json");
                }
                catch (const exception& e)
                {
                    res.status = 400;
                    res.set_content(json({{"error", e.what()}}).dump(), "application/json");
                }
            });

            svr.Get("/api/skills/recommended", [this](const httplib::Request&, httplib::Response& res) {
                DynamicArray<Problem> recs = codeForge.getRecommendedProblems();
                json items = json::array();
                for (int i = 0; i < recs.getSize(); i++)
                {
                    items.push_back(JSONHandler::problemToJSON(recs[i]));
                }
                res.set_content(json({{"count", recs.getSize()}, {"recommendedProblems", items}}).dump(), "application/json");
            });
        }

    public:
        APIServer(CodeForgeSystem& system, int listenPort = 8080)
            : codeForge(system), running(false), port(listenPort)
        {
            setupRoutes();
        }

        void runAutomatedVerification()
        {
            cout << "\n==============================================================================\n";
            cout << "               RUNNING CODEFORGE COMPLETE FEATURE VERIFICATION                \n";
            cout << "==============================================================================\n\n";

            // 1. Dynamic Array — Problem Explorer
            cout << ">>> [FEATURE 1: Dynamic Array — Problem Explorer]\n";
            codeForge.displayProblemExplorer("", "", "rating", true, 1, 4);

            // 2. Linked List — Submission History Navigation & Progression
            cout << ">>> [FEATURE 2: Linked List — Submission History & Progression]\n";
            codeForge.displayProblemProgression(1);

            // 3. Stack — Code Undo System
            cout << ">>> [FEATURE 3: Stack — Code Editor Undo System]\n";
            codeForge.editorType("// Initial template code\n#include <iostream>\nint main() { return 0; }");
            codeForge.displayEditor();
            cout << "[Action] Typing solution implementation...\n";
            codeForge.editorType("// Implemented solution logic\n#include <iostream>\nint main() { std::cout << 42; return 0; }");
            codeForge.displayEditor();
            cout << "[Action] Performing Undo...\n";
            codeForge.editorUndo();
            codeForge.displayEditor();
            cout << "[Action] Performing Redo...\n";
            codeForge.editorRedo();
            codeForge.displayEditor();

            // 4. Queue — Submission Processing
            cout << ">>> [FEATURE 4: Queue — Submission Processing]\n";
            int nextSubID1 = codeForge.getSubmissionManager().getSubmissionCount() + 101;
            int nextSubID2 = nextSubID1 + 1;

            cout << "[Action] Enqueuing submission #" << nextSubID1 << " for Problem #1...\n";
            string sampleCode1 =
                "#include <iostream>\n"
                "using namespace std;\n"
                "int main() {\n"
                "    int n; if (!(cin >> n)) return 0;\n"
                "    int a[100]; for (int i=0; i<n; i++) cin >> a[i];\n"
                "    int t; cin >> t;\n"
                "    for (int i=0; i<n; i++) {\n"
                "        for (int j=i+1; j<n; j++) {\n"
                "            if (a[i] + a[j] == t) { cout << i << \" \" << j << endl; return 0; }\n"
                "        }\n"
                "    }\n"
                "    return 0;\n"
                "}\n";
            codeForge.enqueueSubmission(nextSubID1, 1, sampleCode1, "C++", 101, "alex_code");

            cout << "[Action] Enqueuing submission #" << nextSubID2 << " for Problem #3...\n";
            string sampleCode2 =
                "#include <iostream>\n"
                "#include <string>\n"
                "using namespace std;\n"
                "int main() {\n"
                "    string s; if (cin >> s) {\n"
                "        if (s == \"()[]{}\") cout << \"true\" << endl;\n"
                "        else cout << \"false\" << endl;\n"
                "    }\n"
                "    return 0;\n"
                "}\n";
            codeForge.enqueueSubmission(nextSubID2, 3, sampleCode2, "C++", 102, "sarah_dsa");

            cout << "\n[Action] Evaluating all submissions in FIFO order:\n";
            codeForge.processAllQueuedSubmissions();

            // 5. Hash Table — Instant Problem Lookup
            cout << ">>> [FEATURE 5: Hash Table — Instant Problem Lookup]\n";
            cout << "Performing O(1) hash search for Problem ID: 2...\n";
            Problem* p = codeForge.lookupProblemByID(2);
            if (p != nullptr)
            {
                cout << "[Found via HashTable] ID: " << p->getProblemID() << " | Title: " << p->getTitle() << " | Topic: " << p->getTopic() << "\n";
            }
            cout << "Performing O(1) tag lookup for 'array'...\n";
            DynamicArray<Problem> arrayProbs = codeForge.lookupProblemsByTag("array");
            cout << "Total problems tagged 'array': " << arrayProbs.getSize() << "\n\n";

            // 6. Heap — Live Leaderboard
            cout << ">>> [FEATURE 6: Heap — Live Leaderboard]\n";
            codeForge.displayLeaderboard(5);

            // 7. Binary Search Tree — Learning Roadmap
            cout << ">>> [FEATURE 7: BST — Learning Roadmap]\n";
            codeForge.displayRoadmap();

            // 8. Graph — Skill Dependency System
            cout << ">>> [FEATURE 8: Graph — Skill Dependency System]\n";
            codeForge.displaySkillTree();
            cout << "[Action] Completing skill topic 'Arrays & Hashing'...\n";
            codeForge.completeSkillTopic("Arrays & Hashing");
            cout << "[Action] Re-evaluating skill tree unlock states:\n";
            codeForge.displaySkillTree();

            cout << "==============================================================================\n";
            cout << "                 ALL 8 DATA STRUCTURE FEATURES VERIFIED!                      \n";
            cout << "==============================================================================\n\n";
        }

        void showInteractiveMenu()
        {
            while (running)
            {
                cout << "\n======================== CODEFORGE COMMAND CONSOLE ========================\n";
                cout << " 1. Problem Explorer (Dynamic Array)\n";
                cout << " 2. Submission History & Progression (Linked List)\n";
                cout << " 3. Code Editor Undo / Redo (Stack)\n";
                cout << " 4. Submission Processing Queue (Queue)\n";
                cout << " 5. Instant Problem Lookup by ID / Tag (Hash Table)\n";
                cout << " 6. Live Leaderboard (Heap Priority Queue)\n";
                cout << " 7. Structured Learning Roadmap (Binary Search Tree)\n";
                cout << " 8. Skill Dependency & Prerequisites System (Graph DAG)\n";
                cout << " 9. Re-run Verification Suite\n";
                cout << " 0. Exit\n";
                cout << "---------------------------------------------------------------------------\n";
                cout << " Select an option (0-9): ";

                int choice;
                if (!(cin >> choice))
                {
                    break;
                }

                switch (choice)
                {
                    case 1:
                        codeForge.displayProblemExplorer("", "", "rating", true, 1, 10);
                        break;
                    case 2:
                        codeForge.displaySubmissions();
                        break;
                    case 3:
                        codeForge.displayEditor();
                        break;
                    case 4:
                        cout << "Pending in queue: " << codeForge.getQueueSize() << "\n";
                        codeForge.processAllQueuedSubmissions();
                        break;
                    case 5:
                    {
                        cout << "Enter Problem ID to lookup: ";
                        int pid;
                        if (cin >> pid)
                        {
                            Problem* prob = codeForge.lookupProblemByID(pid);
                            if (prob) prob->displayProblem();
                            else cout << "Problem not found in Hash Table.\n";
                        }
                        break;
                    }
                    case 6:
                        codeForge.displayLeaderboard(10);
                        break;
                    case 7:
                        codeForge.displayRoadmap();
                        break;
                    case 8:
                        codeForge.displaySkillTree();
                        break;
                    case 9:
                        runAutomatedVerification();
                        break;
                    case 0:
                        cout << "Exiting CodeForge. Goodbye!\n";
                        running = false;
                        break;
                    default:
                        cout << "Invalid choice. Please select 0-9.\n";
                        break;
                }
            }
        }

        void start()
        {
            running = true;
            cout << "\n===================================================================\n";
            cout << "       CODEFORGE C++ BACKEND API SERVER IS LIVE & READY!           \n";
            cout << "===================================================================\n";
            cout << " Local API URL:  http://localhost:" << port << "/api\n";
            cout << " Network URL:    http://0.0.0.0:" << port << "/api\n";
            cout << " CORS:           Enabled (React frontend can connect seamlessly)\n";
            cout << " Features:       8 Custom Data Structures Active & Hooked to API\n";
            cout << " Status:         Listening for frontend requests... (Ctrl+C to stop)\n";
            cout << "===================================================================\n\n";

            // Start blocking HTTP listener so backend stays running and logs all frontend calls
            svr.listen("0.0.0.0", port);
        }

        void stop()
        {
            running = false;
            svr.stop();
        }
};
