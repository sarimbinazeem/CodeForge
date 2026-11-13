
import { useState } from "react";
import { submitSolution } from "../services/api";
import "./CodeEditor.css";

const STARTER_CODE = `#include <iostream>
using namespace std;

int main()
{
    // Write your solution here

    return 0;
}`;

function CodeEditor({ problemID }) {

    const [sourceCode, setSourceCode] = useState(STARTER_CODE);
    const [submitting, setSubmitting] = useState(false);
    const [result, setResult] = useState(null);
    const [error, setError] = useState("");

    async function handleSubmit() {

        if (sourceCode.trim() === "") {
            setError("Please write some code before submitting.");
            return;
        }

        try {
            setSubmitting(true);
            setResult(null);
            setError("");

            const submissionID = Math.floor(Date.now() % 2000000000);

            const response = await submitSolution(
                submissionID,
                Number(problemID),
                sourceCode,
                "C++"
            );

            setResult(response);

        }
        catch (err) {
            setError(err.message);
        }
        finally {
            setSubmitting(false);
        }
    }

    function handleReset() {
        setSourceCode(STARTER_CODE);
        setResult(null);
        setError("");
    }

    return (
        <div className="code-editor">

            <div className="editor-toolbar">

                <div className="editor-language">
                    <span className="language-indicator"></span>
                    C++
                </div>

                <button
                    className="reset-button"
                    onClick={handleReset}
                    disabled={submitting}
                >
                    Reset Code
                </button>

            </div>

            <textarea
                className="editor-textarea"
                value={sourceCode}
                onChange={(event) => setSourceCode(event.target.value)}
                spellCheck="false"
                autoCapitalize="off"
                autoComplete="off"
                autoCorrect="off"
                disabled={submitting}
                placeholder="Write your C++ solution here..."
            />

            <div className="editor-footer">

                <span className="editor-hint">
                    C++17
                </span>

                <button
                    className="submit-button"
                    onClick={handleSubmit}
                    disabled={submitting}
                >
                    {submitting ? "Judging..." : "Submit Solution"}
                </button>

            </div>

            {error && (
                <div className="submission-error">
                    {error}
                </div>
            )}

            {result && (
                <div className="judge-result">

                    <div className="result-header">

                        <h3>Submission Result</h3>

                        <span
                            className={`verdict ${result.result.verdict
                                .toLowerCase()
                                .replace(/\s+/g, "-")}`}
                        >
                            {result.result.verdict}
                        </span>

                    </div>

                    <div className="result-statistics">

                        <div>
                            <span>Passed</span>
                            <strong>
                                {result.result.passedTestCases}
                            </strong>
                        </div>

                        <div>
                            <span>Total Tests</span>
                            <strong>
                                {result.result.totalTestCases}
                            </strong>
                        </div>

                        <div>
                            <span>Execution Time</span>
                            <strong>
                                Not measured
                            </strong>
                        </div>

                    </div>

                    {result.result.errorMessage && (
                        <div className="result-error-details">
                            <h4>Error Details</h4>
                            <pre>{result.result.errorMessage}</pre>
                        </div>
                    )}

                    <p className="submission-reference">
                        Submission ID: {result.submission.submissionID}
                    </p>

                </div>
            )}

        </div>
    );
}

export default CodeEditor;
