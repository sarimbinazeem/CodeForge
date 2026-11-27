import { useEffect, useState } from "react";
import { exploreProblems, lookupProblemByID, getTopics } from "../services/api";
import ProblemCard from "../components/ProblemCard";
import "./Problems.css";

const DEFAULT_TOPICS = [
    "Arrays & Hashing",
    "Backtracking",
    "Basic Programming",
    "Binary Search",
    "Binary Search Tree",
    "Dynamic Programming",
    "Graphs",
    "Heap & Priority Queue",
    "Linked List",
    "Sorting",
    "Stack & Queue"
];

function Problems() {
    const [problems, setProblems] = useState([]);
    const [totalProblems, setTotalProblems] = useState(0);
    const [totalPages, setTotalPages] = useState(1);
    const [availableTopics, setAvailableTopics] = useState(DEFAULT_TOPICS);
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState("");

    // Feature 1: Dynamic Array Explorer Filters & Pagination
    const [difficulty, setDifficulty] = useState("");
    const [topic, setTopic] = useState("");
    const [sortBy, setSortBy] = useState("id");
    const [asc, setAsc] = useState(true);
    const [page, setPage] = useState(1);
    const pageSize = 8;

    // Feature 5: Hash Table Instant Lookup input
    const [searchID, setSearchID] = useState("");

    // Fetch dynamic topics from backend
    useEffect(() => {
        async function loadTopics() {
            try {
                const fetched = await getTopics();
                if (fetched && fetched.length > 0) {
                    const merged = Array.from(new Set([...fetched, ...DEFAULT_TOPICS])).sort();
                    setAvailableTopics(merged);
                }
            } catch {
                setAvailableTopics(DEFAULT_TOPICS);
            }
        }
        loadTopics();
    }, []);

    async function fetchProblems() {
        try {
            setLoading(true);
            setError("");

            const data = await exploreProblems({
                difficulty,
                topic,
                sortBy,
                asc,
                page,
                pageSize
            });

            setProblems(data.problems || []);
            setTotalProblems(data.total || 0);
            setTotalPages(data.totalPages || 1);
        } catch (err) {
            setError(err.message);
        } finally {
            setLoading(false);
        }
    }

    useEffect(() => {
        if (!searchID.trim()) {
            fetchProblems();
        }
    }, [difficulty, topic, sortBy, asc, page]);

    // Handle O(1) Instant Hash Table Search
    async function handleInstantSearch(e) {
        e.preventDefault();
        const trimmed = searchID.trim();
        if (!trimmed) {
            fetchProblems();
            return;
        }

        try {
            setLoading(true);
            setError("");
            const pid = parseInt(trimmed, 10);
            if (!isNaN(pid)) {
                const data = await lookupProblemByID(pid);
                if (data.problem) {
                    setProblems([data.problem]);
                    setTotalProblems(1);
                    setTotalPages(1);
                }
            }
        } catch (err) {
            setError(`Hash Table Lookup: No problem found with ID #${trimmed}`);
            setProblems([]);
        } finally {
            setLoading(false);
        }
    }

    return (
        <div className="problems-page">
            <div className="problems-header">
                <div>
                    <h1>Problem Explorer</h1>
                    <p>
                        Browse all {totalProblems} DSA challenges categorized by topic, difficulty, and algorithmic paradigm.
                    </p>
                </div>
                <span className="problem-total">
                    {totalProblems} Problems Found
                </span>
            </div>

            {/* Quick Topic Chips Ribbon */}
            <div className="topic-chips-ribbon">
                <button
                    className={`topic-chip ${topic === "" ? "active" : ""}`}
                    onClick={() => { setTopic(""); setPage(1); }}
                >
                    All Topics
                </button>
                {availableTopics.map((t) => (
                    <button
                        key={t}
                        className={`topic-chip ${topic === t ? "active" : ""}`}
                        onClick={() => { setTopic(t); setPage(1); }}
                    >
                        {t}
                    </button>
                ))}
            </div>

            {/* Feature 1 & 5: Explorer Controls Toolbar */}
            <div className="explorer-toolbar">
                <form className="search-form" onSubmit={handleInstantSearch}>
                    <input
                        type="number"
                        placeholder="O(1) Hash Search (ID)..."
                        value={searchID}
                        onChange={(e) => setSearchID(e.target.value)}
                        className="search-input"
                    />
                    <button type="submit" className="search-btn">Lookup</button>
                    {searchID && (
                        <button
                            type="button"
                            className="clear-btn"
                            onClick={() => { setSearchID(""); setPage(1); fetchProblems(); }}
                        >
                            Clear
                        </button>
                    )}
                </form>

                <div className="filters-group">
                    <select
                        value={difficulty}
                        onChange={(e) => { setDifficulty(e.target.value); setPage(1); }}
                        className="filter-select"
                        title="Filter by Difficulty"
                    >
                        <option value="">All Difficulties</option>
                        <option value="Easy">Easy</option>
                        <option value="Medium">Medium</option>
                        <option value="Hard">Hard</option>
                    </select>

                    <select
                        value={topic}
                        onChange={(e) => { setTopic(e.target.value); setPage(1); }}
                        className="filter-select"
                        title="Filter by Topic"
                    >
                        <option value="">All Topics ({availableTopics.length})</option>
                        {availableTopics.map((t) => (
                            <option key={t} value={t}>
                                {t}
                            </option>
                        ))}
                    </select>

                    <select
                        value={sortBy}
                        onChange={(e) => { setSortBy(e.target.value); setPage(1); }}
                        className="filter-select"
                        title="Sort Column"
                    >
                        <option value="id">Sort by ID</option>
                        <option value="title">Sort by Title</option>
                        <option value="rating">Sort by Difficulty Rating</option>
                    </select>

                    <button
                        className="sort-order-btn"
                        onClick={() => setAsc(!asc)}
                        title="Toggle Ascending / Descending"
                    >
                        {asc ? "↑ Asc" : "↓ Desc"}
                    </button>
                </div>
            </div>

            {loading && <p className="loading-message">Loading problems from backend...</p>}

            {error && <div className="error-message">{error}</div>}

            {!loading && !error && problems.length === 0 && (
                <p className="empty-message">No problems found matching topic "{topic || "All"}".</p>
            )}

            {!loading && !error && problems.length > 0 && (
                <>
                    <div className="problems-list">
                        {problems.map((problem) => (
                            <ProblemCard key={problem.problemID} problem={problem} />
                        ))}
                    </div>

                    {/* Pagination Controls */}
                    {totalPages > 1 && (
                        <div className="pagination-bar">
                            <button
                                className="page-btn"
                                onClick={() => setPage((p) => Math.max(p - 1, 1))}
                                disabled={page === 1}
                            >
                                &larr; Previous
                            </button>
                            <span className="page-info">
                                Page {page} of {totalPages}
                            </span>
                            <button
                                className="page-btn"
                                onClick={() => setPage((p) => Math.min(p + 1, totalPages))}
                                disabled={page === totalPages}
                            >
                                Next &rarr;
                            </button>
                        </div>
                    )}
                </>
            )}
        </div>
    );
}

export default Problems;
