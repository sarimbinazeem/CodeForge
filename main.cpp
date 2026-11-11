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

using namespace std;


int main()
{
    // ==================================
    // DAY 7: COMPILER ENGINE TESTING
    // ==================================

    cout << "\n===== COMPILER ENGINE TEST =====\n";

    Compiler compiler;

    // Test 1: Valid C++ code

    string validCode = R"(
        #include <iostream>
        using namespace std;

        int main()
        {
            int a = 10;
            int b = 20;

            cout << a + b;

            return 0;
        }
        )";

    string errorMessage;

    cout << "\nTest 1: Valid C++ submission\n";

    if (compiler.compile(validCode, errorMessage))
    {
        cout << "Compilation successful!\n";
        cout << "Executable created successfully.\n";
    }
    else
    {
        cout << "Compilation failed!\n";
        cout << errorMessage << endl;
    }


    // Test 2: Invalid C++ code

    string invalidCode = R"(
        #include <iostream>
        using namespace std;

        int main()
        {
            cout << unknownVariable;

            return 0;
        }
        )";

    cout << "\nTest 2: Invalid C++ submission\n";

    if (compiler.compile(invalidCode, errorMessage))
    {
        cout << "Compilation successful!\n";
    }
    else
    {
        cout << "Compilation failed as expected!\n";
        cout << errorMessage << endl;
    }

    cout << "\n===== PROGRAM RUNNER TEST =====\n";

    ProgramRunner runner;

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

    string output;

    // First compile the program.
    cout << "\nCompiling submission...\n";

    if (compiler.compile(sourceCode, errorMessage))
    {
        cout << "Compilation successful!\n";

        // Now execute the compiled program.
        cout << "\nRunning submission...\n";

        string input = "10 20";

        if (runner.run(input, output, errorMessage))
        {
            cout << "Execution successful!\n";
            cout << "Program output: " << output;
        }
        else
        {
            cout << "Execution failed!\n";
            cout << errorMessage << endl;
        }
    }
    else
    {
        cout << "Compilation failed!\n";
        cout << errorMessage << endl;
    }

    return 0;
}
