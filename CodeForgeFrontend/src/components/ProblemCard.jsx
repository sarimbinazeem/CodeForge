import { Link } from "react-router-dom";
import "./ProblemCard.css";

function ProblemCard({ problem }) {
    const diffClass = (problem.difficulty || "easy").toLowerCase();

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
                        {problem.difficulty}
                    </span>

                    <span className="topic-pill">
                        {problem.topic}
                    </span>

                    {problem.rating && (
                        <span className="rating-pill">
                            ★ {problem.rating}
                        </span>
                    )}

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
