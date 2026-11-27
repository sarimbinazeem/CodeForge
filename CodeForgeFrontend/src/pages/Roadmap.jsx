import { useEffect, useState } from "react";
import { Link } from "react-router-dom";
import { getRoadmap } from "../services/api";
import "./Roadmap.css";

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
            setRoadmap(data.roadmap || []);
        } catch (err) {
            setError(err.message);
        } finally {
            setLoading(false);
        }
    }

    useEffect(() => {
        fetchRoadmap();
    }, []);

    const filteredSteps = roadmap.filter((step) => {
        const rating = step.rating || 1000;
        if (filterTier === "fundamental") return rating <= 1300;
        if (filterTier === "intermediate") return rating > 1300 && rating <= 1650;
        if (filterTier === "advanced") return rating > 1650;
        return true;
    });

    return (
        <div className="roadmap-page">
            <div className="roadmap-header">
                <div>
                    <div className="title-with-pill">
                        <h1>Structured Learning Roadmap</h1>
                        <span className="algo-badge bst">Feature 7: Binary Search Tree In-Order Traversal</span>
                    </div>
                    <p>
                        A sequential curriculum path ordered from easiest to hardest challenges.
                        The sequence is generated dynamically via BST In-Order traversal on difficulty ratings,
                        guaranteeing a linear progression curve without gaps.
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
                        Fundamental (Rating &le; 1300)
                    </button>
                    <button
                        className={`tier-btn ${filterTier === "intermediate" ? "active" : ""}`}
                        onClick={() => setFilterTier("intermediate")}
                    >
                        Intermediate (1301 - 1650)
                    </button>
                    <button
                        className={`tier-btn ${filterTier === "advanced" ? "active" : ""}`}
                        onClick={() => setFilterTier("advanced")}
                    >
                        Advanced (Rating &gt; 1650)
                    </button>
                </div>
            </div>

            {loading && <div className="page-state loading"><div className="spinner"></div>Traversing Binary Search Tree...</div>}
            {error && <div className="page-state error">{error}</div>}

            {!loading && !error && (
                <div className="roadmap-timeline-grid">
                    {filteredSteps.map((step, index) => (
                        <div key={step.problemID} className="roadmap-node-card">
                            <div className="node-step-tag">
                                Step #{step.stepNumber || index + 1}
                            </div>

                            <div className="node-content">
                                <div className="node-header">
                                    <span className="node-id">#{step.problemID}</span>
                                    <span className={`diff-pill ${step.difficulty.toLowerCase()}`}>
                                        {step.difficulty}
                                    </span>
                                    <span className="node-rating">★ {step.rating || 1000}</span>
                                </div>

                                <h3 className="node-title">{step.title}</h3>
                                <p className="node-topic">{step.topic}</p>

                                <p className="node-desc">
                                    {step.description && step.description.length > 90
                                        ? step.description.substring(0, 90) + "..."
                                        : step.description}
                                </p>

                                <div className="node-footer">
                                    <Link to={`/problems/${step.problemID}`} className="node-link">
                                        Solve Step &rarr;
                                    </Link>
                                </div>
                            </div>
                        </div>
                    ))}
                </div>
            )}
        </div>
    );
}

export default Roadmap;
