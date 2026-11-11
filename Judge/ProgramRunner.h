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
        string errorFile;
        
    public:
        ProgramRunner()
        {
            //Initliaze Folders
            executableFile = "Judge/Workspace/submission.exe";
            inputFile = "Judge/Workspace/input.txt";
            outputFile = "Judge/Workspace/output.txt";
            errorFile = "Judge/Workspace/runtime_errors.txt";
        }
        
        // Read a file and return its contents.
        string readFile(string fileName)
         {
            ifstream file(fileName);
        
            if (!file.is_open())
            {
                return "";
            }
        
            string content;
            string line;
        
            while (getline(file, line))
            {
                content += line;
                content += "\n";
            }
            file.close();
            return content;
        }

        bool run(string input, string& output, string& errorMessage)
        {
            output = "";
            errorMessage = "";
            
            //check if the file exists
            if (!filesystem::exists(executableFile))
            {
                errorMessage = "Executable file not found!";
                return false;
            }

            // Save input for the submitted program.
            ofstream inputStream(inputFile);
            if (!inputStream.is_open())
            {
                errorMessage = "Unable to create input file!";
                return false;
            }
            inputStream << input;
            inputStream.close();

            // Clear old output and error files.
            ofstream clearOutput(outputFile);
            clearOutput.close();

            ofstream clearError(errorFile);
            clearError.close();

            // Run the executable and redirect input/output.
            string command =
                "\"" + executableFile + "\" < \"" + inputFile + "\" > \"" + 
                outputFile + "\" 2> \"" +
                errorFile + "\"";

            int result = system(command.c_str());

            // Read whatever the program produced.
            output = readFile(outputFile);
            errorMessage = readFile(errorFile);

            if (result != 0)
            {
                if (errorMessage.empty())
                {
                    errorMessage = "Program terminated with a non-zero exit code.";

                }

                return false;
            }

            return true;
        }
};
