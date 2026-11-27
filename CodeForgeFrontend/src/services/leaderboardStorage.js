// ================= REAL LEADERBOARD & USER PERSISTENCE (localStorage) =================

const STORAGE_KEY_USER = "codeforge_username";
const STORAGE_KEY_USER_ID = "codeforge_user_id";
const STORAGE_KEY_LEADERBOARD = "codeforge_live_leaderboard";

const INITIAL_COMPETITORS = [
    { userID: 104, username: "elena_algo", score: 850, solvedCount: 8, attemptCount: 9 },
    { userID: 102, username: "alex_chen", score: 650, solvedCount: 6, attemptCount: 7 },
    { userID: 105, username: "marcus_vance", score: 500, solvedCount: 5, attemptCount: 6 },
    { userID: 101, username: "sarah_j", score: 350, solvedCount: 3, attemptCount: 4 },
    { userID: 103, username: "david_kim", score: 200, solvedCount: 2, attemptCount: 4 }
];

// Initialize and get the current user's handle
export function getCurrentUsername() {
    let name = localStorage.getItem(STORAGE_KEY_USER);
    if (!name || !name.trim()) {
        return null;
    }
    return name.trim();
}

export function setCurrentUsername(name) {
    if (!name || !name.trim()) return;
    const cleanName = name.trim();
    localStorage.setItem(STORAGE_KEY_USER, cleanName);

    // Ensure user ID exists
    if (!localStorage.getItem(STORAGE_KEY_USER_ID)) {
        localStorage.setItem(STORAGE_KEY_USER_ID, String(Math.floor(Date.now() % 100000)));
    }

    // Ensure user is on the leaderboard
    syncUserToLeaderboard(cleanName);
}

export function getCurrentUserID() {
    let uid = localStorage.getItem(STORAGE_KEY_USER_ID);
    if (!uid) {
        uid = String(Math.floor(Date.now() % 100000));
        localStorage.setItem(STORAGE_KEY_USER_ID, uid);
    }
    return parseInt(uid, 10);
}

// Get the full sorted live leaderboard
export function getStoredLeaderboard() {
    let data = localStorage.getItem(STORAGE_KEY_LEADERBOARD);
    let list = [];

    if (data) {
        try {
            list = JSON.parse(data);
        } catch {
            list = [...INITIAL_COMPETITORS];
        }
    } else {
        list = [...INITIAL_COMPETITORS];
    }

    // Ensure active user is present
    const username = getCurrentUsername();
    if (username) {
        const uid = getCurrentUserID();
        const existingIdx = list.findIndex((u) => u.userID === uid || u.username.toLowerCase() === username.toLowerCase());
        if (existingIdx === -1) {
            list.push({
                userID: uid,
                username: username,
                score: 0,
                solvedCount: 0,
                attemptCount: 0
            });
        } else {
            list[existingIdx].username = username;
        }
    }

    // Sort by Score DESC, then Solved Count DESC, then Attempts ASC
    list.sort((a, b) => {
        if (b.score !== a.score) return b.score - a.score;
        if (b.solvedCount !== a.solvedCount) return b.solvedCount - a.solvedCount;
        return a.attemptCount - b.attemptCount;
    });

    // Compute acceptance rates and ranks
    list = list.map((u, idx) => ({
        ...u,
        rank: idx + 1,
        acceptanceRate: u.attemptCount > 0 ? (u.solvedCount / u.attemptCount) * 100 : 0
    }));

    localStorage.setItem(STORAGE_KEY_LEADERBOARD, JSON.stringify(list));
    return list;
}

function syncUserToLeaderboard(username) {
    const list = getStoredLeaderboard();
    const uid = getCurrentUserID();
    const idx = list.findIndex((u) => u.userID === uid);
    if (idx !== -1) {
        list[idx].username = username;
        localStorage.setItem(STORAGE_KEY_LEADERBOARD, JSON.stringify(list));
    }
}

// Record an actual submission for the active user
export function recordActualSubmission(isAccepted, difficulty = "Easy") {
    let username = getCurrentUsername();
    if (!username) {
        username = "GuestCoder";
        setCurrentUsername(username);
    }

    const uid = getCurrentUserID();
    const list = getStoredLeaderboard();
    let userIdx = list.findIndex((u) => u.userID === uid || u.username.toLowerCase() === username.toLowerCase());

    if (userIdx === -1) {
        list.push({
            userID: uid,
            username: username,
            score: 0,
            solvedCount: 0,
            attemptCount: 0
        });
        userIdx = list.length - 1;
    }

    list[userIdx].attemptCount = (list[userIdx].attemptCount || 0) + 1;

    let pointsAwarded = 0;
    if (isAccepted) {
        list[userIdx].solvedCount = (list[userIdx].solvedCount || 0) + 1;
        const diffLower = (difficulty || "easy").toLowerCase();
        if (diffLower === "hard") pointsAwarded = 300;
        else if (diffLower === "medium") pointsAwarded = 200;
        else pointsAwarded = 100;

        list[userIdx].score = (list[userIdx].score || 0) + pointsAwarded;
    }

    localStorage.setItem(STORAGE_KEY_LEADERBOARD, JSON.stringify(list));

    // Return the updated leaderboard and user stats
    const updatedList = getStoredLeaderboard();
    const updatedUser = updatedList.find((u) => u.userID === uid);

    return {
        pointsAwarded,
        user: updatedUser,
        newRank: updatedUser ? updatedUser.rank : 1,
        leaderboard: updatedList
    };
}

// Get the current user's profile and rank
export function getUserProfile() {
    const username = getCurrentUsername();
    if (!username) return null;
    const uid = getCurrentUserID();
    const list = getStoredLeaderboard();
    const user = list.find((u) => u.userID === uid || u.username.toLowerCase() === username.toLowerCase());
    return user || {
        userID: uid,
        username: username,
        score: 0,
        solvedCount: 0,
        attemptCount: 0,
        acceptanceRate: 0,
        rank: list.length + 1
    };
}

// Reset progress for testing
export function resetLeaderboardProgress() {
    localStorage.removeItem(STORAGE_KEY_LEADERBOARD);
    return getStoredLeaderboard();
}
