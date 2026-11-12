#pragma once

// FOR WINDOWS 10 AND WINDOWS 11
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif

#ifndef WINVER
#define WINVER 0x0A00
#endif

#include <iostream>
#include "httplib.h"
#include "JsonHandler.h"
#include "../CodeForge.h"

using namespace std;
class APIServer
{
    private:
        CodeForgeSystem& system;
        httplib::Server server;

    public:
        APIServer(CodeForgeSystem& system): system(system)
        {
        }

        // Configure API routes
        void configureRoutes()
        {
            // Configure CORS for the React frontend
            server.set_default_headers({
                {"Access-Control-Allow-Origin", "http://localhost:5173"},
                {"Access-Control-Allow-Methods", "GET, POST, OPTIONS"},
                {"Access-Control-Allow-Headers", "Content-Type"}
            });

            // Handle browser preflight requests
            server.Options(R"(.*)",
                [](const httplib::Request& request,
                httplib::Response& response)
                {
                    response.status = 204;
                }
            );
            // Health check endpoint
            server.Get("/api/health", [](const httplib::Request& request, httplib::Response& response)
                {
                    response.set_content( "CodeForge API is running!", "text/plain" );
                }
            );

         // GET: Retrieve all problems
        server.Get("/api/problems",
            [this](const httplib::Request& request, httplib::Response& response)
            {
                ProblemManager& manager = system.getProblemManager();

                json problems = json::array();

                for (int i = 0; i < manager.getProblemCount(); i++)
                {
                    Problem problem = manager.getProblemAt(i);

                    problems.push_back(
                        JSONHandler::problemToJSON(problem)
                    );
                }

                response.set_content( problems.dump(4),"application/json");
            }
        );
        

        // GET: Retrieve one problem using its ID
        server.Get(R"(/api/problems/(\d+))",
            [this](const httplib::Request& request,
                httplib::Response& response)
            {
                int problemID;

                try
                {
                    problemID = stoi(request.matches[1].str());
                }
                catch (...)
                {
                    response.status = 400;

                    response.set_content(
                        R"({"error":"Invalid problem ID"})",
                        "application/json"
                    );

                    return;
                }

                ProblemManager& manager = system.getProblemManager();

                Problem* problem = manager.findProblemByID(problemID);

                if (problem == nullptr)
                {
                    response.status = 404;

                    response.set_content(
                        R"({"error":"Problem not found"})",
                        "application/json"
                    );

                    return;
                }

                json data = JSONHandler::problemToJSON(*problem);

                response.set_content(
                    data.dump(4),
                    "application/json"
                );
            }
        );

        // GET: Retrieve all submissions
        server.Get("/api/submissions",
            [this](const httplib::Request&,
                httplib::Response& response)
            {
                SubmissionManager& manager = system.getSubmissionManager();

                json submissions = json::array();

                for (int i = 0; i < manager.getSubmissionCount(); i++)
                {
                    Submission submission = manager.getSubmissionAt(i);

                    submissions.push_back(
                        JSONHandler::submissionToJSON(submission)
                    );
                }

                response.set_content(
                    submissions.dump(4),
                    "application/json"
                );
            }
        );

        // POST: Submit a solution for judging
        server.Post("/api/submissions",
            [this](const httplib::Request& request,
                httplib::Response& response)
            {
                json requestData;

                // Parse incoming JSON
                try
                {
                    requestData = json::parse(request.body);
                }
                catch (...)
                {
                    response.status = 400;

                    response.set_content(
                        R"({"error":"Invalid JSON request"})",
                        "application/json"
                    );

                    return;
                }

                // Validate required fields
                if (!requestData.contains("submissionID") ||
                    !requestData.contains("problemID") ||
                    !requestData.contains("sourceCode") ||
                    !requestData.contains("language"))
                {
                    response.status = 400;

                    response.set_content(
                        R"({"error":"Missing required fields"})",
                        "application/json"
                    );

                    return;
                }

                // Validate field types
                if (!requestData["submissionID"].is_number_integer() ||
                    !requestData["problemID"].is_number_integer() ||
                    !requestData["sourceCode"].is_string() ||
                    !requestData["language"].is_string())
                {
                    response.status = 400;

                    response.set_content(
                        R"({"error":"Invalid field types"})",
                        "application/json"
                    );

                    return;
                }

                int submissionID;
                int problemID;
                string sourceCode;
                string language;

                try
                {
                    submissionID = requestData["submissionID"].get<int>();
                    problemID = requestData["problemID"].get<int>();
                    sourceCode = requestData["sourceCode"].get<string>();
                    language = requestData["language"].get<string>();
                }
                catch (...)
                {
                    response.status = 400;

                    response.set_content(
                        R"({"error":"Invalid request values"})",
                        "application/json"
                    );

                    return;
                }

                // Check whether the problem exists
                Problem* problem = system.getProblemManager().findProblemByID(problemID);

                if (problem == nullptr)
                {
                    response.status = 404;

                    response.set_content(
                        R"({"error":"Problem not found"})",
                        "application/json"
                    );

                    return;
                }

                // Check for duplicate submission IDs
                Submission existingSubmission;

                if (system.getSubmissionManager().findSubmissionByID(submissionID, existingSubmission))
                {
                    response.status = 409;

                    response.set_content(
                        R"({"error":"Submission ID already exists"})",
                        "application/json"
                    );

                    return;
                }

                // Judge the submitted solution
                JudgeResult result(
                    "Pending", 0, 0, 0.0, 0.0, ""
                );

                bool success = system.submitSolution(
                    submissionID,
                    problemID,
                    sourceCode,
                    language,
                    result
                );

                if (!success)
                {
                    response.status = 500;

                    response.set_content(
                        R"({"error":"Failed to process submission"})",
                        "application/json"
                    );

                    return;
                }

                // Retrieve the updated submission
                Submission submission;

                system.getSubmissionManager().findSubmissionByID(
                    submissionID, submission
                );

                // Build the response
                json responseData;

                responseData["submission"] = JSONHandler::submissionToJSON(submission);

                responseData["result"] = JSONHandler::judgeResultToJSON(result);

                response.status = 201;

                response.set_content(
                    responseData.dump(4),
                    "application/json"
                );
            }
        );
        }
        // Start the HTTP server
        void start()
        {
            configureRoutes();

            cout << "\n================================\n";
            cout << "       CODEFORGE API SERVER\n";
            cout << "================================\n";

            cout << "Server starting at:\n";
            cout << "http://localhost:8080\n\n";

            cout << "Press Ctrl + C to stop the server.\n";

            if (!server.listen("127.0.0.1", 8080))
            {
                cout << "Failed to start the server!\n";
            }
        }

};
