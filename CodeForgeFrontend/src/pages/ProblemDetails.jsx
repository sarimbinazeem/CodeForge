import { useEffect, useState } from "react";
import { useParams, Link } from "react-router-dom";
import { getProblem, getProblemProgression } from "../services/api";
import CodeEditor from "../components/CodeEditor";
import "./ProblemDetails.css";

function ProblemDetails() {
    const { id } = useParams();
    const [problem, setProblem] = useState(null);
    const [progression, setProgression] = useState(null);
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState("");

    async function fetchData() {
        try {
            setLoading(true);
            setError("");

            const [problemData, progData] = await Promise.all([
                getProblem(id),
                getProblemProgression(id).catch(() => null)
            ]);

            setProblem(problemData);
            setProgression(progData);
        } catch (err) {
            setError(err.message);
        } finally {
            setLoading(false);
        }
    }

    useEffect(() => {
        fetchData();
    }, [id]);

    if (loading) {
        return (
            <div className="details-state-wrapper">
                <div className="spinner"></div>
                <p>Loading problem workspace...</p>
            </div>
        );
    }

    if (error) {
        return (
            <div className="details-state-wrapper error">
                <h2>Problem Not Found</h2>
                <p>{error}</p>
                <Link to="/problems" className="back-btn">&larr; Back to Problem Explorer</Link>
            </div>
        );
    }

    if (!problem) return null;

    return (
        <div className="problem-workspace-layout">
            {/* Top breadcrumb navigation */}
            <div className="workspace-top-bar">
                <Link to="/problems" className="workspace-back-link">
                    &larr; Problems
                </Link>
                <span className="breadcrumb-separator">/</span>
                <span className="breadcrumb-current">#{problem.problemID} {problem.title}</span>
            </div>

            <div className="workspace-split">
                {/* Left Pane: Problem Description & History */}
                <div className="workspace-left-pane">
                    <div className="problem-main-card">
                        <div className="problem-header-row">
                            <h1 className="problem-title">
                                <span className="problem-number">#{problem.problemID}.</span> {problem.title}
                            </h1>
                        </div>

                        <div className="problem-badge-row">
                            <span className={`pill-difficulty ${problem.difficulty.toLowerCase()}`}>
                                {problem.difficulty}
                            </span>
                            <span className="pill-topic">{problem.topic}</span>
                            <span className="pill-rating">★ Rating: {problem.rating || 1000}</span>
                            <span className="pill-tests">{problem.testCaseCount} Test Cases</span>
                        </div>

                        <hr className="divider" />

                        <div className="problem-body">
                            <h3>Description</h3>
                            <div className="description-text">
                                {problem.description}
                            </div>
                        </div>

                        {/* Feature 2: Linked List — Attempt Progression Timeline */}
                        {progression && progression.totalAttempts > 0 && (
                            <div className="progression-card">
                                <div className="progression-head">
                                    <h4>Your Submission History (Linked List)</h4>
                                    <span className={`status-tag ${progression.isSolved ? "solved" : "attempted"}`}>
                                        {progression.isSolved ? "✓ Accepted" : "In Progress"}
                                    </span>
                                </div>

                                <div className="progression-list">
                                    {progression.attempts.map((att, idx) => (
                                        <div key={att.submissionID || idx} className="progression-row">
                                            <span className="prog-idx">#{idx + 1}</span>
                                            <span className={`prog-verdict ${att.status.toLowerCase().replace(/\s+/g, "-")}`}>
                                                {att.status}
                                            </span>
                                            <span className="prog-lang">{att.language}</span>
                                            <span className="prog-id">Sub #{att.submissionID}</span>
                                        </div>
                                    ))}
                                </div>
                            </div>
                        )}
                    </div>
                </div>

                {/* Right Pane: VS Code Code Editor IDE */}
                <div className="workspace-right-pane">
                    <CodeEditor problemID={problem.problemID} />
                </div>
            </div>
        </div>
    );
}

export default ProblemDetails;
