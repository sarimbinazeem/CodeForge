import { useEffect, useState } from "react";
import { exploreProblems, lookupProblemByID } from "../services/api";
import ProblemCard from "../components/ProblemCard";
import "./Problems.css";

function Problems() {
    const [problems, setProblems] = useState([]);
    const [totalProblems, setTotalProblems] = useState(0);
    const [totalPages, setTotalPages] = useState(1);
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState("");

    // Feature 1: Dynamic Array Explorer Filters & Pagination
    const [difficulty, setDifficulty] = useState("");
    const [topic, setTopic] = useState("");
    const [sortBy, setSortBy] = useState("id");
    const [asc, setAsc] = useState(true);
    const [page, setPage] = useState(1);
    const pageSize = 5;

    // Feature 5: Hash Table Instant Lookup input
    const [searchID, setSearchID] = useState("");

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
                        Explore DSA challenges with dynamic filtering, sorting, pagination, and instant hash lookup.
                    </p>
                </div>
                <span className="problem-total">
                    {totalProblems} Problems Found
                </span>
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
                    >
                        <option value="">All Topics</option>
                        <option value="Arrays & Hashing">Arrays & Hashing</option>
                        <option value="Linked List">Linked List</option>
                        <option value="Stack & Queue">Stack & Queue</option>
                        <option value="Binary Search">Binary Search</option>
                        <option value="Dynamic Programming">Dynamic Programming</option>
                        <option value="Graphs">Graphs</option>
                    </select>

                    <select
                        value={sortBy}
                        onChange={(e) => { setSortBy(e.target.value); setPage(1); }}
                        className="filter-select"
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
                <p className="empty-message">No problems found matching your criteria.</p>
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
