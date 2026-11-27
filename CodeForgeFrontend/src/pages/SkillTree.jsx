import { useEffect, useState } from "react";
import { Link } from "react-router-dom";
import { getSkills, completeSkill, getRecommendedProblems } from "../services/api";
import "./SkillTree.css";

function SkillTree() {
    const [skillsData, setSkillsData] = useState({ skills: [], nextRecommendedTopics: [] });
    const [recommendedProbs, setRecommendedProbs] = useState([]);
    const [loading, setLoading] = useState(true);
    const [completing, setCompleting] = useState("");
    const [error, setError] = useState("");

    async function loadTree() {
        try {
            setLoading(true);
            setError("");
            const [sk, rec] = await Promise.all([
                getSkills(),
                getRecommendedProblems().catch(() => ({ recommendedProblems: [] }))
            ]);
            setSkillsData(sk);
            setRecommendedProbs(rec.recommendedProblems || []);
        } catch (err) {
            setError(err.message);
        } finally {
            setLoading(false);
        }
    }

    useEffect(() => {
        loadTree();
    }, []);

    async function handleComplete(skillName) {
        try {
            setCompleting(skillName);
            await completeSkill(skillName);
            await loadTree();
        } catch (err) {
            alert(err.message);
        } finally {
            setCompleting("");
        }
    }

    return (
        <div className="skilltree-page">
            <div className="skilltree-header">
                <div>
                    <div className="title-with-pill">
                        <h1>Skill Dependency System</h1>
                        <span className="algo-badge graph">Feature 8: Directed Acyclic Graph (DAG)</span>
                    </div>
                    <p>
                        A topological curriculum tree modeling concept prerequisites.
                        Advanced topics unlock dynamically as foundational concepts are mastered.
                    </p>
                </div>
            </div>

            {loading && <div className="page-state loading"><div className="spinner"></div>Traversing Skill Graph...</div>}
            {error && <div className="page-state error">{error}</div>}

            {!loading && !error && (
                <>
                    {/* Unlocked Next Recommended Banner */}
                    <div className="unlocked-banner">
                        <div className="banner-left">
                            <span className="banner-badge">Next Topics to Study</span>
                            <div className="unlocked-tags">
                                {skillsData.nextRecommendedTopics && skillsData.nextRecommendedTopics.length > 0 ? (
                                    skillsData.nextRecommendedTopics.map((t) => (
                                        <span key={t} className="unlocked-chip">➔ {t}</span>
                                    ))
                                ) : (
                                    <span className="all-done">All topics completed!</span>
                                )}
                            </div>
                        </div>
                    </div>

                    {/* Skill Graph Node Cards Grid */}
                    <div className="skills-grid">
                        {skillsData.skills && skillsData.skills.map((s, idx) => (
                            <div key={s.name || idx} className={`skill-card ${s.status}`}>
                                <div className="card-top-row">
                                    <span className={`status-dot-indicator ${s.status}`}></span>
                                    <span className={`status-text ${s.status}`}>
                                        {s.status === "completed" && "✓ Completed"}
                                        {s.status === "unlocked" && "🔓 Unlocked"}
                                        {s.status === "locked" && "🔒 Locked"}
                                    </span>
                                </div>

                                <h3 className="skill-name">{s.name}</h3>

                                <div className="skill-action-area">
                                    {s.status === "unlocked" && (
                                        <button
                                            className="unlock-btn"
                                            onClick={() => handleComplete(s.name)}
                                            disabled={completing === s.name}
                                        >
                                            {completing === s.name ? "Updating Graph..." : "Mark as Mastered ✓"}
                                        </button>
                                    )}
                                    {s.status === "completed" && (
                                        <span className="completed-stamp">Mastered</span>
                                    )}
                                    {s.status === "locked" && (
                                        <span className="locked-hint">Complete prerequisites to unlock</span>
                                    )}
                                </div>
                            </div>
                        ))}
                    </div>

                    {/* Recommended Problems based on Unlocked Skills */}
                    {recommendedProbs.length > 0 && (
                        <div className="recommendations-box">
                            <h2>Recommended Challenges for Unlocked Skills</h2>
                            <div className="recs-grid">
                                {recommendedProbs.map((p) => (
                                    <div key={p.problemID} className="rec-card">
                                        <div className="rec-head">
                                            <span className="rec-id">#{p.problemID}</span>
                                            <span className={`diff-pill ${p.difficulty.toLowerCase()}`}>
                                                {p.difficulty}
                                            </span>
                                        </div>
                                        <h4>{p.title}</h4>
                                        <span className="rec-topic">{p.topic}</span>
                                        <Link to={`/problems/${p.problemID}`} className="rec-solve-btn">
                                            Solve Problem &rarr;
                                        </Link>
                                    </div>
                                ))}
                            </div>
                        </div>
                    )}
                </>
            )}
        </div>
    );
}

export default SkillTree;
