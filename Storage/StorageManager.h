
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "../Models/Problem.h"
#include "../Managers/ProblemManager.h"

using namespace std;

/*
Storage in database so that the program contains memory
*/

class StorageManager
{
    private:
        string problemFile;

    public:
        StorageManager()
        {
            problemFile = "Storage/problems.dat";
            filesystem::create_directories("Storage");
        }
        
        // Write a string to a binary file.
        void writeString(ofstream& file, string value)
        {
            int length = (int)value.length();

            //create buffer in file to write 
            file.write((char*)&length, sizeof(length));
        
            if (length > 0)
            {
                file.write(value.c_str(), length);
            }
        }
        
        // Read a string from a binary file. and store intot a value string
        bool readString(ifstream& file, string& value)
        {
            int length;
        
            if (!file.read((char*)&length, sizeof(length)))
                return false;
        
            // Prevent invalid or excessively large allocations.
            if (length < 0 || length > 10000000)
                return false;
        
            value = "";
        
            if (length > 0)
            {
                value.resize(length);
        
                if (!file.read(&value[0], length))
                    return false;
            }

            return true;
        }
        
        // Save all problems.
        bool saveProblems(const ProblemManager& manager)
        {
            ofstream file(problemFile, ios::binary);
            
            if (!file.is_open())
                return false;
            
            int count = manager.getProblemCount();
            file.write((char*)&count, sizeof(count));
            
            for (int i = 0; i < count; i++)
            {
                Problem problem = manager.getProblemAt(i);

                int id = problem.getProblemID();

                file.write((char*)&id, sizeof(id));
                writeString(file, problem.getTitle());
                writeString(file, problem.getDescription());
                writeString(file, problem.getDifficulty());
                writeString(file, problem.getTopic());

                int testCount = problem.getTestCaseCount();
                file.write((char*)&testCount, sizeof(testCount));
                for (int j = 0; j < testCount; j++)
                {
                    TestCase test = problem.getTestCase(j);

                    writeString(file, test.getInput());
                    writeString(file, test.getExpectedOutput());
                }
            }

            file.close();

            return !file.fail();
        }

        // Load problems from storage.
        bool loadProblems(ProblemManager& manager)
        {
            ifstream file(problemFile, ios::binary);

            if (!file.is_open())
                return false;

            int count;

            if (!file.read((char*)&count, sizeof(count)))
                return false;

            if (count < 0 || count > 100000)
                return false;

            for (int i = 0; i < count; i++)
            {
                int id;

                if (!file.read((char*)&id, sizeof(id)))
                    return false;

                string title;
                string description;
                string difficulty;
                string topic;

                if (!readString(file, title) || !readString(file, description) ||
                    !readString(file, difficulty) || !readString(file, topic))
                {
                    return false;
                }

                int testCount;

                if (!file.read((char*)&testCount, sizeof(testCount)))
                    return false;

                if (testCount < 0 || testCount > 100000)
                    return false;

                //read from the file and make a problem object
                Problem problem(id,title,description,difficulty,topic);

                for (int j = 0; j < testCount; j++)
                {
                    //go through all the test cases and store each test cases into the problem
                    string input;
                    string expectedOutput;

                    if (!readString(file, input) || !readString(file, expectedOutput))
                    {
                        return false;
                    }

                    problem.addTestCase( TestCase(input, expectedOutput));
                }

                if (!manager.addProblem(problem))
                {
                    cout << "Problem ID " << id  << " already exists. Skipping.\n";
                }
            }

            file.close();

            return true;
        }
};
