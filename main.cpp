
#include <iostream>

#include "Models/TestCase.h"
#include "Models/Problem.h"
#include "Models/Submission.h"
#include "Models/JudgeResult.h"

using namespace std;

int main()
{
    Problem problem(1,"Add Two Numbers","Given two integers, print their sum.","Easy","Math");

    TestCase test1("5 7", "12");
    TestCase test2("10 20", "30");
    TestCase test3("-5 5", "0");

    problem.addTestCase(test1);
    problem.addTestCase(test2);
    problem.addTestCase(test3);


    cout << "========== CODEFORGE ==========\n";

    problem.displayProblem();

    string code =
        "#include <iostream>\n"
        "using namespace std;\n"
        "int main() {\n"
        "int a, b;\n"
        "cin >> a >> b;\n"
        "cout << a + b;\n"
        "return 0;\n"
        "}";

    Submission submission(
        101,
        problem.getProblemID(),
        code,
        "C++"
    );

    submission.setStatus("Judged");

    cout << "\n========== SUBMISSION ==========\n";

    submission.display();


    JudgeResult result( "Accepted",3,3,12.5,1024 ,"");

    cout << "\n========== JUDGE RESULT ==========\n";

    result.displayResult();



    return 0;
}
