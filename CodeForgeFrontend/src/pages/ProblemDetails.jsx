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
    const [activeTab, setActiveTab] = useState("description");
    const [copiedIndex, setCopiedIndex] = useState(null);

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

    function handleCopyInput(text, index) {
        navigator.clipboard.writeText(text);
        setCopiedIndex(index);
        setTimeout(() => setCopiedIndex(null), 1800);
    }

    if (loading) {
        return (
            <div className="workspace-loading-state">
                <div className="pulse-spinner"></div>
                <p>Loading problem workspace...</p>
            </div>
        );
    }

    if (error) {
        return (
            <div className="workspace-loading-state error">
                <h2>Problem Not Found</h2>
                <p>{error}</p>
                <Link to="/problems" className="back-catalog-btn">&larr; Back to Problem Explorer</Link>
            </div>
        );
    }

    if (!problem) return null;

    return (
        <div className="problem-workspace-container">
            {/* Top Workspace Bar */}
            <div className="workspace-nav-bar">
                <div className="breadcrumb-nav">
                    <Link to="/problems" className="crumb-link">Problems</Link>
                    <span className="crumb-sep">/</span>
                    <span className="crumb-id">#{problem.problemID}</span>
                    <span className="crumb-sep">/</span>
                    <span className="crumb-title">{problem.title}</span>
                </div>

                <div className="workspace-nav-badges">
                    <span className={`diff-badge ${problem.difficulty.toLowerCase()}`}>
                        {problem.difficulty}
                    </span>
                    <span className="topic-badge">{problem.topic}</span>
                    <span className="rating-badge">★ Rating: {problem.rating || 1000}</span>
                </div>
            </div>

            {/* Split Screen IDE Workspace */}
            <div className="workspace-dual-pane">
                {/* Left Pane: Problem Description, Test Cases, and Progression */}
                <div className="left-pane-scrollable">
                    <div className="pane-tabs-header">
                        <button
                            className={`pane-tab-btn ${activeTab === "description" ? "active" : ""}`}
                            onClick={() => setActiveTab("description")}
                        >
                            Description & Examples
                        </button>
                        <button
                            className={`pane-tab-btn ${activeTab === "submissions" ? "active" : ""}`}
                            onClick={() => setActiveTab("submissions")}
                        >
                            Submissions ({progression?.totalAttempts || 0})
                        </button>
                    </div>

                    <div className="pane-content-body">
                        {activeTab === "description" && (
                            <>
                                <div className="problem-title-header">
                                    <h1>#{problem.problemID}. {problem.title}</h1>
                                </div>

                                <div className="problem-statement-box">
                                    <p className="statement-text">{problem.description}</p>
                                </div>

                                {/* Feature 1: Display Test Cases as Examples */}
                                <div className="testcases-section">
                                    <div className="testcases-header">
                                        <h3>Examples & Test Cases</h3>
                                        <span className="testcases-count-pill">
                                            {problem.testCases ? problem.testCases.length : 0} Cases Provided
                                        </span>
                                    </div>

                                    {problem.testCases && problem.testCases.length > 0 ? (
                                        problem.testCases.map((tc, idx) => (
                                            <div key={idx} className="example-card">
                                                <div className="example-card-head">
                                                    <span className="example-tag">Example {idx + 1}</span>
                                                    <button
                                                        className="copy-input-btn"
                                                        onClick={() => handleCopyInput(tc.input, idx)}
                                                        title="Copy input to clipboard"
                                                    >
                                                        {copiedIndex === idx ? "✓ Copied!" : "📋 Copy Input"}
                                                    </button>
                                                </div>

                                                <div className="example-block">
                                                    <div className="example-field">
                                                        <span className="field-label">Input:</span>
                                                        <pre className="field-pre">{tc.input}</pre>
                                                    </div>
                                                    <div className="example-field">
                                                        <span className="field-label">Expected Output:</span>
                                                        <pre className="field-pre output">{tc.expectedOutput}</pre>
                                                    </div>
                                                </div>
                                            </div>
                                        ))
                                    ) : (
                                        <p className="empty-cases-note">No test cases registered for this problem yet.</p>
                                    )}
                                </div>

                                {/* Problem Metadata & Constraints */}
                                <div className="metadata-card">
                                    <h4>Problem Specifications</h4>
                                    <div className="meta-grid">
                                        <div className="meta-item">
                                            <span>Difficulty</span>
                                            <strong className={problem.difficulty.toLowerCase()}>{problem.difficulty}</strong>
                                        </div>
                                        <div className="meta-item">
                                            <span>Category</span>
                                            <strong>{problem.topic}</strong>
                                        </div>
                                        <div className="meta-item">
                                            <span>Difficulty Rating</span>
                                            <strong>{problem.rating || 1000}</strong>
                                        </div>
                                        <div className="meta-item">
                                            <span>Execution Time Limit</span>
                                            <strong>2.0 seconds</strong>
                                        </div>
                                        <div className="meta-item">
                                            <span>Memory Limit</span>
                                            <strong>128 MB</strong>
                                        </div>
                                    </div>
                                </div>
                            </>
                        )}

                        {activeTab === "submissions" && (
                            <div className="submissions-history-tab">
                                <div className="history-tab-head">
                                    <h3>Attempt History (Linked List Traversal)</h3>
                                    <span className={`status-pill ${progression?.isSolved ? "solved" : "in-progress"}`}>
                                        {progression?.isSolved ? "✓ Solved" : "In Progress"}
                                    </span>
                                </div>

                                {progression && progression.totalAttempts > 0 ? (
                                    <div className="timeline-flow">
                                        {progression.attempts.map((att, idx) => (
                                            <div key={att.submissionID || idx} className="timeline-node">
                                                <div className="node-marker">
                                                    <span className="marker-dot"></span>
                                                    <span className="marker-line"></span>
                                                </div>
                                                <div className="node-card">
                                                    <div className="node-head">
                                                        <span className="att-label">Attempt #{idx + 1}</span>
                                                        <span className={`verdict-tag ${att.status.toLowerCase().replace(/\s+/g, "-")}`}>
                                                            {att.status}
                                                        </span>
                                                    </div>
                                                    <div className="node-details">
                                                        <span>Language: {att.language}</span>
                                                        <span>Sub ID: #{att.submissionID}</span>
                                                    </div>
                                                </div>
                                            </div>
                                        ))}
                                    </div>
                                ) : (
                                    <p className="no-history-hint">No submissions recorded for this problem yet. Write code and click "Run & Submit"!</p>
                                )}
                            </div>
                        )}
                    </div>
                </div>

                {/* Right Pane: VS Code Editor IDE */}
                <div className="workspace-right-ide">
                    <CodeEditor problemID={problem.problemID} />
                </div>
            </div>
        </div>
    );
}

export default ProblemDetails;