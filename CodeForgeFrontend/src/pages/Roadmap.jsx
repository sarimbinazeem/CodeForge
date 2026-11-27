import { useEffect, useState } from "react";
import { Link } from "react-router-dom";
import { getRoadmap } from "../services/api";
import "./Roadmap.css";

// Helper to guarantee a reasonable competitive programming rating (800 - 2100)
function getNormalizedRating(step) {
    const diff = (step?.difficulty || "").toLowerCase();
    const raw = Number(step?.rating || 0);
    // If rating is already in normal range (800 - 2500), keep it
    if (raw >= 800 && raw <= 2500) {
        return raw;
    }
    // Otherwise fallback based on difficulty
    const id = step?.problemID || 1;
    if (diff === "hard") return 1750 + (id % 30) * 10;
    if (diff === "medium") return 1350 + (id % 25) * 10;
    return 850 + (id % 35) * 10;
}

function Roadmap() {
    const [roadmap, setRoadmap] = useState([]);
    const [filterTier, setFilterTier] = useState("all");
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState("");

    async function fetchRoadmap() {
        try {
            setLoading(true);
            setError("");
            const data = await getRoadmap();
            setRoadmap(Array.isArray(data?.roadmap) ? data.roadmap : []);
        } catch (err) {
            setError(err.message);
        } finally {
            setLoading(false);
        }
    }

    useEffect(() => {
        fetchRoadmap();
    }, []);

    // Robust category counts matching by difficulty and rating
    const fundamentalCount = roadmap.filter((s) => {
        const diff = (s?.difficulty || "").toLowerCase();
        const r = getNormalizedRating(s);
        return diff === "easy" || r <= 1300;
    }).length;

    const intermediateCount = roadmap.filter((s) => {
        const diff = (s?.difficulty || "").toLowerCase();
        const r = getNormalizedRating(s);
        return diff === "medium" || (r > 1300 && r <= 1650);
    }).length;

    const advancedCount = roadmap.filter((s) => {
        const diff = (s?.difficulty || "").toLowerCase();
        const r = getNormalizedRating(s);
        return diff === "hard" || r > 1650;
    }).length;

    const filteredSteps = roadmap.filter((step) => {
        const diff = (step?.difficulty || "").toLowerCase();
        const r = getNormalizedRating(step);
        if (filterTier === "fundamental") return diff === "easy" || r <= 1300;
        if (filterTier === "intermediate") return diff === "medium" || (r > 1300 && r <= 1650);
        if (filterTier === "advanced") return diff === "hard" || r > 1650;
        return true;
    });

    return (
        <div className="roadmap-page">
            <div className="roadmap-header">
                <div>
                    <div className="title-with-pill">
                        <h1>Structured Learning Roadmap</h1>
                        <span className="algo-badge bst">BST In-Order Traversal</span>
                    </div>
                    <p>
                        A sequential curriculum path ordered from easiest to hardest challenges.
                        Generated dynamically via BST In-Order traversal on difficulty ratings to guarantee a linear progression curve.
                    </p>
                </div>

                <div className="roadmap-filter-tabs">
                    <button
                        className={`tier-btn ${filterTier === "all" ? "active" : ""}`}
                        onClick={() => setFilterTier("all")}
                    >
                        All Steps ({roadmap.length})
                    </button>
                    <button
                        className={`tier-btn ${filterTier === "fundamental" ? "active" : ""}`}
                        onClick={() => setFilterTier("fundamental")}
                    >
                        Fundamental ({fundamentalCount})
                    </button>
                    <button
                        className={`tier-btn ${filterTier === "intermediate" ? "active" : ""}`}
                        onClick={() => setFilterTier("intermediate")}
                    >
                        Intermediate ({intermediateCount})
                    </button>
                    <button
                        className={`tier-btn ${filterTier === "advanced" ? "active" : ""}`}
                        onClick={() => setFilterTier("advanced")}
                    >
                        Advanced ({advancedCount})
                    </button>
                </div>
            </div>

            {loading && <div className="page-state loading">Traversing Binary Search Tree...</div>}
            {error && <div className="page-state error">{error}</div>}

            {!loading && !error && (
                <div className="roadmap-timeline-grid">
                    {filteredSteps.map((step, index) => {
                        const cleanRating = getNormalizedRating(step);
                        const cleanDiff = step?.difficulty || (cleanRating <= 1300 ? "Easy" : cleanRating <= 1650 ? "Medium" : "Hard");

                        return (
                            <div key={step?.problemID || index} className="roadmap-node-card">
                                <div className="node-step-tag">Step #{step?.stepNumber || index + 1}</div>
                                <div className="node-content">
                                    <div className="node-header">
                                        <span className="node-id">#{step?.problemID}</span>
                                        <span className={`diff-pill ${cleanDiff.toLowerCase()}`}>{cleanDiff}</span>
                                        <span className="node-rating">★ {cleanRating}</span>
                                    </div>
                                    <h3 className="node-title">{step?.title || "Challenge"}</h3>
                                    <p className="node-topic">{step?.topic || "General"}</p>
                                    <p className="node-desc">
                                        {step?.description && step.description.length > 90
                                            ? step.description.substring(0, 90) + "..."
                                            : step?.description || "Solve this problem to progress on the roadmap."}
                                    </p>
                                    <div className="node-footer">
                                        <Link to={`/problems/${step?.problemID}`} className="node-link">
                                            Solve Step &rarr;
                                        </Link>
                                    </div>
                                </div>
                            </div>
                        );
                    })}
                </div>
            )}
        </div>
    );
}

export default Roadmap;
