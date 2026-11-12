
const API_BASE_URL = "http://localhost:8080/api";

// Retrieve all problems
export async function getProblems() {
    const response = await fetch(`${API_BASE_URL}/problems`);

    if (!response.ok) {
        throw new Error("Failed to retrieve problems");
    }

    return await response.json();
}

// Retrieve one problem
export async function getProblem(problemID) {
    const response = await fetch(
        `${API_BASE_URL}/problems/${problemID}`
    );

    if (!response.ok) {
        throw new Error("Failed to retrieve problem");
    }

    return await response.json();
}

// Retrieve submission history
export async function getSubmissions() {
    const response = await fetch(`${API_BASE_URL}/submissions`);

    if (!response.ok) {
        throw new Error("Failed to retrieve submissions");
    }

    return await response.json();
}

// Submit a solution
export async function submitSolution(
    submissionID,
    problemID,
    sourceCode,
    language = "C++"
) {
    const response = await fetch(`${API_BASE_URL}/submissions`, {
        method: "POST",

        headers: {
            "Content-Type": "application/json"
        },

        body: JSON.stringify({
            submissionID,
            problemID,
            sourceCode,
            language
        })
    });

    const data = await response.json();

    if (!response.ok) {
        throw new Error(data.error || "Submission failed");
    }

    return data;
}
