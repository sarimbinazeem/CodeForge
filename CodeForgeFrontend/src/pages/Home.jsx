import { useEffect, useState } from "react";
import { Link } from "react-router-dom";
import { getLeaderboard, getRoadmap, getSkills, completeSkill, getRecommendedProblems } from "../services/api";
import { getStoredLeaderboard, getCurrentUsername } from "../services/leaderboardStorage";
import "./Home.css";

function getNormalizedRating(step) {
    const diff = (step?.difficulty || "").toLowerCase();
    const raw = Number(step?.rating || 0);
    if (raw >= 800 && raw <= 2500) return raw;
    const id = step?.problemID || 1;
    if (diff === "hard") return 1750 + (id % 30) * 10;
    if (diff === "medium") return 1350 + (id % 25) * 10;
    return 850 + (id % 35) * 10;
}

function Home() {
    const [leaderboard, setLeaderboard] = useState([]);
    const [roadmap, setRoadmap] = useState([]);
    const [skillsData, setSkillsData] = useState({ skills: [], nextRecommendedTopics: [] });
    const [recommendedProbs, setRecommendedProbs] = useState([]);
    const [loading, setLoading] = useState(true);
    const [completingSkill, setCompletingSkill] = useState("");
    const currentUsername = getCurrentUsername();

    async function loadDashboard() {
        try {
            setLoading(true);
            const [lbData, rmData, skData, recData] = await Promise.all([
                getLeaderboard(5).catch(() => ({ leaderboard: [] })),
                getRoadmap().catch(() => ({ roadmap: [] })),
                getSkills().catch(() => ({ skills: [], nextRecommendedTopics: [] })),
                getRecommendedProblems().catch(() => ({ recommendedProblems: [] }))
            ]);

            if (lbData?.leaderboard && lbData.leaderboard.length > 0) {
                setLeaderboard(lbData.leaderboard);
            } else {
                setLeaderboard(getStoredLeaderboard());
            }
            setRoadmap(Array.isArray(rmData?.roadmap) ? rmData.roadmap : []);
            setSkillsData({
                skills: Array.isArray(skData?.skills) ? skData.skills : [],
                nextRecommendedTopics: Array.isArray(skData?.nextRecommendedTopics) ? skData.nextRecommendedTopics : []
            });
            setRecommendedProbs(Array.isArray(recData?.recommendedProblems) ? recData.recommendedProblems : []);
        } catch (err) {
            console.error("Dashboard error:", err);
        } finally {
            setLoading(false);
        }
    }

    useEffect(() => {
        loadDashboard();

        window.addEventListener("codeforge_user_updated", loadDashboard);
        return () => window.removeEventListener("codeforge_user_updated", loadDashboard);
    }, []);

    async function handleCompleteSkill(skillName) {
        try {
            setCompletingSkill(skillName);
            await completeSkill(skillName);
            await loadDashboard();
        } catch (err) {
            console.error("Error completing skill:", err);
        } finally {
            setCompletingSkill("");
        }
    }

    return (
        <div className="home-dashboard">
            {/* Hero Section */}
            <header className="hero-banner">
                <div className="hero-content">
                    <h1>CodeForge Platform</h1>
                    <p>
                        High-performance C++ online judge powered by 8 manual data structures:
                        Dynamic Arrays, Doubly Linked Lists, Dual Stacks, FIFO Queues, Hash Tables,
                        MaxHeaps, Binary Search Trees, and Directed Graphs.
                    </p>
                    <div className="hero-actions">
                        <Link to="/problems" className="hero-btn primary">
                            Explore Problems &rarr;
                        </Link>
                        <Link to="/submissions" className="hero-btn secondary">
                            View Submission Queue
                        </Link>
                    </div>
                </div>
            </header>

            {loading ? (
                <div className="dashboard-loading">
                    <p>Loading CodeForge system from C++ backend...</p>
                </div>
            ) : (
                <div className="dashboard-grid">
                    {/* Live Leaderboard Card */}
                    <div className="dashboard-card leaderboard-card">
                        <div className="card-header">
                            <h2>Live Leaderboard</h2>
                            <span className="badge heap-badge">MaxHeap Priority Queue</span>
                        </div>
                        <p className="card-desc">Top coders ranked by score and solved count via custom MaxHeap priority queue.</p>

                        <div className="leaderboard-table">
                            {leaderboard.length === 0 ? (
                                <p className="empty-hint">No users registered on leaderboard yet.</p>
                            ) : (
                                leaderboard.slice(0, 5).map((u, i) => {
                                    const isMe = currentUsername && u?.username?.toLowerCase() === currentUsername.toLowerCase();
                                    return (
                                        <div key={u?.userID || i} className={`leaderboard-row ${isMe ? "current-user-row" : ""}`}>
                                            <span className={`rank-tag rank-${i + 1}`}>#{i + 1}</span>
                                            <div className="user-info">
                                                <div className="user-name-line">
                                                    <strong>{u?.username || `User_${u?.userID || i + 1}`}</strong>
                                                    {isMe && <span className="you-badge">YOU</span>}
                                                </div>
                                                <span>
                                                    {u?.solvedCount || 0} solved ({Number(u?.acceptanceRate || 0).toFixed(1)}% acc)
                                                </span>
                                            </div>
                                            <span className="user-score">{u?.score || 0} pts</span>
                                        </div>
                                    );
                                })
                            )}
                        </div>
                    </div>

                    {/* Learning Roadmap Card */}
                    <div className="dashboard-card roadmap-card">
                        <div className="card-header">
                            <h2>Learning Roadmap</h2>
                            <span className="badge bst-badge">BST In-Order Traversal</span>
                        </div>
                        <p className="card-desc">Step-by-step problem sequence ordered by difficulty rating via BST In-Order traversal.</p>

                        <div className="roadmap-steps">
                            {roadmap.length === 0 ? (
                                <p className="empty-hint">No roadmap steps found in database.</p>
                            ) : (
                                roadmap.slice(0, 5).map((step, idx) => {
                                    const cleanRating = getNormalizedRating(step);
                                    const cleanDiff = step?.difficulty || (cleanRating <= 1300 ? "Easy" : cleanRating <= 1650 ? "Medium" : "Hard");
                                    return (
                                        <Link key={step?.problemID || idx} to={`/problems/${step?.problemID}`} className="roadmap-step-item">
                                            <span className="step-num">Step {idx + 1}</span>
                                            <div className="step-details">
                                                <strong>{step?.title || "Challenge"}</strong>
                                                <span>{step?.topic || "General"} &bull; Rating {cleanRating}</span>
                                            </div>
                                            <span className={`step-diff ${cleanDiff.toLowerCase()}`}>
                                                {cleanDiff}
                                            </span>
                                        </Link>
                                    );
                                })
                            )}
                        </div>
                    </div>

                    {/* Skill Dependency System Card */}
                    <div className="dashboard-card skills-card full-width">
                        <div className="card-header">
                            <h2>Skill Dependency System</h2>
                            <span className="badge graph-badge">Directed Acyclic Graph</span>
                        </div>
                        <p className="card-desc">Prerequisite topics unlock as you progress through foundational concepts.</p>

                        <div className="skills-tree-grid">
                            {skillsData.skills && skillsData.skills.length > 0 ? (
                                skillsData.skills.map((s, idx) => (
                                    <div key={s?.name || idx} className={`skill-node ${s?.status || "locked"}`}>
                                        <div className="skill-node-top">
                                            <span className={`status-indicator ${s?.status || "locked"}`}></span>
                                            <strong>{s?.name || "Topic"}</strong>
                                        </div>
                                        <span className="skill-status-text">
                                            {s?.status === "completed" && "✓ Completed"}
                                            {s?.status === "unlocked" && "Available to Study"}
                                            {(!s?.status || s?.status === "locked") && "Locked (Prereqs required)"}
                                        </span>

                                        {s?.status === "unlocked" && (
                                            <button
                                                className="complete-skill-btn"
                                                onClick={() => handleCompleteSkill(s.name)}
                                                disabled={completingSkill === s.name}
                                            >
                                                {completingSkill === s.name ? "Unlocking..." : "Mark Complete"}
                                            </button>
                                        )}
                                    </div>
                                ))
                            ) : (
                                <p className="empty-hint">No skill graph data available.</p>
                            )}
                        </div>

                        {recommendedProbs.length > 0 && (
                            <div className="recommended-section">
                                <h3>Recommended Problems for Unlocked Skills:</h3>
                                <div className="recommended-list">
                                    {recommendedProbs.slice(0, 4).map((p) => (
                                        <Link key={p?.problemID} to={`/problems/${p?.problemID}`} className="rec-problem-chip">
                                            #{p?.problemID} {p?.title} ({p?.topic})
                                        </Link>
                                    ))}
                                </div>
                            </div>
                        )}
                    </div>
                </div>
            )}
        </div>
    );
}

export default Home;
