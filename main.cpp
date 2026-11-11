#include <iostream>

#include "Models/TestCase.h"
#include "Models/Problem.h"
#include "Models/Submission.h"
#include "Models/JudgeResult.h"

#include "Managers/ProblemManager.h"
#include "DSA/DynamicArray.h"
#include "DSA/LinkedList.h"
#include "DSA/Stack.h"
#include "DSA/Queue.h"
#include "DSA/HashTable.h"
#include "Managers/SubmissionManager.h"
#include "Judge/Compiler.h"
#include "Judge/ProgramRunner.h"
#include "Judge/JudgeEngine.h"
#include "Storage/StorageManager.h"

using namespace std;


int main()
{

    cout << "\n===== PROBLEM STORAGE TEST =====\n";

    StorageManager storage;

    // Create a manager with sample problems.
    ProblemManager storageTestManager;

    Problem p1(
        501,
        "Addition",
        "Read two integers and print their sum.",
        "Easy",
        "Basic Programming"
    );

    p1.addTestCase(TestCase("10 20", "30"));
    p1.addTestCase(TestCase("5 7", "12"));

    Problem p2(
        502,
        "Reverse Array",
        "Reverse the elements of an array.",
        "Medium",
        "Arrays"
    );

    p2.addTestCase(TestCase("3\n1 2 3", "3 2 1"));
    p2.addTestCase(TestCase("4\n5 6 7 8", "8 7 6 5"));

    storageTestManager.addProblem(p1);
    storageTestManager.addProblem(p2);

    // Save problems.
    cout << "\nSaving problems...\n";

    if (storage.saveProblems(storageTestManager))
    {
        cout << "Problems saved successfully!\n";
    }
    else
    {
        cout << "Unable to save problems!\n";
    }

    // Create a fresh manager.
    ProblemManager loadedManager;

    // Load saved problems.
    cout << "\nLoading problems...\n";

    if (storage.loadProblems(loadedManager))
    {
        cout << "Problems loaded successfully!\n";

        cout << "\nLoaded problem count: "
            << loadedManager.getProblemCount() << endl;

        loadedManager.displayAllProblems();
    }
    else
    {
        cout << "Unable to load problems!\n";
    }



    return 0;
}
