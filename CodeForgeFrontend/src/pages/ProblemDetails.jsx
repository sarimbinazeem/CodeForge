
import { useEffect, useState } from "react";
import { useParams, Link } from "react-router-dom";

import { getProblem } from "../services/api";

import "./ProblemDetails.css";

function ProblemDetails() {

    // Get problem ID from the URL
    const { id } = useParams();

    const [problem, setProblem] = useState(null);
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState("");

    // Fetch the selected problem
    useEffect(() => {

        async function fetchProblem() {

            try {
                setLoading(true);
                setError("");

                const data = await getProblem(id);

                setProblem(data);
            }
            catch (err) {
                setError(err.message);
            }
            finally {
                setLoading(false);
            }

        }

        fetchProblem();

    }, [id]);

    if (loading) {
        return (
            <div className="details-message">
                Loading problem...
            </div>
        );
    }

    if (error) {
        return (
            <div className="details-message error-message">

                <h2>Unable to load problem</h2>

                <p>{error}</p>

                <Link to="/problems">
                    Back to Problems
                </Link>

            </div>
        );
    }

    if (!problem) {
        return null;
    }

    return (

        <div className="problem-details-page">

            {/* Navigation */}
            <Link to="/problems" className="back-link">
                &larr; Back to Problems
            </Link>

            {/* Problem Header */}
            <div className="details-header">

                <div className="details-title">

                    <span className="details-id">
                        Problem #{problem.problemID}
                    </span>

                    <h1>{problem.title}</h1>

                </div>

                <div className="details-tags">

                    <span
                        className={`difficulty ${problem.difficulty.toLowerCase()}`}
                    >
                        {problem.difficulty}
                    </span>

                    <span className="topic-tag">
                        {problem.topic}
                    </span>

                </div>

            </div>

            {/* Problem Description */}
            <section className="problem-section">

                <h2>Description</h2>

                <p className="problem-full-description">
                    {problem.description}
                </p>

            </section>

            {/* Problem Information */}
            <section className="problem-section">

                <h2>Problem Information</h2>

                <div className="problem-information">

                    <div className="information-item">
                        <span>Difficulty</span>
                        <strong>{problem.difficulty}</strong>
                    </div>

                    <div className="information-item">
                        <span>Topic</span>
                        <strong>{problem.topic}</strong>
                    </div>

                    <div className="information-item">
                        <span>Test Cases</span>
                        <strong>{problem.testCaseCount}</strong>
                    </div>

                </div>

            </section>

            {/* Code Editor Placeholder */}
            <section className="problem-section editor-placeholder">

                <h2>Your Solution</h2>

                <p>
                    The C++ code editor and submission functionality
                    will be available here.
                </p>

            </section>

        </div>

    );
}

export default ProblemDetails;
