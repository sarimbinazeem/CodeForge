
import { useEffect, useState } from "react";
import { Link } from "react-router-dom";
import { getSubmissions } from "../services/api";
import "./Submissions.css";

function Submissions() {
    const [submissions, setSubmissions] = useState([]);
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState("");

    async function loadSubmissions() {
        try {
            setLoading(true);
            setError("");

            const data = await getSubmissions();
            setSubmissions(data);
        }
        catch (err) {
            setError(err.message);
        }
        finally {
            setLoading(false);
        }
    }

    useEffect(() => {
        loadSubmissions();
    }, []);

    function getStatusClass(status) {
        return status.toLowerCase().replace(/\s+/g, "-");
    }

    return (
        <div className="submissions-page">
            <div className="submissions-header">
                <div>
                    <h1>Submission History</h1>
                    <p>Review your previous coding submissions and their verdicts.</p>
                </div>

                <div className="submission-count">
                    <span>Total Submissions</span>
                    <strong>{submissions.length}</strong>
                </div>
            </div>

            {loading && (
                <div className="submissions-message">
                    Loading submissions...
                </div>
            )}

            {!loading && error && (
                <div className="submissions-message error">
                    <p>{error}</p>
                    <button onClick={loadSubmissions}>Try Again</button>
                </div>
            )}

            {!loading && !error && submissions.length === 0 && (
                <div className="submissions-empty">
                    <h2>No submissions yet</h2>
                    <p>
                        Your submitted solutions will appear here.
                    </p>
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
                                <th>Language</th>
                                <th>Status</th>
                            </tr>
                        </thead>

                        <tbody>
                            {submissions.map((submission) => (
                                <tr key={submission.submissionID}>
                                    <td className="submission-id">
                                        #{submission.submissionID}
                                    </td>

                                    <td>
                                        <Link
                                            to={`/problems/${submission.problemID}`}
                                            className="problem-link"
                                        >
                                            Problem #{submission.problemID}
                                        </Link>
                                    </td>

                                    <td>{submission.language}</td>

                                    <td>
                                        <span
                                            className={`submission-status ${getStatusClass(submission.status)}`}
                                        >
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
