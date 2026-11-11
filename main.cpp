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
#include "CodeForge.h"

using namespace std;


int main()
{
    cout << "\n===== CODEFORGE SYSTEM INTEGRATION TEST =====\n";

    // Create the central system.
    CodeForgeSystem system;

    // Load previously saved information.
    system.initialize();


    // ------------------------------------
    // STEP 1: ADD A NEW PROBLEM
    // ------------------------------------

    Problem problem(
        800,
        "Add Two Numbers",
        "Read two integers and print their sum.",
        "Easy",
        "Basic Programming"
    );

    problem.addTestCase(TestCase("10 20", "30"));
    problem.addTestCase(TestCase("5 7", "12"));

    cout << "\nAdding a new problem...\n";

    system.addProblem(problem);


    // ------------------------------------
    // STEP 2: SUBMIT A SOLUTION
    // ------------------------------------

    string sourceCode = R"(
    #include <iostream>
    using namespace std;

    int main()
    {
        int a, b;
        cin >> a >> b;

        cout << a + b;

        return 0;
    }
    )";

    JudgeResult result;

    cout << "\nSubmitting solution...\n";

    bool submitted = system.submitSolution(
        8001,
        800,
        sourceCode,
        "C++",
        result
    );

    if (submitted)
    {
        cout << "\n===== JUDGE RESULT =====\n";

        result.displayResult();
    }


    // ------------------------------------
    // STEP 3: DISPLAY DATA
    // ------------------------------------

    cout << "\n===== ALL PROBLEMS =====\n";

    system.displayProblems();

    cout << "\n===== ALL SUBMISSIONS =====\n";

    system.displaySubmissions();


    // ------------------------------------
    // STEP 4: SAVE EVERYTHING
    // ------------------------------------

    cout << "\nSaving CodeForge data...\n";

    system.saveData();


    return 0;
}
