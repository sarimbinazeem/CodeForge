import { useState, useEffect } from "react";
import { Link, NavLink } from "react-router-dom";
import { getCurrentUsername, setCurrentUsername, getUserProfile } from "../services/leaderboardStorage";
import "./Navbar.css";

function Navbar() {
    const [username, setUsername] = useState("");
    const [userProfile, setUserProfile] = useState(null);
    const [showNameModal, setShowNameModal] = useState(false);
    const [inputName, setInputName] = useState("");

    function refreshUser() {
        const name = getCurrentUsername();
        if (name) {
            setUsername(name);
            setUserProfile(getUserProfile());
        } else {
            setShowNameModal(true);
        }
    }

    useEffect(() => {
        refreshUser();

        // Listen for storage changes when submissions are made
        window.addEventListener("codeforge_user_updated", refreshUser);
        return () => window.removeEventListener("codeforge_user_updated", refreshUser);
    }, []);

    function handleSaveName(e) {
        e.preventDefault();
        if (!inputName.trim()) return;
        setCurrentUsername(inputName);
        setShowNameModal(false);
        refreshUser();
        window.dispatchEvent(new Event("codeforge_user_updated"));
    }

    return (
        <>
            <nav className="navbar">
                <Link to="/" className="navbar-logo">
                    <span className="logo-bracket">{"<"}</span>
                    CodeForge
                    <span className="logo-bracket">{"/>"}</span>
                </Link>

                <div className="navbar-links">
                    <NavLink to="/" className={({ isActive }) => isActive ? "nav-link active" : "nav-link"} end>
                        Dashboard
                    </NavLink>

                    <NavLink to="/problems" className={({ isActive }) => isActive ? "nav-link active" : "nav-link"}>
                        Problems
                    </NavLink>

                    <NavLink to="/leaderboard" className={({ isActive }) => isActive ? "nav-link active" : "nav-link"}>
                        Leaderboard
                    </NavLink>

                    <NavLink to="/roadmap" className={({ isActive }) => isActive ? "nav-link active" : "nav-link"}>
                        Roadmap
                    </NavLink>

                    <NavLink to="/skills" className={({ isActive }) => isActive ? "nav-link active" : "nav-link"}>
                        Skill Tree
                    </NavLink>

                    <NavLink to="/submissions" className={({ isActive }) => isActive ? "nav-link active" : "nav-link"}>
                        Submissions
                    </NavLink>
                </div>

                <div className="navbar-right">
                    {username ? (
                        <button
                            className="user-profile-badge"
                            onClick={() => { setInputName(username); setShowNameModal(true); }}
                            title="Click to change your username"
                        >
                            <span className="user-icon"></span>
                            <span className="user-handle">{username}</span>
                            <span className="user-pts">{userProfile?.score || 0} pts</span>
                            <span className="user-rank-tag">#{userProfile?.rank || 1}</span>
                        </button>
                    ) : (
                        <button
                            className="set-name-btn"
                            onClick={() => setShowNameModal(true)}
                        >
                            + Set Username
                        </button>
                    )}

                    <div className="api-online-pill">
                        <span className="status-dot"></span>
                        <span className="api-text">API Online</span>
                    </div>
                </div>
            </nav>

            {/* Prompt User for Name Modal */}
            {showNameModal && (
                <div className="name-modal-overlay">
                    <div className="name-modal-card">
                        <div className="modal-head">
                            <h3>👋 Enter Your Coder Handle</h3>
                            <button className="close-btn" onClick={() => setShowNameModal(false)}>&times;</button>
                        </div>
                        <p className="modal-desc">
                            Your username is saved in <code>localStorage</code> to track your actual score,
                            problem submissions, and live rank on the real leaderboard.
                        </p>

                        <form onSubmit={handleSaveName} className="modal-form">
                            <input
                                type="text"
                                placeholder="Enter your username (e.g. Sarim)..."
                                value={inputName}
                                onChange={(e) => setInputName(e.target.value)}
                                className="modal-input"
                                autoFocus
                                required
                            />
                            <div className="modal-actions">
                                <button type="button" className="modal-cancel" onClick={() => setShowNameModal(false)}>
                                    Cancel
                                </button>
                                <button type="submit" className="modal-submit">
                                    Save & Join Leaderboard
                                </button>
                            </div>
                        </form>
                    </div>
                </div>
            )}
        </>
    );
}

export default Navbar;
