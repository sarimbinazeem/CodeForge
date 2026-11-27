#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <filesystem>

using namespace std;

class ProgramRunner
{
    private:
        string executableFile;
        string inputFile;
        string outputFile;
        string runtimeErrorFile;

    public:
        ProgramRunner(const string& exec = "Judge/Workspace/submission.exe")
        {
            executableFile = exec;
            inputFile = "Judge/Workspace/input.txt";
            outputFile = "Judge/Workspace/output.txt";
            runtimeErrorFile = "Judge/Workspace/runtime_error.txt";
            filesystem::create_directories("Judge/Workspace");
        }

        bool run(const string& input, string& output, string& errorMessage)
        {
            output = "";
            errorMessage = "";

            // Write input to input file
            ofstream in(inputFile);
            if (!in.is_open())
            {
                errorMessage = "Failed to create input file.";
                return false;
            }
            in << input;
            if (!input.empty() && input.back() != '\n')
            {
                in << "\n";
            }
            in.close();

            // Clear output and error files
            ofstream clearOut(outputFile);
            clearOut.close();
            ofstream clearErr(runtimeErrorFile);
            clearErr.close();

#ifdef _WIN32
            // Windows execution: normalize paths with backslashes
            string cleanExec = executableFile;
            for (char& c : cleanExec) if (c == '/') c = '\\';
            string cleanIn = inputFile;
            for (char& c : cleanIn) if (c == '/') c = '\\';
            string cleanOut = outputFile;
            for (char& c : cleanOut) if (c == '/') c = '\\';
            string cleanErr = runtimeErrorFile;
            for (char& c : cleanErr) if (c == '/') c = '\\';

            string command = cleanExec + " < " + cleanIn + " > " + cleanOut + " 2> " + cleanErr;
            int status = system(command.c_str());
            int exitCode = status;
#else
            // Linux / Unix execution with GNU timeout
            string command =
                "timeout 2s ./" + executableFile +
                " < \"" + inputFile + "\" > \"" +
                outputFile + "\" 2> \"" +
                runtimeErrorFile + "\"";

            int status = system(command.c_str());
            int exitCode = WEXITSTATUS(status);
#endif
            if (exitCode == 124)
            {
                errorMessage = "Time Limit Exceeded (Limit: 2.0s)";
                return false;
            }
            else if (exitCode != 0)
            {
                ifstream errFile(runtimeErrorFile);
                string errLine;
                string errDetails;
                while (getline(errFile, errLine))
                {
                    errDetails += errLine + "\n";
                }
                errFile.close();

                errorMessage = !errDetails.empty() ? errDetails : "Runtime Error (Exit code: " + to_string(exitCode) + ")";
                return false;
            }

            // Read output
            ifstream outFile(outputFile);
            if (!outFile.is_open())
            {
                errorMessage = "Failed to read program output.";
                return false;
            }

            string line;
            while (getline(outFile, line))
            {
                output += line + "\n";
            }
            outFile.close();

            return true;
        }
};
