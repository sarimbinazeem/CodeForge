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

using namespace std;


int main()
{

cout << "\n===== AUTOMATED JUDGE TEST =====\n";

JudgeEngine judge;

// Create a problem.
Problem testProblem(999,"Add Two Numbers","Read two integers and print their sum.","Easy","Basic Programming");

// Add test cases.
testProblem.addTestCase(TestCase("10 20", "30"));
testProblem.addTestCase(TestCase("5 7", "12"));


// ====================================
// TEST 1: ACCEPTED
// ====================================

string correctCode = R"(
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

Submission submission1(9001,999,correctCode,"C++");

cout << "\nTEST 1: Correct Submission\n";

JudgeResult result1 = judge.judgeSubmission(testProblem, submission1);

result1.displayResult();
submission1.display();


// ====================================
// TEST 2: WRONG ANSWER
// ====================================

string wrongCode = R"(
#include <iostream>
using namespace std;

int main()
{
    int a, b;
    cin >> a >> b;

    cout << a * b;

    return 0;
}
)";

Submission submission2(
    9002,
    999,
    wrongCode,
    "C++"
);

cout << "\nTEST 2: Wrong Answer Submission\n";

JudgeResult result2 =
    judge.judgeSubmission(testProblem, submission2);

result2.displayResult();
submission2.display();


// ====================================
// TEST 3: COMPILATION ERROR
// ====================================

string invalidCode = R"(
#include <iostream>
using namespace std;

int main()
{
    cout << unknownVariable;
    return 0;
}
)";

Submission submission3(
    9003,
    999,
    invalidCode,
    "C++"
);

cout << "\nTEST 3: Compilation Error Submission\n";

JudgeResult result3 =
    judge.judgeSubmission(testProblem, submission3);

result3.displayResult();
submission3.display();



    return 0;
}
