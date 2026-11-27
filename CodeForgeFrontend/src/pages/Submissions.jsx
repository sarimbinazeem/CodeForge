import { useEffect, useState } from "react";
import { Link } from "react-router-dom";
import { getSubmissions, getQueueStatus, processNextQueued, processAllQueued } from "../services/api";
import "./Submissions.css";

function Submissions() {
    const [submissions, setSubmissions] = useState([]);
    const [queueStatus, setQueueStatus] = useState({ queueSize: 0, isEmpty: true });
    const [loading, setLoading] = useState(true);
    const [processing, setProcessing] = useState(false);
    const [error, setError] = useState("");

    async function loadData() {
        try {
            setLoading(true);
            setError("");
            const [subs, q] = await Promise.all([
                getSubmissions(),
                getQueueStatus().catch(() => ({ queueSize: 0, isEmpty: true }))
            ]);
            setSubmissions(subs);
            setQueueStatus(q);
        } catch (err) {
            setError(err.message);
        } finally {
            setLoading(false);
        }
    }

    useEffect(() => {
        loadData();
    }, []);

    async function handleProcessNext() {
        try {
            setProcessing(true);
            await processNextQueued();
            await loadData();
        } catch (err) {
            setError(err.message);
        } finally {
            setProcessing(false);
        }
    }

    async function handleProcessAll() {
        try {
            setProcessing(true);
            await processAllQueued();
            await loadData();
        } catch (err) {
            setError(err.message);
        } finally {
            setProcessing(false);
        }
    }

    function getStatusClass(status) {
        return status.toLowerCase().replace(/\s+/g, "-");
    }

    return (
        <div className="submissions-page">
            <div className="submissions-header">
                <div>
                    <h1>Submission History & Judge Queue</h1>
                    <p>Review evaluations or process queued solutions in FIFO order.</p>
                </div>

                <div className="header-stats-group">
                    <div className="submission-count">
                        <span>Total Records</span>
                        <strong>{submissions.length}</strong>
                    </div>
                </div>
            </div>

            {/* Feature 4: Queue — Submission Processing Control Panel */}
            <div className="queue-control-card">
                <div className="queue-info">
                    <span className="queue-label">Evaluation Queue Status (FIFO):</span>
                    <strong className="queue-badge">
                        {queueStatus.queueSize} Submissions Pending
                    </strong>
                </div>

                <div className="queue-actions">
                    <button
                        className="queue-btn"
                        onClick={handleProcessNext}
                        disabled={queueStatus.isEmpty || processing}
                    >
                        {processing ? "Evaluating..." : "Evaluate Next (FIFO)"}
                    </button>
                    <button
                        className="queue-btn primary"
                        onClick={handleProcessAll}
                        disabled={queueStatus.isEmpty || processing}
                    >
                        {processing ? "Evaluating..." : "Evaluate All in Queue"}
                    </button>
                    <button className="queue-btn refresh" onClick={loadData}>
                        ↻ Refresh
                    </button>
                </div>
            </div>

            {loading && <div className="submissions-message">Loading submissions...</div>}

            {!loading && error && (
                <div className="submissions-message error">
                    <p>{error}</p>
                    <button onClick={loadData}>Try Again</button>
                </div>
            )}

            {!loading && !error && submissions.length === 0 && (
                <div className="submissions-empty">
                    <h2>No submissions recorded yet</h2>
                    <p>Your submitted solutions will appear here in chronological order.</p>
                    <Link to="/problems">Explore Problems</Link>
                </div>
            )}

            {!loading && !error && submissions.length > 0 && (
                <div className="submissions-table-container">
                    <table className="submissions-table">
                        <thead>
                            <tr>
                                <th>Submission ID</th>
                                <th>Problem</th>
                                <th>User</th>
                                <th>Language</th>
                                <th>Status / Verdict</th>
                            </tr>
                        </thead>

                        <tbody>
                            {submissions.map((submission) => (
                                <tr key={submission.submissionID}>
                                    <td className="submission-id">#{submission.submissionID}</td>
                                    <td>
                                        <Link
                                            to={`/problems/${submission.problemID}`}
                                            className="problem-link"
                                        >
                                            Problem #{submission.problemID}
                                        </Link>
                                    </td>
                                    <td>{submission.username || "Guest"}</td>
                                    <td>{submission.language}</td>
                                    <td>
                                        <span className={`submission-status ${getStatusClass(submission.status)}`}>
                                            {submission.status}
                                        </span>
                                    </td>
                                </tr>
                            ))}
                        </tbody>
                    </table>
                </div>
            )}
        </div>
    );
}

export default Submissions;
