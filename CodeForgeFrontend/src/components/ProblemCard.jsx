import { Link } from "react-router-dom";
import "./ProblemCard.css";

function getNormalizedRating(problem) {
    const diff = (problem?.difficulty || "").toLowerCase();
    const raw = Number(problem?.rating || 0);
    if (raw >= 800 && raw <= 2500) return raw;
    const id = problem?.problemID || 1;
    if (diff === "hard") return 1750 + (id % 30) * 10;
    if (diff === "medium") return 1350 + (id % 25) * 10;
    return 850 + (id % 35) * 10;
}

function ProblemCard({ problem }) {
    const cleanRating = getNormalizedRating(problem);
    const cleanDiff = problem?.difficulty || (cleanRating <= 1300 ? "Easy" : cleanRating <= 1650 ? "Medium" : "Hard");
    const diffClass = cleanDiff.toLowerCase();

    return (
        <div className="problem-card">
            <div className="problem-card-main">
                <div className="problem-title-bar">
                    <span className="problem-id">#{problem.problemID}</span>
                    <Link to={`/problems/${problem.problemID}`} className="problem-title-link">
                        {problem.title}
                    </Link>
                </div>

                <p className="problem-description-excerpt">
                    {problem.description && problem.description.length > 140
                        ? problem.description.substring(0, 140) + "..."
                        : problem.description}
                </p>

                <div className="problem-tags-row">
                    <span className={`diff-pill ${diffClass}`}>
                        {cleanDiff}
                    </span>

                    <span className="topic-pill">
                        {problem.topic}
                    </span>

                    <span className="rating-pill">
                        ★ {cleanRating}
                    </span>

                    <span className="test-count-pill">
                        {problem.testCaseCount || 0} Test Cases
                    </span>
                </div>
            </div>

            <div className="problem-card-actions">
                <Link
                    to={`/problems/${problem.problemID}`}
                    className="solve-btn"
                >
                    Solve Challenge &rarr;
                </Link>
            </div>
        </div>
    );
}

export default ProblemCard;
