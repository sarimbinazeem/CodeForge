const API_BASE_URL = "http://localhost:8080/api";

// 1. Core Problems
export async function getProblems() {
    const response = await fetch(`${API_BASE_URL}/problems`);
    if (!response.ok) throw new Error("Failed to retrieve problems");
    return await response.json();
}

export async function getProblem(problemID) {
    const response = await fetch(`${API_BASE_URL}/problems/${problemID}`);
    if (!response.ok) throw new Error("Failed to retrieve problem");
    return await response.json();
}

// Retrieve unique topics from database
export async function getTopics() {
    try {
        const response = await fetch(`${API_BASE_URL}/topics`);
        if (!response.ok) return [];
        return await response.json();
    } catch {
        return [];
    }
}

// Feature 1: Dynamic Array — Problem Explorer
export async function exploreProblems({ difficulty = "", topic = "", sortBy = "id", asc = true, page = 1, pageSize = 6 } = {}) {
    const params = new URLSearchParams({ difficulty, topic, sortBy, asc: String(asc), page: String(page), pageSize: String(pageSize) });
    const response = await fetch(`${API_BASE_URL}/problems/explore?${params}`);
    if (!response.ok) throw new Error("Failed to explore problems");
    return await response.json();
}

// Feature 5: Hash Table — Instant Lookup
export async function lookupProblemByID(id) {
    const response = await fetch(`${API_BASE_URL}/problems/lookup/${id}`);
    if (!response.ok) throw new Error("Problem not found in hash table index");
    return await response.json();
}

export async function lookupProblemsByTag(tag) {
    const response = await fetch(`${API_BASE_URL}/problems/tag/${encodeURIComponent(tag)}`);
    if (!response.ok) throw new Error("Failed to lookup tag");
    return await response.json();
}

// 2. Submissions
export async function getSubmissions() {
    const response = await fetch(`${API_BASE_URL}/submissions`);
    if (!response.ok) throw new Error("Failed to retrieve submissions");
    return await response.json();
}

// Feature 2: Linked List — Submission History Progression
export async function getProblemProgression(problemID) {
    const response = await fetch(`${API_BASE_URL}/submissions/problem/${problemID}/progression`);
    if (!response.ok) throw new Error("Failed to load problem progression");
    return await response.json();
}

// Submit Solution (Synchronous Evaluation)
export async function submitSolution(submissionID, problemID, sourceCode, language = "C++") {
    const response = await fetch(`${API_BASE_URL}/submissions`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ submissionID, problemID, sourceCode, language })
    });
    const data = await response.json();
    if (!response.ok) throw new Error(data.error || "Submission failed");
    return data;
}

// Feature 4: Queue — Submission Processing
export async function enqueueSolution(submissionID, problemID, sourceCode, language = "C++") {
    const response = await fetch(`${API_BASE_URL}/queue/enqueue`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ submissionID, problemID, sourceCode, language })
    });
    const data = await response.json();
    if (!response.ok) throw new Error(data.error || "Failed to enqueue submission");
    return data;
}

export async function processNextQueued() {
    const response = await fetch(`${API_BASE_URL}/queue/process-next`, { method: "POST" });
    return await response.json();
}

export async function processAllQueued() {
    const response = await fetch(`${API_BASE_URL}/queue/process-all`, { method: "POST" });
    return await response.json();
}

export async function getQueueStatus() {
    const response = await fetch(`${API_BASE_URL}/queue/status`);
    return await response.json();
}

// Feature 3: Stack — Code Editor Undo/Redo
export async function editorType(code) {
    const response = await fetch(`${API_BASE_URL}/editor/type`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ code })
    });
    return await response.json();
}

export async function editorUndo() {
    const response = await fetch(`${API_BASE_URL}/editor/undo`, { method: "POST" });
    return await response.json();
}

export async function editorRedo() {
    const response = await fetch(`${API_BASE_URL}/editor/redo`, { method: "POST" });
    return await response.json();
}

export async function getEditorState() {
    const response = await fetch(`${API_BASE_URL}/editor/state`);
    return await response.json();
}

// Feature 6: Heap — Live Leaderboard
export async function getLeaderboard(top = 10) {
    const response = await fetch(`${API_BASE_URL}/leaderboard?top=${top}`);
    if (!response.ok) throw new Error("Failed to load leaderboard");
    return await response.json();
}

// Feature 7: BST — Learning Roadmap
export async function getRoadmap() {
    const response = await fetch(`${API_BASE_URL}/roadmap`);
    if (!response.ok) throw new Error("Failed to load roadmap");
    return await response.json();
}

// Feature 8: Graph — Skill Dependency System
export async function getSkills() {
    const response = await fetch(`${API_BASE_URL}/skills`);
    if (!response.ok) throw new Error("Failed to load skill dependencies");
    return await response.json();
}

export async function completeSkill(skill) {
    const response = await fetch(`${API_BASE_URL}/skills/complete`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ skill })
    });
    return await response.json();
}

export async function getRecommendedProblems() {
    const response = await fetch(`${API_BASE_URL}/skills/recommended`);
    return await response.json();
}
