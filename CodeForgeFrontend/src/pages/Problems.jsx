
import { useEffect, useState } from "react";

import { getProblems } from "../services/api";
import ProblemCard from "../components/ProblemCard";

import "./Problems.css";

function Problems() {

    const [problems, setProblems] = useState([]);
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState("");

    useEffect(() => {

        async function fetchProblems() {

            try {
                setLoading(true);
                setError("");

                const data = await getProblems();

                setProblems(data);

            }
            catch (err) {
                setError(err.message);
            }
            finally {
                setLoading(false);
            }

        }

        fetchProblems();

    }, []);

    return (
        <div className="problems-page">

            <div className="problems-header">

                <div>
                    <h1>Programming Problems</h1>

                    <p>
                        Practice your problem-solving skills
                        with C++.
                    </p>
                </div>

                <span className="problem-total">
                    {problems.length} Problems
                </span>

            </div>

            {loading && (
                <p className="loading-message">
                    Loading problems...
                </p>
            )}

            {error && (
                <div className="error-message">
                    {error}
                </div>
            )}

            {!loading && !error && problems.length === 0 && (
                <p className="empty-message">
                    No problems available yet.
                </p>
            )}

            {!loading && !error && problems.length > 0 && (

                <div className="problems-list">

                    {problems.map((problem) => (

                        <ProblemCard
                            key={problem.problemID}
                            problem={problem}
                        />

                    ))}

                </div>

            )}

        </div>
    );
}

export default Problems;
