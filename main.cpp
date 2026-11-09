#include <iostream>

#include "Models/TestCase.h"
#include "Models/Problem.h"
#include "Models/Submission.h"
#include "Models/JudgeResult.h"

#include "Managers/ProblemManager.h"

using namespace std;

int main()
{
    ProblemManager manager;


    Problem p1(1,"Add Two Numbers","Given two integers, print their sum.","Easy","Math");

    p1.addTestCase(TestCase("5 7", "12"));
    p1.addTestCase(TestCase("10 20", "30"));

    Problem p2(2,"Reverse an Array","Reverse the elements of an array.","Easy","Arrays");

    p2.addTestCase(TestCase("5 1 2 3 4 5", "5 4 3 2 1"));

    Problem p3(3,"Binary Search","Find a target in a sorted array.","Medium","Searching");

    p3.addTestCase(TestCase("5 1 2 3 4 5 4", "3"));

    Problem p4(4,"Merge Sort","Sort an array using merge sort.","Medium","Sorting");

    p4.addTestCase(TestCase("5 5 4 3 2 1", "1 2 3 4 5") );


    cout << "\n===== ADD PROBLEMS =====\n";

    manager.addProblem(p1);
    manager.addProblem(p2);
    manager.addProblem(p3);
    manager.addProblem(p4);

    cout << "\nTotal Problems: "<< manager.getProblemCount() << endl;

    cout << "\n===== DUPLICATE ID TEST =====\n";
    manager.addProblem(p1);

    cout << "\n===== DISPLAY ALL =====\n";
    manager.displayAllProblems();


    cout << "\n===== SEARCH BY ID =====\n";
    Problem* foundID = manager.findProblemByID(2);
    if (foundID != nullptr)
    {
        foundID->displayProblem();
    }
    else
    {
        cout << "Problem not found.\n";
    }

    cout << "\n===== SEARCH BY TITLE =====\n";
    Problem* foundTitle = manager.findProblemByTitle("Binary Search");
    if (foundTitle != nullptr)
    {
        foundTitle->displayProblem();
    }
    else
    {
        cout << "Problem not found.\n";
    }

    cout << "\n===== FILTER: EASY =====\n";
    manager.filterByDifficulty("Easy");

    cout << "\n===== FILTER: ARRAYS =====\n";
    manager.filterByTopic("Arrays");


    cout << "\n===== UPDATE PROBLEM =====\n";
    manager.updateProblem(1,"Addition of Two Integers","Calculate the sum of two integers.","Easy","Mathematics");


    cout << "\n===== DELETE PROBLEM =====\n";
    manager.deleteProblem(2);
    cout << "\nTotal Problems: " << manager.getProblemCount() << endl;


    cout << "\n===== RESIZING TEST =====\n";

    Problem p5( 5,"Graph Traversal","Traverse a graph.","Hard","Graphs");

    Problem p6( 6,"Stack Operations","Perform stack operations.","Easy","Stacks" );

    Problem p7(7,"Queue Operations","Perform queue operations.","Easy","Queues");

    manager.addProblem(p5);
    manager.addProblem(p6);
    manager.addProblem(p7);

    manager.displayAllProblems();

    cout << "\nTotal Problems: "<< manager.getProblemCount() << endl;

    return 0;
}
