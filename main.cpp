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


cout << "\n===== SUBMISSION STORAGE TEST =====\n";

StorageManager storage;

SubmissionManager submissionTestManager;

// Create sample submissions.

Submission s1(
    7001,
    501,
    R"(
#include <iostream>
using namespace std;

int main()
{
    int a, b;
    cin >> a >> b;
    cout << a + b;
    return 0;
}
)",
    "C++"
);

s1.setStatus("Accepted");


Submission s2(
    7002,
    502,
    R"(
#include <iostream>
using namespace std;

int main()
{
    int n;
    cin >> n;

    int arr[100];

    for (int i = 0; i < n; i++)
        cin >> arr[i];

    for (int i = n - 1; i >= 0; i--)
        cout << arr[i] << " ";

    return 0;
}
)",
    "C++"
);

s2.setStatus("Wrong Answer");


Submission s3(
    7003,
    501,
    "int main(){ return 0; }",
    "C++"
);

s3.setStatus("Compilation Error");


// Add submissions to the manager.

submissionTestManager.addSubmission(s1);
submissionTestManager.addSubmission(s2);
submissionTestManager.addSubmission(s3);


// Save submissions.

cout << "\nSaving submissions...\n";

if (storage.saveSubmission(submissionTestManager))
{
    cout << "Submissions saved successfully!\n";
}
else
{
    cout << "Unable to save submissions!\n";
}


// Create a fresh manager.

SubmissionManager loadedSubmissionManager;


// Load submissions.

cout << "\nLoading submissions...\n";

if (storage.loadSubmission(loadedSubmissionManager))
{
    cout << "Submissions loaded successfully!\n";

    cout << "\nLoaded submission count: "
         << loadedSubmissionManager.getSubmissionCount()
         << endl;

    loadedSubmissionManager.displaySubmissions();
}
else
{
    cout << "Unable to load submissions!\n";
}


    return 0;
}
