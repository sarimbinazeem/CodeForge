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

    // ==================================
    // DAY 3: DYNAMIC ARRAY TESTING
    // ==================================

    cout << "\n===== DYNAMIC ARRAY TEST =====\n";

    DynamicArray<int> arr;

    arr.pushBack(10);
    arr.pushBack(20);
    arr.pushBack(30);
    arr.pushBack(40);
    arr.pushBack(50);

    cout << "Elements: ";
    arr.display();

    cout << "Size: " << arr.getSize() << endl;
    cout << "Capacity: " << arr.getCapacity() << endl;

    arr.insertAt(2, 25);

    cout << "After inserting 25 at index 2: ";
    arr.display();

    arr.removeAt(1);

    cout << "After removing index 1: ";
    arr.display();

    cout << "Element at index 2: " << arr.getAt(2) << endl;

    cout << "Index of 40: " << arr.search(40) << endl;


    // ==================================
    // DAY 4: STACK TESTING
    // ==================================

    cout << "\n===== STACK TEST =====\n";

    Stack<int> myStack;

    myStack.push(10);
    myStack.push(20);
    myStack.push(30);
    myStack.push(40);

    cout << "Stack elements: ";
    myStack.display();

    cout << "Top element: " << myStack.top() << endl;

    cout << "Popped element: " << myStack.pop() << endl;

    cout << "After pop: ";
    myStack.display();

    cout << "Stack size: " << myStack.getSize() << endl;

    cout << "Is stack empty? " << myStack.isEmpty() << endl;


    // ==================================
    // DAY 4: QUEUE TESTING
    // ==================================

    cout << "\n===== QUEUE TEST =====\n";

    Queue<int> myQueue;

    myQueue.enqueue(10);
    myQueue.enqueue(20);
    myQueue.enqueue(30);
    myQueue.enqueue(40);

    cout << "Queue elements: ";
    myQueue.display();

    cout << "Front element: " << myQueue.front() << endl;

    cout << "Dequeued element: " << myQueue.dequeue() << endl;

    cout << "After dequeue: ";
    myQueue.display();

    cout << "Queue size: " << myQueue.getSize() << endl;

    cout << "Is queue empty? " << myQueue.isEmpty() << endl;


        // ==================================
        // DAY 5: HASH TABLE TESTING
        // ==================================

        cout << "\n===== HASH TABLE TEST =====\n";

        HashTable<string> hashTable;

        // Insert entries
        hashTable.insert(15, "Addition");
        hashTable.insert(22, "Binary Search");
        hashTable.insert(35, "Reverse Array");
        hashTable.insert(42, "Merge Sort");
        hashTable.insert(18, "Linked List");

        cout << "\nHash Table:\n";
        hashTable.display();

        // Search
        string result;

        cout << "\nSearching for key 35:\n";

        if (hashTable.search(35, result))
            cout << "Found: " << result << endl;
        else
            cout << "Key not found!\n";

        // Update existing key
        cout << "\nUpdating key 35:\n";

        hashTable.insert(35, "Array Reversal");

        if (hashTable.search(35, result))
            cout << "Updated value: " << result << endl;

        // Delete
        cout << "\nDeleting key 22:\n";

        if (hashTable.remove(22))
            cout << "Deleted successfully!\n";
        else
            cout << "Key not found!\n";

        // Display after deletion
        cout << "\nAfter deletion:\n";
        hashTable.display();

        // Information
        cout << "\nTotal entries: "
            << hashTable.getSize() << endl;

        cout << "Capacity: "
            << hashTable.getCapacity() << endl;

        cout << "Load Factor: "<< hashTable.getLoadFactor() << endl;

        cout << "\n===== RESIZING TEST =====\n";

        HashTable<int> resizeTest;

        for (int i = 1; i <= 8; i++)
        {
            resizeTest.insert(i, i * 100);
        }

        cout << "Entries: " << resizeTest.getSize() << endl;
        cout << "New capacity: " << resizeTest.getCapacity() << endl;

        int foundValue;

        if (resizeTest.search(5, foundValue))
            cout << "Key 5 value: " << foundValue << endl;


    // ==================================
    // DAY 6: SUBMISSION MANAGER TESTING
    // ==================================

    cout << "\n===== SUBMISSION MANAGER TEST =====\n";

    SubmissionManager submissionManager;

    // Create submissions
    Submission s1(
        101,
        1,
        "int main() { return 0; }",
        "C++"
    );

    Submission s2(
        102,
        2,
        "int main() { return 1; }",
        "C++"
    );

    Submission s3(
        103,
        1,
        "int main() { return 2; }",
        "C++"
    );

    // Add submissions
    submissionManager.addSubmission(s1);
    submissionManager.addSubmission(s2);
    submissionManager.addSubmission(s3);

    // Test duplicate ID
    cout << "\nTesting duplicate ID:\n";

    submissionManager.addSubmission(s1);

    // Display all
    cout << "\nAll submissions:\n";

    submissionManager.displaySubmissions();

    // Search by ID
    cout << "\nSearching for submission 102:\n";

    Submission foundSubmission;

    if (submissionManager.findSubmissionByID( 102, foundSubmission))
    {
        foundSubmission.display();
    }
    else
    {
        cout << "Submission not found!\n";
    }

    // Display by problem ID
    cout << "\nSubmissions for Problem 1:\n";

    submissionManager.displayByProblem(1);

    // Update status
    cout << "\nUpdating submission 101 status:\n";

    if (submissionManager.updateSubmissionStatus(101, "Accepted"))
    {
        cout << "Status updated successfully!\n";
    }

    // Search again to verify update
    if (submissionManager.findSubmissionByID(
        101, foundSubmission))
    {
        foundSubmission.display();
    }

    // Delete submission
    cout << "\nDeleting submission 102:\n";

    submissionManager.deleteSubmission(102);

    // Final display
    cout << "\nRemaining submissions:\n";

    submissionManager.displaySubmissions();

    cout << "\nTotal submissions: "<< submissionManager.getSubmissionCount() << endl;
    

    return 0;
}
