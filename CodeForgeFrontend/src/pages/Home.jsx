import { useEffect, useState } from "react";
import { Link } from "react-router-dom";
import { getLeaderboard, getRoadmap, getSkills, completeSkill, getRecommendedProblems } from "../services/api";
import "./Home.css";

function Home() {
    const [leaderboard, setLeaderboard] = useState([]);
    const [roadmap, setRoadmap] = useState([]);
    const [skillsData, setSkillsData] = useState({ skills: [], nextRecommendedTopics: [] });
    const [recommendedProbs, setRecommendedProbs] = useState([]);
    const [loading, setLoading] = useState(true);
    const [completingSkill, setCompletingSkill] = useState("");

    async function loadDashboard() {
        try {
            setLoading(true);
            const [lbData, rmData, skData, recData] = await Promise.all([
                getLeaderboard(5).catch(() => ({ leaderboard: [] })),
                getRoadmap().catch(() => ({ roadmap: [] })),
                getSkills().catch(() => ({ skills: [], nextRecommendedTopics: [] })),
                getRecommendedProblems().catch(() => ({ recommendedProblems: [] }))
            ]);

            setLeaderboard(lbData.leaderboard || []);
            setRoadmap(rmData.roadmap || []);
            setSkillsData(skData);
            setRecommendedProbs(recData.recommendedProblems || []);
        } catch (err) {
            console.error("Dashboard error:", err);
        } finally {
            setLoading(false);
        }
    }

    useEffect(() => {
        loadDashboard();
    }, []);

    async function handleCompleteSkill(skillName) {
        try {
            setCompletingSkill(skillName);
            await completeSkill(skillName);
            await loadDashboard();
        } catch (err) {
            alert(err.message);
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
                        Real-time C++ online judge powered by 8 custom data structures:
                        Dynamic Arrays, Linked Lists, Dual Stacks, FIFO Queues, Hash Tables,
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
                <div className="dashboard-loading">Loading CodeForge system features...</div>
            ) : (
                <div className="dashboard-grid">
                    {/* Feature 6: Heap — Live Leaderboard */}
                    <div className="dashboard-card leaderboard-card">
                        <div className="card-header">
                            <h2>Live Leaderboard</h2>
                            <span className="badge heap-badge">MaxHeap Priority Queue</span>
                        </div>
                        <p className="card-desc">Top coders ranked by score and solved count.</p>

                        <div className="leaderboard-table">
                            {leaderboard.length === 0 ? (
                                <p className="empty-hint">No users registered yet.</p>
                            ) : (
                                leaderboard.map((u, i) => (
                                    <div key={u.userID} className="leaderboard-row">
                                        <span className={`rank-tag rank-${i + 1}`}>#{i + 1}</span>
                                        <div className="user-info">
                                            <strong>{u.username}</strong>
                                            <span>{u.solvedCount} solved ({u.acceptanceRate.toFixed(1)}% acc)</span>
                                        </div>
                                        <span className="user-score">{u.score} pts</span>
                                    </div>
                                ))
                            )}
                        </div>
                    </div>

                    {/* Feature 7: BST — Learning Roadmap */}
                    <div className="dashboard-card roadmap-card">
                        <div className="card-header">
                            <h2>Learning Roadmap</h2>
                            <span className="badge bst-badge">BST In-Order Traversal</span>
                        </div>
                        <p className="card-desc">Step-by-step problem sequence ordered by difficulty rating.</p>

                        <div className="roadmap-steps">
                            {roadmap.length === 0 ? (
                                <p className="empty-hint">No roadmap steps found.</p>
                            ) : (
                                roadmap.slice(0, 6).map((step) => (
                                    <Link key={step.problemID} to={`/problems/${step.problemID}`} className="roadmap-step-item">
                                        <span className="step-num">Step {step.stepNumber}</span>
                                        <div className="step-details">
                                            <strong>{step.title}</strong>
                                            <span>{step.topic} &bull; Rating {step.rating || 1000}</span>
                                        </div>
                                        <span className={`step-diff ${step.difficulty.toLowerCase()}`}>
                                            {step.difficulty}
                                        </span>
                                    </Link>
                                ))
                            )}
                        </div>
                    </div>

                    {/* Feature 8: Graph — Skill Dependency System */}
                    <div className="dashboard-card skills-card full-width">
                        <div className="card-header">
                            <h2>Skill Dependency System</h2>
                            <span className="badge graph-badge">Directed Acyclic Graph (DAG)</span>
                        </div>
                        <p className="card-desc">Prerequisite topics unlock as you progress through foundational concepts.</p>

                        <div className="skills-tree-grid">
                            {skillsData.skills.map((s) => (
                                <div key={s.name} className={`skill-node ${s.status}`}>
                                    <div className="skill-node-top">
                                        <span className={`status-indicator ${s.status}`}></span>
                                        <strong>{s.name}</strong>
                                    </div>
                                    <span className="skill-status-text">
                                        {s.status === "completed" && "✓ Completed"}
                                        {s.status === "unlocked" && "Available to Study"}
                                        {s.status === "locked" && "Locked (Prereqs required)"}
                                    </span>

                                    {s.status === "unlocked" && (
                                        <button
                                            className="complete-skill-btn"
                                            onClick={() => handleCompleteSkill(s.name)}
                                            disabled={completingSkill === s.name}
                                        >
                                            {completingSkill === s.name ? "Unlocking..." : "Mark Complete"}
                                        </button>
                                    )}
                                </div>
                            ))}
                        </div>

                        {recommendedProbs.length > 0 && (
                            <div className="recommended-section">
                                <h3>Recommended Problems for Unlocked Skills:</h3>
                                <div className="recommended-list">
                                    {recommendedProbs.slice(0, 3).map((p) => (
                                        <Link key={p.problemID} to={`/problems/${p.problemID}`} className="rec-problem-chip">
                                            #{p.problemID} {p.title} ({p.topic})
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
