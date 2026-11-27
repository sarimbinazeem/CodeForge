#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "Models/Problem.h"
#include "Models/TestCase.h"
#include "Managers/ProblemManager.h"
#include "Storage/StorageManager.h"

using namespace std;

int main()
{
    cout << "Generating complete CodeForge problem database from ALL uploaded files...\n";

    ProblemManager pm;

    // ================= ORIGINAL USER PROBLEMS =================
    Problem p501(501, "Addition",
                 "Given two integers a and b, compute and print their sum.\n\nInput Format:\nTwo space-separated integers a and b.\n\nOutput Format:\nPrint the sum of a and b.",
                 "Easy", "Basic Programming", "basics", 1000);
    p501.addTestCase(TestCase("2 3", "5"));
    p501.addTestCase(TestCase("10 -4", "6"));
    p501.addTestCase(TestCase("0 0", "0"));
    p501.addTestCase(TestCase("999999 1", "1000000"));
    pm.addProblem(p501);

    Problem p502(502, "Reverse Array",
                 "Given an array of integers of size n, reverse the array in-place and print its elements separated by spaces.\n\nInput Format:\nFirst line: integer n\nSecond line: n space-separated integers\n\nOutput Format:\nReversed array elements separated by spaces.",
                 "Medium", "Arrays & Hashing", "array", 1400);
    p502.addTestCase(TestCase("5\n1 2 3 4 5", "5 4 3 2 1"));
    p502.addTestCase(TestCase("4\n10 20 30 40", "40 30 20 10"));
    p502.addTestCase(TestCase("1\n42", "42"));
    pm.addProblem(p502);

    Problem p800(800, "Add Two Numbers",
                 "Read two integers x and y from standard input and output their sum.\n\nInput Format:\nTwo integers x and y.\n\nOutput Format:\nSingle integer representing x + y.",
                 "Easy", "Basic Programming", "basics", 1050);
    p800.addTestCase(TestCase("100 200", "300"));
    p800.addTestCase(TestCase("-50 50", "0"));
    p800.addTestCase(TestCase("1234 5678", "6912"));
    pm.addProblem(p800);

    // ================= BATCH 1 (101 - 120) =================
    // 101: 1D Dynamic Array Using Pointers
    Problem p101(101, "1D Dynamic Array Sum",
                 "Dynamically allocate a 1D array of size n using pointers. Read n elements, compute the sum of all elements, print each element with its index, and print the total sum.\n\nInput Format:\nLine 1: integer n\nLine 2: n space-separated integers\n\nOutput Format:\nLine 1 to n: 'Element <i> : <value>'\nLast line: 'Sum Of Array: <sum>'",
                 "Easy", "Arrays & Hashing", "pointers", 1100);
    p101.addTestCase(TestCase("4\n10 20 30 40", "Element 0 : 10\nElement 1 : 20\nElement 2 : 30\nElement 3 : 40\nSum Of Array: 100"));
    p101.addTestCase(TestCase("3\n5 -2 7", "Element 0 : 5\nElement 1 : -2\nElement 2 : 7\nSum Of Array: 10"));
    pm.addProblem(p101);

    // 102: 2D Dynamic Array Matrix
    Problem p102(102, "2D Dynamic Array Matrix",
                 "Dynamically allocate a 2D array with r rows and c columns using pointers to pointers (int**). Read all elements, display the matrix row by row, and print the total sum of all elements.\n\nInput Format:\nLine 1: integers r and c\nNext r lines: c integers per row\n\nOutput Format:\nMatrix with elements separated by spaces.\nLast line: 'Sum: <sum>'",
                 "Easy", "Arrays & Hashing", "matrix", 1150);
    p102.addTestCase(TestCase("2 3\n1 2 3\n4 5 6", "1 2 3\n4 5 6\nSum: 21"));
    p102.addTestCase(TestCase("2 2\n10 20\n30 40", "10 20\n30 40\nSum: 100"));
    pm.addProblem(p102);

    // 103: Infix to Postfix Converter
    Problem p103(103, "Infix to Postfix Converter",
                 "Given an infix expression containing alphanumeric operands, operators (+, -, *, /, ^), and parentheses, convert it to its postfix notation using a Stack.\nPrecedence: ^ (3) > *, / (2) > +, - (1).\n\nInput Format:\nA single string representing the infix expression.\n\nOutput Format:\nThe corresponding postfix expression string.",
                 "Medium", "Stack & Queue", "stack", 1450);
    p103.addTestCase(TestCase("a+b*(c^d-e)^(f+g*h)-i", "abcd^e-*fgh*+^+i-"));
    p103.addTestCase(TestCase("(A+B)*(C-D)", "AB+CD-*"));
    p103.addTestCase(TestCase("A+B*C", "ABC*+"));
    pm.addProblem(p103);

    // 104: Postfix Expression Evaluation
    Problem p104(104, "Postfix Evaluation",
                 "Evaluate a given postfix expression containing single-digit operands (0-9) and arithmetic operators (+, -, *, /) using an integer Stack. Output the final evaluated integer result.\n\nInput Format:\nA single string representing the postfix expression.\n\nOutput Format:\nA single integer representing the evaluated value.",
                 "Medium", "Stack & Queue", "stack", 1500);
    p104.addTestCase(TestCase("231*+9-", "-4"));
    p104.addTestCase(TestCase("53+82-*", "48"));
    p104.addTestCase(TestCase("62/3*", "9"));
    pm.addProblem(p104);

    // 105: Sort Static Stack
    Problem p105(105, "Sort Static Stack",
                 "Given an array-based stack containing n integers, sort the elements in ascending order (smallest at bottom, largest at top) using sorting techniques on stack storage.\n\nInput Format:\nLine 1: integer n\nLine 2: n space-separated integers\n\nOutput Format:\nSorted stack elements displayed from top to bottom.",
                 "Medium", "Stack & Queue", "sorting", 1400);
    p105.addTestCase(TestCase("6\n20 10 30 70 3 1", "70 30 20 10 3 1"));
    p105.addTestCase(TestCase("4\n4 3 2 1", "4 3 2 1"));
    pm.addProblem(p105);

    // 106: Treasure Battle Simulation
    Problem p106(106, "Treasure Battle Game",
                 "Simulate the Treasure Battle round-robin game with a Queue and player action Stacks.\nEach player starts with 100 energy.\nCommands:\nF: Forward (-10 energy)\nB: Backward (0 energy)\nT: Treasure (+20 energy)\nU: Undo (reverses player's most recent valid F or T action).\nIf energy drops <= 0, player is eliminated.\n\nInput Format:\nLine 1: integer p (number of players) and their IDs\nLine 2: integer c (number of commands)\nLine 3: c space-separated commands\n\nOutput Format:\nEliminated: order of eliminated IDs (or None)\nRemaining: active player IDs with remaining energy.",
                 "Hard", "Stack & Queue", "simulation", 1850);
    p106.addTestCase(TestCase("2\n1 2\n4\nF F T U", "Eliminated: None\nActive: 1 (100) 2 (90)"));
    p106.addTestCase(TestCase("1\n10\n11\nF F F F F F F F F F F", "Eliminated: 10\nActive: None"));
    pm.addProblem(p106);

    // 107: Two Stacks in One Array
    Problem p107(107, "Two Stacks in One Array",
                 "Implement two stacks using a single dynamic array of size n. Stack 1 grows from left to right (index 0 upwards), and Stack 2 grows from right to left (index n-1 downwards).\n\nInput Format:\nLine 1: integer n\nLine 2: sequence of operations (push1 x, push2 y, pop1, pop2)\n\nOutput Format:\nOutputs of all pop operations separated by spaces.",
                 "Medium", "Stack & Queue", "stack", 1350);
    p107.addTestCase(TestCase("5\npush1 10\npush2 20\npush1 30\npop1\npop2", "30 20"));
    p107.addTestCase(TestCase("3\npush1 5\npush2 15\npush1 25\npop1\npop1\npop2", "25 5 15"));
    pm.addProblem(p107);

    // 108: Undo Battle 1D Movement
    Problem p108(108, "Undo Battle Movement",
                 "A player starts at position 0 on a 1D path. Valid movements:\nR: Move +1\nL: Move -1 (invalid if position < 0, discarded)\nJ: Jump +2\nB: Undo most recent valid move using a Stack.\nAt the end, output final position, successful movements, and successful undos.\n\nInput Format:\nLine 1: integer n\nLine 2: n space-separated commands\n\nOutput Format:\nFinal Position: <pos>\nMovements: <m>\nUndos: <u>",
                 "Medium", "Stack & Queue", "stack", 1400);
    p108.addTestCase(TestCase("5\nR R J B L", "Final Position: 1\nMovements: 3\nUndos: 1"));
    p108.addTestCase(TestCase("4\nL R J B", "Final Position: 1\nMovements: 2\nUndos: 1"));
    pm.addProblem(p108);

    // 109: Undo-Redo Word Editor
    Problem p109(109, "Undo-Redo Text Editor",
                 "Implement a text editor using two stacks (undoStack and redoStack) supporting:\ntype(word): appends word to document and clears redo history.\nundo(): reverses most recent typing action.\nredo(): re-applies an undone typing action.\n\nInput Format:\nLine 1: integer n\nNext n lines: operations (type <word>, undo, redo)\n\nOutput Format:\nDocument: <words separated by space>",
                 "Medium", "Stack & Queue", "dual-stack", 1500);
    p109.addTestCase(TestCase("7\ntype Hello\ntype World\nundo\nredo\nundo\ntype There\nredo", "Document: Hello There"));
    p109.addTestCase(TestCase("4\ntype DSA\ntype Is\ntype Fun\nundo", "Document: DSA Is"));
    pm.addProblem(p109);

    // 110: Operation History Stack
    Problem p110(110, "Operation History Stack",
                 "An editor stack of maximum capacity 8 stores editing operation codes as integers. After each push and pop (undo), track the top element. Print remaining operations in the stack from top to bottom.\n\nInput Format:\nSequence of operation integers and 'undo' commands.\n\nOutput Format:\nRemaining operations (top to bottom): <elements>",
                 "Easy", "Stack & Queue", "stack", 1200);
    p110.addTestCase(TestCase("6\n12 25 17 31 44 19\nundo 3\npush 52\nundo 2", "Remaining operations (top to bottom): 17 25 12"));
    pm.addProblem(p110);

    // 111: Bucket Sort
    Problem p111(111, "Bucket Sort",
                 "Implement Bucket Sort to sort an array of 2-digit positive integers. Distribute elements into 10 buckets based on their tens digit (index = value/10 - 1), sort each bucket using Insertion Sort, and merge into a sorted array.\n\nInput Format:\nLine 1: integer n\nLine 2: n 2-digit integers\n\nOutput Format:\nSorted integers separated by spaces.",
                 "Medium", "Sorting", "bucket-sort", 1550);
    p111.addTestCase(TestCase("7\n29 13 25 11 28 17 12", "11 12 13 17 25 28 29"));
    p111.addTestCase(TestCase("5\n45 12 89 23 67", "12 23 45 67 89"));
    pm.addProblem(p111);

    // 112: CGPA Merit Ranking System
    Problem p112(112, "CGPA Merit Ranking System",
                 "Given a list of students with ID, Name, Department, and CGPA, rank students in descending order of CGPA using stable O(n log n) Merge Sort. Awards: CGPA >= 4.00 -> 'Rector's List', CGPA >= 3.50 -> 'Dean's List', else 'No Award'.\n\nInput Format:\nLine 1: integer n\nNext n lines: <id> <name> <dept> <cgpa>\n\nOutput Format:\n<Rank> <ID> <Name> <Department> <CGPA> <Award>",
                 "Medium", "Sorting", "merge-sort", 1600);
    p112.addTestCase(TestCase("3\n101 Sarim CS 3.92\n102 Ahmed CS 3.33\n106 Ayousha SE 3.50", "1 101 Sarim CS 3.92 Dean's List\n2 106 Ayousha SE 3.50 Dean's List\n3 102 Ahmed CS 3.33 No Award"));
    pm.addProblem(p112);

    // 113: Counting Sort
    Problem p113(113, "Counting Sort",
                 "Given an array of n non-negative integers, sort them in ascending order in O(n + k) time complexity using Counting Sort.\n\nInput Format:\nLine 1: integer n\nLine 2: n non-negative integers\n\nOutput Format:\nSorted integers separated by spaces.",
                 "Easy", "Sorting", "count-sort", 1250);
    p113.addTestCase(TestCase("7\n4 2 2 8 3 3 1", "1 2 2 3 3 4 8"));
    p113.addTestCase(TestCase("5\n5 0 2 0 1", "0 0 1 2 5"));
    pm.addProblem(p113);

    // 114: Delivery Bubble Sort
    Problem p114(114, "Delivery Bubble Sort",
                 "Sort package tracking numbers using adjacent swapping (Bubble Sort) with early termination when no swaps occur. Output sorted tracking numbers, total swaps, comparisons, and passes saved.\n\nInput Format:\nLine 1: integer n\nLine 2: n package tracking numbers\n\nOutput Format:\nSorted array elements\nTotal swaps: <s>\nTotal comparisons: <c>\nPasses saved: <p>",
                 "Easy", "Sorting", "bubble-sort", 1200);
    p114.addTestCase(TestCase("5\n50 40 30 20 10", "10 20 30 40 50\nTotal swaps: 10\nTotal comparisons: 10\nPasses saved: 0"));
    p114.addTestCase(TestCase("4\n1 2 3 4", "1 2 3 4\nTotal swaps: 0\nTotal comparisons: 3\nPasses saved: 2"));
    pm.addProblem(p114);

    // 115: Delivery Arm Insertion Sorter
    Problem p115(115, "Delivery Insertion Sorter",
                 "Sort package tracking numbers using an Insertion Arm sorter. Shift larger elements right and place the key in its spot. Compute and print the total shift distance (sum of all position shifts).\n\nInput Format:\nLine 1: integer n\nLine 2: n tracking numbers\n\nOutput Format:\nSorted array elements\nTotal shift distance: <d>",
                 "Easy", "Sorting", "insertion-sort", 1250);
    p115.addTestCase(TestCase("5\n12 11 13 5 6", "5 6 11 12 13\nTotal shift distance: 6"));
    p115.addTestCase(TestCase("3\n1 2 3", "1 2 3\nTotal shift distance: 0"));
    pm.addProblem(p115);

    // 116: Minimal Swap Selection Sort
    Problem p116(116, "Minimal Swap Selection Sort",
                 "Sort package tracking numbers using Selection Sort to minimize physical crane swaps. In each pass, swap the minimum item in the unsorted section. Count total actual swaps and skipped swaps.\n\nInput Format:\nLine 1: integer n\nLine 2: n tracking numbers\n\nOutput Format:\nSorted array elements\nTotal actual swaps: <s>\nNumber of skipped swaps: <k>",
                 "Easy", "Sorting", "selection-sort", 1300);
    p116.addTestCase(TestCase("5\n64 25 12 22 11", "11 12 22 25 64\nTotal actual swaps: 4\nNumber of skipped swaps: 0"));
    p116.addTestCase(TestCase("3\n1 2 3", "1 2 3\nTotal actual swaps: 0\nNumber of skipped swaps: 2"));
    pm.addProblem(p116);

    // 117: Diminishing Distance Shell Sort
    Problem p117(117, "Delivery Shell Sort",
                 "Sort items using Shell Sort with diminishing gap sizes starting at n/2, dividing by 2 down to 1.\n\nInput Format:\nLine 1: integer n\nLine 2: n integers\n\nOutput Format:\nSorted array elements",
                 "Medium", "Sorting", "shell-sort", 1450);
    p117.addTestCase(TestCase("6\n12 34 54 2 3 1", "1 2 3 12 34 54"));
    p117.addTestCase(TestCase("5\n9 8 3 7 5", "3 5 7 8 9"));
    pm.addProblem(p117);

    // 118: Standard Merge Sort
    Problem p118(118, "Merge Sort Algorithm",
                 "Implement recursive divide-and-conquer Merge Sort to sort an array of n integers in ascending order in O(n log n) time.\n\nInput Format:\nLine 1: integer n\nLine 2: n integers\n\nOutput Format:\nSorted array: <elements separated by spaces>",
                 "Medium", "Sorting", "merge-sort", 1500);
    p118.addTestCase(TestCase("6\n8 3 5 1 9 2", "Sorted array: 1 2 3 5 8 9"));
    p118.addTestCase(TestCase("4\n10 -1 2 8", "Sorted array: -1 2 8 10"));
    pm.addProblem(p118);

    // 119: Parcel Tracking Radix Sort
    Problem p119(119, "Parcel Radix Sort",
                 "Sort parcels by numeric tracking numbers (up to 9 digits) in linear time O(n * d) using Least Significant Digit (LSD) Radix Sort with Counting Sort subroutine.\n\nInput Format:\nLine 1: integer n\nLine 2: n tracking numbers\n\nOutput Format:\nSorted tracking numbers separated by spaces.",
                 "Medium", "Sorting", "radix-sort", 1650);
    p119.addTestCase(TestCase("6\n170 45 75 90 802 24", "24 45 75 90 170 802"));
    p119.addTestCase(TestCase("4\n123456789 9 9876 54321", "9 9876 54321 123456789"));
    pm.addProblem(p119);

    // 120: Median-of-Three QuickSort
    Problem p120(120, "Median-of-Three QuickSort",
                 "Implement QuickSort using the Median-of-Three pivot selection method (comparing low, mid, and high elements) to partition and sort an array of n integers in ascending order.\n\nInput Format:\nLine 1: integer n\nLine 2: n integers\n\nOutput Format:\nSorted array: <elements separated by spaces>",
                 "Medium", "Sorting", "quick-sort", 1600);
    p120.addTestCase(TestCase("7\n8 3 5 1 9 2 7", "Sorted array: 1 2 3 5 7 8 9"));
    p120.addTestCase(TestCase("5\n10 80 30 90 40", "Sorted array: 10 30 40 80 90"));
    pm.addProblem(p120);

    // ================= BATCH 2 (121 - 139) =================

    // 121: Recursive Binary Search (BinarySearch.cpp)
    Problem p121(121, "Recursive Binary Search",
                 "Given a sorted array of n integers and a target value, find the 0-based index of target using recursive binary search. If target is not found, return -1.\n\nInput Format:\nLine 1: integers n and target\nLine 2: n space-separated sorted integers\n\nOutput Format:\n'Target found at index: <index>' or 'Target not found.'",
                 "Easy", "Binary Search", "binary-search", 1200);
    p121.addTestCase(TestCase("7 9\n2 4 7 9 12 15 20", "Target found at index: 3"));
    p121.addTestCase(TestCase("7 5\n2 4 7 9 12 15 20", "Target not found."));
    p121.addTestCase(TestCase("1 42\n42", "Target found at index: 0"));
    pm.addProblem(p121);

    // 122: Count Linked List Nodes Recursively (CountLinkedList.cpp)
    Problem p122(122, "Count Linked List Nodes",
                 "Given a singly linked list with n nodes, count and return the total number of nodes in the linked list using a recursive function.\n\nInput Format:\nLine 1: integer n\nLine 2: n integers representing the linked list\n\nOutput Format:\n'Total number of nodes: <total>'",
                 "Easy", "Linked List", "recursion", 1100);
    p122.addTestCase(TestCase("4\n10 20 30 40", "Total number of nodes: 4"));
    p122.addTestCase(TestCase("1\n99", "Total number of nodes: 1"));
    p122.addTestCase(TestCase("0", "Total number of nodes: 0"));
    pm.addProblem(p122);

    // 123: Delete Middle Element of Stack (DeleteMiddleStack.cpp)
    Problem p123(123, "Delete Middle Element of Stack",
                 "Given a stack of n integers, delete the middle element of the stack using recursion without using any additional data structures.\nMiddle element index from top is floor(n / 2).\n\nInput Format:\nLine 1: integer n\nLine 2: n space-separated integers pushed onto the stack\n\nOutput Format:\nStack after deleting middle (top to bottom).",
                 "Medium", "Stack & Queue", "recursion", 1400);
    p123.addTestCase(TestCase("5\n30 -5 18 14 -3", "-3 14 -5 30"));
    p123.addTestCase(TestCase("4\n10 20 30 40", "40 30 10"));
    p123.addTestCase(TestCase("1\n7", ""));
    pm.addProblem(p123);

    // 124: Recursive Factorial (Factorial.cpp)
    Problem p124(124, "Tail-Recursive Factorial",
                 "Compute the factorial of a non-negative integer n using tail recursion.\n\nInput Format:\nA single integer n.\n\nOutput Format:\n'Factorial: <result>'",
                 "Easy", "Basic Programming", "recursion", 1000);
    p124.addTestCase(TestCase("5", "Factorial: 120"));
    p124.addTestCase(TestCase("0", "Factorial: 1"));
    p124.addTestCase(TestCase("7", "Factorial: 5040"));
    pm.addProblem(p124);

    // 125: Insert at Bottom of Stack (InsertBottomStack.cpp)
    Problem p125(125, "Insert at Bottom of Stack",
                 "Given a stack of n elements and a value x, insert x at the very bottom of the stack using recursion, without using any loops or extra data structures.\n\nInput Format:\nLine 1: integers n and x\nLine 2: n integers in the stack (bottom to top)\n\nOutput Format:\nUpdated stack displayed from top to bottom.",
                 "Easy", "Stack & Queue", "recursion", 1250);
    p125.addTestCase(TestCase("4 5\n10 20 30 40", "40 30 20 10 5"));
    p125.addTestCase(TestCase("1 99\n10", "10 99"));
    p125.addTestCase(TestCase("0 7", "7"));
    pm.addProblem(p125);

    // 126: Recursive Linear Search (LinearSearch.cpp)
    Problem p126(126, "Recursive Linear Search",
                 "Recursively search for a target value in an array of n integers. Return the 0-based index of the first occurrence or -1 if not found.\n\nInput Format:\nLine 1: integers n and target\nLine 2: n space-separated integers\n\nOutput Format:\n'Target found at index: <index>' or 'Target not found.'",
                 "Easy", "Arrays & Hashing", "recursion", 1050);
    p126.addTestCase(TestCase("5 9\n4 7 2 9 5", "Target found at index: 3"));
    p126.addTestCase(TestCase("5 100\n4 7 2 9 5", "Target not found."));
    pm.addProblem(p126);

    // 127: Maximum Element in Array Recursively (MaximumArray.cpp)
    Problem p127(127, "Recursive Array Maximum",
                 "Given an array of n integers, find and return the maximum element using a divide-and-conquer recursive function.\n\nInput Format:\nLine 1: integer n\nLine 2: n integers\n\nOutput Format:\n'Maximum: <max>'",
                 "Easy", "Arrays & Hashing", "recursion", 1100);
    p127.addTestCase(TestCase("5\n4 7 2 9 5", "Maximum: 9"));
    p127.addTestCase(TestCase("4\n-10 -3 -20 -1", "Maximum: -1"));
    p127.addTestCase(TestCase("1\n42", "Maximum: 42"));
    pm.addProblem(p127);

    // 128: Merge Two Sorted Linked Lists (MergeSortedList.cpp)
    Problem p128(128, "Merge Sorted Lists",
                 "Given two sorted linked lists of sizes n and m, merge them into a single sorted linked list using recursion.\n\nInput Format:\nLine 1: integers n and m\nLine 2: n sorted integers (list 1)\nLine 3: m sorted integers (list 2)\n\nOutput Format:\nMerged sorted elements separated by spaces.",
                 "Easy", "Linked List", "recursion", 1250);
    p128.addTestCase(TestCase("3 3\n10 30 50\n20 40 60", "10 20 30 40 50 60"));
    p128.addTestCase(TestCase("2 1\n1 5\n2", "1 2 5"));
    p128.addTestCase(TestCase("0 2\n\n3 4", "3 4"));
    pm.addProblem(p128);

    // 129: Palindrome Linked List (PalindromeLL.cpp)
    Problem p129(129, "Palindrome Linked List",
                 "Given a singly linked list of n integers, determine whether it is a palindrome using a two-pointer recursive approach.\n\nInput Format:\nLine 1: integer n\nLine 2: n integers\n\nOutput Format:\n'Palindrome' or 'Not a palindrome'",
                 "Medium", "Linked List", "recursion", 1400);
    p129.addTestCase(TestCase("5\n1 2 3 2 1", "Palindrome"));
    p129.addTestCase(TestCase("4\n1 2 3 4", "Not a palindrome"));
    p129.addTestCase(TestCase("1\n7", "Palindrome"));
    pm.addProblem(p129);

    // 130: Recursive Queue Traversal (PrintQueue.cpp)
    Problem p130(130, "Recursive Queue Traversal",
                 "Given a FIFO queue of n integers, display all elements recursively by dequeueing and re-enqueueing each element, preserving the original order of the queue.\n\nInput Format:\nLine 1: integer n\nLine 2: n integers\n\nOutput Format:\nQueue elements in original order separated by spaces.",
                 "Easy", "Stack & Queue", "recursion", 1200);
    p130.addTestCase(TestCase("5\n10 20 30 40 50", "10 20 30 40 50"));
    p130.addTestCase(TestCase("3\n1 2 3", "1 2 3"));
    pm.addProblem(p130);

    // 131: Reverse Linked List in Groups of K (revereKLL.cpp)
    Problem p131(131, "Reverse Linked List in Groups of K",
                 "Given a singly linked list of n elements and an integer k, reverse the nodes of the list k at a time using recursion and return its modified head.\n\nInput Format:\nLine 1: integers n and k\nLine 2: n space-separated integers\n\nOutput Format:\nReversed in groups of k: <elements separated by spaces>",
                 "Hard", "Linked List", "recursion", 1750);
    p131.addTestCase(TestCase("8 3\n1 2 3 4 5 6 7 8", "3 2 1 6 5 4 8 7"));
    p131.addTestCase(TestCase("4 2\n1 2 3 4", "2 1 4 3"));
    p131.addTestCase(TestCase("5 1\n1 2 3 4 5", "1 2 3 4 5"));
    pm.addProblem(p131);

    // 132: Reverse First K Elements of Queue (reverseK.cpp)
    Problem p132(132, "Reverse First K Elements of Queue",
                 "Given an integer k and a queue of n integers, reverse the order of the first k elements of the queue, leaving the other elements in the same relative order using recursion and standard queue operations.\n\nInput Format:\nLine 1: integers n and k\nLine 2: n space-separated integers\n\nOutput Format:\nQueue after reversal: <elements separated by spaces>",
                 "Medium", "Stack & Queue", "queue", 1400);
    p132.addTestCase(TestCase("5 3\n10 20 30 40 50", "30 20 10 40 50"));
    p132.addTestCase(TestCase("4 2\n1 2 3 4", "2 1 3 4"));
    p132.addTestCase(TestCase("3 3\n5 6 7", "7 6 5"));
    pm.addProblem(p132);

    // 133: Recursive Singly Linked List Reversal (reverseList.cpp)
    Problem p133(133, "Recursive Linked List Reversal",
                 "Reverse a singly linked list of n nodes using pure recursion.\n\nInput Format:\nLine 1: integer n\nLine 2: n space-separated integers\n\nOutput Format:\nReversed linked list: <elements separated by spaces>",
                 "Easy", "Linked List", "recursion", 1200);
    p133.addTestCase(TestCase("4\n10 20 30 40", "40 30 20 10"));
    p133.addTestCase(TestCase("2\n1 2", "2 1"));
    p133.addTestCase(TestCase("1\n55", "55"));
    pm.addProblem(p133);

    // 134: Recursive Number Reversal (ReverseNumber.cpp)
    Problem p134(134, "Tail-Recursive Number Reversal",
                 "Reverse the digits of a given positive integer n using tail recursion.\n\nInput Format:\nA single integer n.\n\nOutput Format:\n'Reversed number: <reversed>'",
                 "Easy", "Basic Programming", "recursion", 1050);
    p134.addTestCase(TestCase("12345", "Reversed number: 54321"));
    p134.addTestCase(TestCase("708", "Reversed number: 807"));
    p134.addTestCase(TestCase("9", "Reversed number: 9"));
    pm.addProblem(p134);

    // 135: Reverse Stack Using Recursion (ReverseStack.cpp)
    Problem p135(135, "Reverse Stack Recursively",
                 "Reverse an entire stack of n integers using recursion and insertAtBottom helper without using loops or extra stacks.\n\nInput Format:\nLine 1: integer n\nLine 2: n integers in stack (bottom to top)\n\nOutput Format:\nReversed stack from top to bottom.",
                 "Medium", "Stack & Queue", "recursion", 1450);
    p135.addTestCase(TestCase("4\n10 20 30 40", "10 20 30 40"));
    p135.addTestCase(TestCase("3\n1 2 3", "1 2 3"));
    pm.addProblem(p135);

    // 136: Recursive Search in Queue (SearchQueue.cpp)
    Problem p136(136, "Recursive Search in Queue",
                 "Given a queue of n elements and a target integer, recursively search for target while preserving the original elements and queue order.\n\nInput Format:\nLine 1: integers n and target\nLine 2: n integers in queue\n\nOutput Format:\n'<target> found in the queue!' or '<target> not found.'",
                 "Easy", "Stack & Queue", "recursion", 1200);
    p136.addTestCase(TestCase("5 30\n10 20 30 40 50", "30 found in the queue!"));
    p136.addTestCase(TestCase("5 99\n10 20 30 40 50", "99 not found."));
    pm.addProblem(p136);

    // 137: Midpoint Split Search (DeliveryBinarySearch.cpp)
    Problem p137(137, "Midpoint Split Search",
                 "First check if a delivery conveyor array of size n is sorted. If unsorted, print 'Error: Conveyor is unsorted. Search aborted.' and return -1. Otherwise, perform binary search using mid = low + (high-low)/2.\n\nInput Format:\nLine 1: integer n\nLine 2: n tracking numbers\nLine 3: target integer\n\nOutput Format:\n'Target found at index: <index>' or 'Target not found.' or 'Error: Conveyor is unsorted. Search aborted.'",
                 "Medium", "Binary Search", "binary-search", 1350);
    p137.addTestCase(TestCase("5\n10 20 30 40 50\n30", "Target found at index: 2"));
    p137.addTestCase(TestCase("5\n10 20 30 40 50\n99", "Target not found."));
    p137.addTestCase(TestCase("3\n30 10 20\n10", "Error: Conveyor is unsorted. Search aborted."));
    pm.addProblem(p137);

    // 138: LSD Radix Sort Array (RadixSort.cpp)
    Problem p138(138, "Radix Sort Array",
                 "Sort an array of n integers using non-negative Least Significant Digit (LSD) Radix Sort with decimal counting sort passes.\n\nInput Format:\nLine 1: integer n\nLine 2: n space-separated integers\n\nOutput Format:\nSorted integers separated by spaces.",
                 "Medium", "Sorting", "radix-sort", 1550);
    p138.addTestCase(TestCase("6\n170 45 75 90 802 24", "24 45 75 90 170 802"));
    p138.addTestCase(TestCase("4\n100 20 5 3000", "5 20 100 3000"));
    pm.addProblem(p138);

    // 139: Student Marks Insertion Sort (StudentsInsertionSort.cpp)
    Problem p139(139, "Student Marks Insertion Sort",
                 "Given n student records containing Roll No., Name, and Marks, sort the students in descending order based on their marks using Insertion Sort.\n\nInput Format:\nLine 1: integer n\nNext n lines: <rollNo> <name> <marks>\n\nOutput Format:\nSorted student records in descending order of marks.",
                 "Easy", "Sorting", "insertion-sort", 1250);
    p139.addTestCase(TestCase("3\n1 Ali 85\n2 Sara 95\n3 Zain 70", "Roll Number: 2\nStudent Name: Sara\nStudent Marks: 95\n\nRoll Number: 1\nStudent Name: Ali\nStudent Marks: 85\n\nRoll Number: 3\nStudent Name: Zain\nStudent Marks: 70"));
    p139.addTestCase(TestCase("2\n10 A 50\n20 B 60", "Roll Number: 20\nStudent Name: B\nStudent Marks: 60\n\nRoll Number: 10\nStudent Name: A\nStudent Marks: 50"));
    pm.addProblem(p139);

    // Save to binary database
    StorageManager sm;
    if (sm.saveProblems(pm))
    {
        cout << "\n>>> Successfully saved " << pm.getProblemCount() << " problems to Storage/problems.dat!\n";
    }
    else
    {
        cerr << "Failed to save problems!\n";
        return 1;
    }

    return 0;
}
