
import { Link } from "react-router-dom";
import "./ProblemCard.css";

function ProblemCard({ problem }) {
    return (
        <div className="problem-card">

            <div className="problem-card-main">

                <div className="problem-title">
                    <span className="problem-id">
                        #{problem.problemID}
                    </span>

                    <h3>{problem.title}</h3>
                </div>

                <p className="problem-description">
                    {problem.description}
                </p>

                <div className="problem-tags">
                    <span className={`difficulty ${problem.difficulty.toLowerCase()}`}>
                        {problem.difficulty}
                    </span>

                    <span className="topic-tag">
                        {problem.topic}
                    </span>
                </div>

            </div>

            <div className="problem-card-side">

                <span className="test-count">
                    {problem.testCaseCount} Test Cases
                </span>

                <Link
                    to={`/problems/${problem.problemID}`}
                    className="solve-button"
                >
                    Solve Problem
                </Link>

            </div>

        </div>
    );
}

export default ProblemCard;
