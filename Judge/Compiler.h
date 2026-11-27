#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <filesystem>

using namespace std;

class Compiler
{
    private:
        string sourceFile;
        string executableFile;
        string errorFile;

    public:
        Compiler()
        {
            sourceFile = "Judge/Workspace/submission.cpp";
            executableFile = "Judge/Workspace/submission.exe";
            errorFile = "Judge/Workspace/compiler_errors.txt";
            filesystem::create_directories("Judge/Workspace");
        }

        string getExecutablePath() const
        {
            return executableFile;
        }

        string readErrors()
        {
            ifstream file(errorFile);
            if (!file.is_open())
            {
                return "Unable to read compiler errors.";
            }

            string errors;
            string line;
            while (getline(file, line))
            {
                errors += line + "\n";
            }
            file.close();
            return errors;
        }

        bool saveSourceCode(const string& sourceCode)
        {
            filesystem::create_directories("Judge/Workspace");
            ofstream file(sourceFile);
            if (!file.is_open())
            {
                cout << "Unable to create source file!\n";
                return false;
            }

            file << sourceCode;
            file.close();
            return true;
        }

        bool compile(const string& sourceCode, string& errorMessage)
        {
            errorMessage = "";

            if (!saveSourceCode(sourceCode))
            {
                errorMessage = "Source file creation failed.";
                return false;
            }

            // Clear previous errors
            ofstream clearFile(errorFile);
            clearFile.close();

            // Compilation command with standard optimizations
            string command =
                "g++ -std=c++17 -O2 \"" +
                sourceFile +
                "\" -o \"" +
                executableFile +
                "\" 2> \"" +
                errorFile +
                "\"";

            int result = system(command.c_str());

            if (result != 0)
            {
                errorMessage = readErrors();
                if (errorMessage.empty())
                {
                    errorMessage = "Compilation failed.";
                }
                return false;
            }

#ifndef _WIN32
            // Ensure executable permission on Unix systems
            system(("chmod +x \"" + executableFile + "\" 2>/dev/null").c_str());
#endif

            return true;
        }
};
