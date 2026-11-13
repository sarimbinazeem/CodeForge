
import { useEffect, useState } from "react";
import {
    BrowserRouter,
    Routes,
    Route,
    Link
} from "react-router-dom";

import Navbar from "./components/Navbar";
import Problems from "./pages/Problems";
import ProblemDetails from "./pages/ProblemDetails";
import Submissions from "./pages/Submissions";

import { getProblems, getSubmissions } from "./services/api";

function Home() {
    const [problemCount, setProblemCount] = useState(0);
    const [submissionCount, setSubmissionCount] = useState(0);
    const [recentSubmissions, setRecentSubmissions] = useState([]);
    const [backendStatus, setBackendStatus] = useState("Checking");

    useEffect(() => {
        async function loadDashboard() {
            try {
                const response = await fetch(
                    "http://localhost:8080/api/health"
                );

                setBackendStatus(response.ok ? "Online" : "Offline");
            }
            catch {
                setBackendStatus("Offline");
            }

            try {
                const problems = await getProblems();
                setProblemCount(problems.length);
            }
            catch {
                setProblemCount(0);
            }

            try {
                const submissions = await getSubmissions();

                setSubmissionCount(submissions.length);

                const latest = [...submissions].reverse().slice(0, 5);
                setRecentSubmissions(latest);
            }
            catch {
                setSubmissionCount(0);
                setRecentSubmissions([]);
            }
        }

        loadDashboard();
    }, []);

    function getStatusClass(status) {
        return status.toLowerCase().replace(/\s+/g, "-");
    }

    return (
        <div className="home-page">
            <style>{`
                .home-page {
                    max-width: 1200px;
                    margin: 0 auto;
                    padding: 55px 25px;
                }

                .hero {
                    padding: 65px 45px;
                    background: linear-gradient(120deg, #211416, #171717 65%);
                    border: 1px solid #382326;
                    border-radius: 14px;
                    margin-bottom: 35px;
                }

                .hero-label {
                    color: #ef4444;
                    font-size: 12px;
                    font-weight: 700;
                    letter-spacing: 2px;
                    text-transform: uppercase;
                    margin-bottom: 18px;
                }

                .hero h1 {
                    font-size: clamp(34px, 6vw, 55px);
                    line-height: 1.15;
                    margin-bottom: 20px;
                }

                .hero h1 span {
                    color: #ef4444;
                }

                .hero p {
                    max-width: 600px;
                    color: #aaa;
                    font-size: 15px;
                    line-height: 1.8;
                    margin-bottom: 28px;
                }

                .hero-actions {
                    display: flex;
                    flex-wrap: wrap;
                    gap: 12px;
                }

                .home-button {
                    display: inline-block;
                    padding: 12px 20px;
                    border-radius: 6px;
                    text-decoration: none;
                    font-size: 13px;
                    font-weight: 600;
                    transition: 0.2s;
                }

                .home-button.primary {
                    background: #b20710;
                    color: white;
                }

                .home-button.primary:hover {
                    background: #d20a15;
                }

                .home-button.secondary {
                    border: 1px solid #555;
                    color: #ddd;
                }

                .home-button.secondary:hover {
                    border-color: #aaa;
                    color: white;
                }

                .dashboard-stats {
                    display: grid;
                    grid-template-columns: repeat(3, 1fr);
                    gap: 18px;
                    margin-bottom: 40px;
                }

                .stat-card {
                    padding: 23px;
                    background: #191919;
                    border: 1px solid #303030;
                    border-radius: 9px;
                }

                .stat-card span {
                    display: block;
                    color: #999;
                    font-size: 12px;
                    margin-bottom: 12px;
                }

                .stat-card strong {
                    font-size: 27px;
                }

                .stat-card .online {
                    color: #4ade80;
                }

                .stat-card .offline {
                    color: #f87171;
                }

                .home-section {
                    margin-bottom: 42px;
                }

                .home-section-heading {
                    display: flex;
                    justify-content: space-between;
                    align-items: center;
                    gap: 15px;
                    margin-bottom: 18px;
                }

                .home-section-heading h2 {
                    font-size: 21px;
                }

                .home-section-heading a {
                    color: #ef4444;
                    text-decoration: none;
                    font-size: 13px;
                }

                .feature-grid {
                    display: grid;
                    grid-template-columns: repeat(3, 1fr);
                    gap: 18px;
                }

                .feature-card {
                    padding: 24px;
                    background: #191919;
                    border: 1px solid #303030;
                    border-radius: 9px;
                }

                .feature-icon {
                    width: 40px;
                    height: 40px;
                    display: flex;
                    justify-content: center;
                    align-items: center;
                    background: #32191c;
                    color: #ef4444;
                    border-radius: 8px;
                    font-size: 19px;
                    margin-bottom: 18px;
                }

                .feature-card h3 {
                    font-size: 16px;
                    margin-bottom: 10px;
                }

                .feature-card p {
                    color: #999;
                    font-size: 13px;
                    line-height: 1.7;
                }

                .recent-container {
                    overflow-x: auto;
                    background: #191919;
                    border: 1px solid #303030;
                    border-radius: 9px;
                }

                .recent-table {
                    width: 100%;
                    border-collapse: collapse;
                    text-align: left;
                }

                .recent-table th {
                    padding: 15px 18px;
                    background: #202020;
                    color: #999;
                    font-size: 11px;
                    text-transform: uppercase;
                }

                .recent-table td {
                    padding: 16px 18px;
                    border-top: 1px solid #2b2b2b;
                    color: #ddd;
                    font-size: 13px;
                }

                .recent-table a {
                    color: #ddd;
                    text-decoration: none;
                }

                .recent-table a:hover {
                    color: #ef4444;
                }

                .home-status {
                    display: inline-block;
                    padding: 5px 9px;
                    border-radius: 5px;
                    background: #303030;
                    font-size: 11px;
                    font-weight: 700;
                }

                .home-status.accepted {
                    background: #153522;
                    color: #4ade80;
                }

                .home-status.wrong-answer,
                .home-status.runtime-error,
                .home-status.compilation-error {
                    background: #3a1d20;
                    color: #f87171;
                }

                .home-status.pending,
                .home-status.compiling {
                    background: #3b321b;
                    color: #facc15;
                }

                .no-recent {
                    padding: 25px;
                    color: #999;
                    text-align: center;
                    font-size: 13px;
                }

                @media (max-width: 750px) {
                    .dashboard-stats,
                    .feature-grid {
                        grid-template-columns: 1fr;
                    }

                    .hero {
                        padding: 40px 25px;
                    }

                    .home-page {
                        padding: 30px 15px;
                    }
                }
            `}</style>

            <section className="hero">
                <div className="hero-label">
                    C++ Coding Practice Platform
                </div>

                <h1>
                    Build your logic.
                    <br />
                    <span>Forge your skills.</span>
                </h1>

                <p>
                    CodeForge is a coding practice platform where you can
                    explore programming problems, write C++ solutions,
                    and test your code using an automated judging system.
                </p>

                <div className="hero-actions">
                    <Link to="/problems" className="home-button primary">
                        Explore Problems
                    </Link>

                    <Link to="/submissions" className="home-button secondary">
                        View Submissions
                    </Link>
                </div>
            </section>

            <section className="dashboard-stats">
                <div className="stat-card">
                    <span>Available Problems</span>
                    <strong>{problemCount}</strong>
                </div>

                <div className="stat-card">
                    <span>Total Submissions</span>
                    <strong>{submissionCount}</strong>
                </div>

                <div className="stat-card">
                    <span>Backend Status</span>
                    <strong className={
                        backendStatus === "Online" ? "online" :
                        backendStatus === "Offline" ? "offline" : ""
                    }>
                        {backendStatus}
                    </strong>
                </div>
            </section>

            <section className="home-section">
                <div className="home-section-heading">
                    <h2>What You Can Do</h2>
                </div>

                <div className="feature-grid">
                    <div className="feature-card">
                        <div className="feature-icon">01</div>
                        <h3>Practice Problems</h3>
                        <p>
                            Browse programming challenges and understand
                            their descriptions, difficulty levels, and topics.
                        </p>
                    </div>

                    <div className="feature-card">
                        <div className="feature-icon">02</div>
                        <h3>Write C++ Solutions</h3>
                        <p>
                            Write your C++17 code in the editor and submit
                            your solution directly from a problem page.
                        </p>
                    </div>

                    <div className="feature-card">
                        <div className="feature-icon">03</div>
                        <h3>Automated Judging</h3>
                        <p>
                            Your code is compiled and executed against
                            test cases to determine its verdict.
                        </p>
                    </div>
                </div>
            </section>

            <section className="home-section">
                <div className="home-section-heading">
                    <h2>Recent Submissions</h2>
                    <Link to="/submissions">View All</Link>
                </div>

                {recentSubmissions.length === 0 ? (
                    <div className="recent-container">
                        <div className="no-recent">
                            No submissions available yet. Solve a problem
                            to see your activity here.
                        </div>
                    </div>
                ) : (
                    <div className="recent-container">
                        <table className="recent-table">
                            <thead>
                                <tr>
                                    <th>Submission ID</th>
                                    <th>Problem</th>
                                    <th>Language</th>
                                    <th>Status</th>
                                </tr>
                            </thead>

                            <tbody>
                                {recentSubmissions.map((submission) => (
                                    <tr key={submission.submissionID}>
                                        <td>#{submission.submissionID}</td>

                                        <td>
                                            <Link to={`/problems/${submission.problemID}`}>
                                                Problem #{submission.problemID}
                                            </Link>
                                        </td>

                                        <td>{submission.language}</td>

                                        <td>
                                            <span className={
                                                `home-status ${getStatusClass(submission.status)}`
                                            }>
                                                {submission.status}
                                            </span>
                                        </td>
                                    </tr>
                                ))}
                            </tbody>
                        </table>
                    </div>
                )}
            </section>
        </div>
    );
}

function App() {
    return (
        <BrowserRouter>
            <Navbar />

            <Routes>
                <Route path="/" element={<Home />} />
                <Route path="/problems" element={<Problems />} />
                <Route path="/problems/:id" element={<ProblemDetails />} />
                <Route path="/submissions" element={<Submissions />} />
            </Routes>
        </BrowserRouter>
    );
}

export default App;
