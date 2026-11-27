// ================= REAL LEADERBOARD & USER PERSISTENCE (localStorage & C++ Server) =================
import { registerUserOnServer } from "./api";

const STORAGE_KEY_USER = "codeforge_username";
const STORAGE_KEY_USER_ID = "codeforge_user_id";
const STORAGE_KEY_LEADERBOARD = "codeforge_live_leaderboard";

// List of any previously seeded fake names to strictly purge
const FAKE_NAMES = new Set([
    "elena_algo",
    "alex_chen",
    "marcus_vance",
    "sarah_j",
    "david_kim",
    "alex_code",
    "sarah_dsa",
    "kevin_cpp",
    "coder_pro"
]);

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
    const oldName = localStorage.getItem(STORAGE_KEY_USER);
    localStorage.setItem(STORAGE_KEY_USER, cleanName);

    // Register with central C++ server for cross-device shared leaderboard
    registerUserOnServer(cleanName).then((res) => {
        if (res?.userID) {
            localStorage.setItem(STORAGE_KEY_USER_ID, String(res.userID));
        }
    }).catch(() => null);

    // Ensure user ID exists
    if (!localStorage.getItem(STORAGE_KEY_USER_ID)) {
        localStorage.setItem(STORAGE_KEY_USER_ID, String(Math.floor(Date.now() % 100000)));
    }

    // Rename existing user entry in leaderboard rather than duplicating
    const uid = getCurrentUserID();
    let data = localStorage.getItem(STORAGE_KEY_LEADERBOARD);
    if (data) {
        try {
            let list = JSON.parse(data);
            const idx = list.findIndex(
                (u) => u.userID === uid || (oldName && u.username.toLowerCase() === oldName.toLowerCase())
            );
            if (idx !== -1) {
                list[idx].username = cleanName;
                list[idx].userID = uid;
                localStorage.setItem(STORAGE_KEY_LEADERBOARD, JSON.stringify(list));
            }
        } catch {}
    }

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

// Get the full sorted live leaderboard - contains ONLY real users
export function getStoredLeaderboard() {
    let data = localStorage.getItem(STORAGE_KEY_LEADERBOARD);
    let list = [];

    if (data) {
        try {
            list = JSON.parse(data);
        } catch {
            list = [];
        }
    }

    // Purge any old fake demo names
    list = list.filter((u) => !FAKE_NAMES.has((u.username || "").toLowerCase()));

    // Ensure active user is present
    const username = getCurrentUsername();
    if (username) {
        const uid = getCurrentUserID();
        const existingIdx = list.findIndex(
            (u) => u.userID === uid || u.username.toLowerCase() === username.toLowerCase()
        );
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

    // Compute acceptance rates and real ranks
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

// Remove a single user entry (e.g. duplicate or old test handle)
export function deleteUserFromLeaderboard(userID) {
    let data = localStorage.getItem(STORAGE_KEY_LEADERBOARD);
    if (!data) return getStoredLeaderboard();
    try {
        let list = JSON.parse(data);
        list = list.filter((u) => u.userID !== userID);
        localStorage.setItem(STORAGE_KEY_LEADERBOARD, JSON.stringify(list));
    } catch {}
    return getStoredLeaderboard();
}

// Reset progress for testing
export function resetLeaderboardProgress() {
    localStorage.removeItem(STORAGE_KEY_LEADERBOARD);
    return getStoredLeaderboard();
}
