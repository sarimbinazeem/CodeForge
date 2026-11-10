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
        
        // Constructor
        Compiler()
        {
            //this is the defualt path for the files to be saved at
            sourceFile = "Judge/Workspace/submission.cpp";
            executableFile = "Judge/Workspace/submission.exe";
            errorFile = "Judge/Workspace/compiler_errors.txt";
        }
        
        // Read compiler error messages
        string readErrors()
        {
            ifstream file(errorFile);

            if (!file.is_open())
            {
                return "Unable to read compiler errors.";

            }

            string errors;
            string line;
            
            //get the line from the text file and append it to the string of errors
            while (getline(file, line))
            {
                errors += line + "\n";
            }

            file.close();

            return errors;
        }

        bool saveSourceCode(string sourceCode)
        {
            //to create the directory (SAFETY IF NOT ALREADY EXISTS)
            filesystem::create_directories("Judge/Workspace");

            ofstream file(sourceFile);

            if (!file.is_open())
            {
                cout << "Unable to create source file!\n";
                return false;
            }

            //PUT IN FILE
            file << sourceCode;

            file.close();

            return true;
        }

        // Compile submitted C++ code 
        bool compile(string sourceCode, string& errorMessage)
        {
            errorMessage = "";

            // Save source code before compiling
            if (!saveSourceCode(sourceCode))
            {
                errorMessage = "Source file creation failed.";
                return false;
            }

            // Remove previous compiler errors
            ofstream clearFile(errorFile);
            clearFile.close();

            // Build compilation command (DEFAULT)
            string command =
                "g++ -std=c++17 -Wall -Wextra -O2 \"" +
                sourceFile +
                "\" -o \"" +
                executableFile +
                "\" 2> \"" +
                errorFile +
                "\"";

            // Performs a cmd task 
            //executes compiler
            //result here contains the success code
            // c_str converts string to C style string
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

            return true;
        }
};
