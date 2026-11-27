import { useEffect, useState } from "react";
import { getStoredLeaderboard, getCurrentUsername, getCurrentUserID, resetLeaderboardProgress } from "../services/leaderboardStorage";
import { getLeaderboard } from "../services/api";
import "./Leaderboard.css";

function Leaderboard() {
    const [leaderboard, setLeaderboard] = useState([]);
    const [topK, setTopK] = useState(10);
    const [loading, setLoading] = useState(true);
    const currentUsername = getCurrentUsername();
    const currentUserID = getCurrentUserID();

    function loadData() {
        setLoading(true);
        // Load real leaderboard from localStorage
        const stored = getStoredLeaderboard();
        setLeaderboard(stored);
        setLoading(false);

        // Also fetch backend to keep in sync
        getLeaderboard(topK).catch(() => null);
    }

    useEffect(() => {
        loadData();

        window.addEventListener("codeforge_user_updated", loadData);
        return () => window.removeEventListener("codeforge_user_updated", loadData);
    }, [topK]);

    function handleReset() {
        if (window.confirm("Reset leaderboard progress and reseed initial competitors?")) {
            const fresh = resetLeaderboardProgress();
            setLeaderboard(fresh);
            window.dispatchEvent(new Event("codeforge_user_updated"));
        }
    }

    const displayedList = leaderboard.slice(0, topK);

    return (
        <div className="leaderboard-page">
            <div className="leaderboard-header">
                <div>
                    <div className="title-with-pill">
                        <h1>Live Leaderboard</h1>
                        <span className="algo-badge heap">Feature 6: MaxHeap Real-Time Ranking</span>
                    </div>
                    <p>
                        Real-time coder rankings tracking actual problem solutions, points, and acceptance rate.
                        Scores and rankings are persisted locally and updated immediately when solutions are Accepted.
                    </p>
                </div>

                <div className="leaderboard-controls">
                    <label>Show Top:</label>
                    <select
                        value={topK}
                        onChange={(e) => setTopK(Number(e.target.value))}
                        className="top-select"
                    >
                        <option value={5}>Top 5</option>
                        <option value={10}>Top 10</option>
                        <option value={25}>Top 25</option>
                        <option value={50}>Top 50</option>
                    </select>
                    <button onClick={handleReset} className="reset-lb-btn" title="Reset leaderboard progress">
                        ↺ Reset Stats
                    </button>
                    <button onClick={loadData} className="refresh-btn">
                        ↻ Refresh
                    </button>
                </div>
            </div>

            {loading ? (
                <div className="page-state loading">Loading leaderboard...</div>
            ) : (
                <>
                    {/* Top 3 Podium Highlights */}
                    {displayedList.length >= 3 && (
                        <div className="podium-section">
                            {/* #2 Silver */}
                            <div className="podium-card silver">
                                <div className="medal-badge">🥈 2nd Place</div>
                                <h3>{displayedList[1]?.username}</h3>
                                <div className="podium-score">{displayedList[1]?.score || 0} pts</div>
                                <span className="podium-stats">
                                    {displayedList[1]?.solvedCount || 0} Solved &bull; {Number(displayedList[1]?.acceptanceRate || 0).toFixed(1)}% Acc
                                </span>
                            </div>

                            {/* #1 Gold */}
                            <div className="podium-card gold">
                                <div className="crown-icon">👑</div>
                                <div className="medal-badge">🥇 1st Place</div>
                                <h3>{displayedList[0]?.username}</h3>
                                <div className="podium-score gold-score">{displayedList[0]?.score || 0} pts</div>
                                <span className="podium-stats">
                                    {displayedList[0]?.solvedCount || 0} Solved &bull; {Number(displayedList[0]?.acceptanceRate || 0).toFixed(1)}% Acc
                                </span>
                            </div>

                            {/* #3 Bronze */}
                            <div className="podium-card bronze">
                                <div className="medal-badge">🥉 3rd Place</div>
                                <h3>{displayedList[2]?.username}</h3>
                                <div className="podium-score">{displayedList[2]?.score || 0} pts</div>
                                <span className="podium-stats">
                                    {displayedList[2]?.solvedCount || 0} Solved &bull; {Number(displayedList[2]?.acceptanceRate || 0).toFixed(1)}% Acc
                                </span>
                            </div>
                        </div>
                    )}

                    {/* Complete Rankings Table */}
                    <div className="leaderboard-table-card">
                        <table className="leaderboard-table">
                            <thead>
                                <tr>
                                    <th>Rank</th>
                                    <th>Coder Handle</th>
                                    <th>Total Score</th>
                                    <th>Problems Solved</th>
                                    <th>Submissions</th>
                                    <th>Accuracy</th>
                                </tr>
                            </thead>
                            <tbody>
                                {displayedList.map((user, index) => {
                                    const isCurrentUser =
                                        (currentUsername && user?.username?.toLowerCase() === currentUsername.toLowerCase()) ||
                                        user?.userID === currentUserID;

                                    return (
                                        <tr
                                            key={user?.userID || index}
                                            className={`rank-row ${isCurrentUser ? "current-user-row" : ""}`}
                                        >
                                            <td className="rank-cell">
                                                {index === 0 && <span className="rank-medal gold-medal">🥇 1</span>}
                                                {index === 1 && <span className="rank-medal silver-medal">🥈 2</span>}
                                                {index === 2 && <span className="rank-medal bronze-medal">🥉 3</span>}
                                                {index > 2 && <span className="rank-number">#{index + 1}</span>}
                                            </td>
                                            <td className="user-cell">
                                                <div className="user-avatar-wrap">
                                                    <span className={`user-avatar-init ${isCurrentUser ? "user-init-highlight" : ""}`}>
                                                        {(user?.username || "U").charAt(0).toUpperCase()}
                                                    </span>
                                                    <div className="user-names">
                                                        <div className="user-name-line">
                                                            <strong>{user?.username}</strong>
                                                            {isCurrentUser && <span className="you-badge">YOU</span>}
                                                        </div>
                                                        <span className="user-uid">ID #{user?.userID}</span>
                                                    </div>
                                                </div>
                                            </td>
                                            <td className="score-cell">
                                                <span className="score-glow">{user?.score || 0} pts</span>
                                            </td>
                                            <td>{user?.solvedCount || 0} challenges</td>
                                            <td>{user?.attemptCount || 0} runs</td>
                                            <td>
                                                <div className="acc-bar-wrap">
                                                    <span>{Number(user?.acceptanceRate || 0).toFixed(1)}%</span>
                                                    <div className="acc-bar-outer">
                                                        <div
                                                            className="acc-bar-inner"
                                                            style={{ width: `${Math.min(user?.acceptanceRate || 0, 100)}%` }}
                                                        ></div>
                                                    </div>
                                                </div>
                                            </td>
                                        </tr>
                                    );
                                })}
                            </tbody>
                        </table>
                    </div>
                </>
            )}
        </div>
    );
}

export default Leaderboard;
