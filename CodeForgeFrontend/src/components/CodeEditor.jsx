import { useState, useRef, useEffect } from "react";
import { submitSolution, enqueueSolution } from "../services/api";
import { getCurrentUsername, getCurrentUserID, recordActualSubmission } from "../services/leaderboardStorage";
import "./CodeEditor.css";

const STARTER_CODE = `#include <iostream>
using namespace std;

int main()
{
    // Write your C++ solution here
    
    return 0;
}
`;

function CodeEditor({ problemID }) {
    const [sourceCode, setSourceCode] = useState(STARTER_CODE);
    const [submitting, setSubmitting] = useState(false);
    const [result, setResult] = useState(null);
    const [error, setError] = useState("");
    const [queueMessage, setQueueMessage] = useState("");

    // Feature 3: Stack-based Undo / Redo
    const [undoStack, setUndoStack] = useState([]);
    const [redoStack, setRedoStack] = useState([]);

    // VS Code Cursor & Status Info
    const [cursorPos, setCursorPos] = useState({ line: 1, col: 1 });
    const textareaRef = useRef(null);
    const lineNumbersRef = useRef(null);

    // Sync scroll between textarea and line numbers gutter
    function handleScroll() {
        if (textareaRef.current && lineNumbersRef.current) {
            lineNumbersRef.current.scrollTop = textareaRef.current.scrollTop;
        }
    }

    // Update Line/Column indicator based on cursor selection
    function updateCursorPosition() {
        if (!textareaRef.current) return;
        const text = textareaRef.current.value;
        const selStart = textareaRef.current.selectionStart;
        const lines = text.substring(0, selStart).split("\n");
        const line = lines.length;
        const col = lines[lines.length - 1].length + 1;
        setCursorPos({ line, col });
    }

    // Push snapshot to undo stack
    function recordState(newCode) {
        setUndoStack((prev) => [...prev, sourceCode]);
        setRedoStack([]);
        setSourceCode(newCode);
    }

    function handleUndo() {
        if (undoStack.length === 0) return;
        const previousState = undoStack[undoStack.length - 1];
        setRedoStack((prev) => [...prev, sourceCode]);
        setUndoStack((prev) => prev.slice(0, prev.length - 1));
        setSourceCode(previousState);
    }

    function handleRedo() {
        if (redoStack.length === 0) return;
        const nextState = redoStack[redoStack.length - 1];
        setUndoStack((prev) => [...prev, sourceCode]);
        setRedoStack((prev) => prev.slice(0, prev.length - 1));
        setSourceCode(nextState);
    }

    // ================= VS CODE EDITING LOGIC =================
    function handleKeyDown(e) {
        const textarea = textareaRef.current;
        if (!textarea) return;

        const { selectionStart, selectionEnd, value } = textarea;

        // 1. Tab & Shift+Tab: Indent / Unindent 4 spaces
        if (e.key === "Tab") {
            e.preventDefault();

            if (!e.shiftKey) {
                // Insert 4 spaces at cursor
                const newValue = value.substring(0, selectionStart) + "    " + value.substring(selectionEnd);
                recordState(newValue);
                setTimeout(() => {
                    textarea.selectionStart = textarea.selectionEnd = selectionStart + 4;
                    updateCursorPosition();
                }, 0);
            } else {
                // Unindent if line starts with spaces
                const lineStart = value.lastIndexOf("\n", selectionStart - 1) + 1;
                if (value.substring(lineStart, lineStart + 4) === "    ") {
                    const newValue = value.substring(0, lineStart) + value.substring(lineStart + 4);
                    recordState(newValue);
                    setTimeout(() => {
                        textarea.selectionStart = textarea.selectionEnd = Math.max(lineStart, selectionStart - 4);
                        updateCursorPosition();
                    }, 0);
                }
            }
            return;
        }

        // 2. Enter: Smart Auto-Indentation & Bracket Expansion
        if (e.key === "Enter") {
            e.preventDefault();

            // Find current line and its leading indentation
            const lineStart = value.lastIndexOf("\n", selectionStart - 1) + 1;
            const currentLine = value.substring(lineStart, selectionStart);
            const indentMatch = currentLine.match(/^[ \t]*/);
            let indent = indentMatch ? indentMatch[0] : "";

            const prevChar = value[selectionStart - 1];
            const nextChar = value[selectionStart];

            // If pressing Enter between { and }
            if (prevChar === "{" && nextChar === "}") {
                const innerIndent = indent + "    ";
                const insertText = "\n" + innerIndent + "\n" + indent;
                const newValue = value.substring(0, selectionStart) + insertText + value.substring(selectionEnd);
                recordState(newValue);
                setTimeout(() => {
                    textarea.selectionStart = textarea.selectionEnd = selectionStart + 1 + innerIndent.length;
                    updateCursorPosition();
                }, 0);
                return;
            }

            // If previous line ended with {, add 4 spaces
            if (currentLine.trimEnd().endsWith("{")) {
                indent += "    ";
            }

            const insertText = "\n" + indent;
            const newValue = value.substring(0, selectionStart) + insertText + value.substring(selectionEnd);
            recordState(newValue);
            setTimeout(() => {
                textarea.selectionStart = textarea.selectionEnd = selectionStart + insertText.length;
                updateCursorPosition();
            }, 0);
            return;
        }

        // 3. Auto-close brackets and quotes
        const pairMap = {
            "(": ")",
            "[": "]",
            "{": "}",
            "\"": "\"",
            "'": "'"
        };

        if (pairMap[e.key]) {
            const closing = pairMap[e.key];
            const nextChar = value[selectionStart];

            // If already at closing quote/bracket, just step over
            if ((e.key === "\"" || e.key === "'") && nextChar === e.key) {
                e.preventDefault();
                textarea.selectionStart = textarea.selectionEnd = selectionStart + 1;
                updateCursorPosition();
                return;
            }

            e.preventDefault();
            const selectedText = value.substring(selectionStart, selectionEnd);
            const insertText = e.key + selectedText + closing;
            const newValue = value.substring(0, selectionStart) + insertText + value.substring(selectionEnd);
            recordState(newValue);
            setTimeout(() => {
                textarea.selectionStart = textarea.selectionEnd = selectionStart + 1;
                updateCursorPosition();
            }, 0);
            return;
        }

        // 4. Step over closing brackets: ), ], }
        if (e.key === ")" || e.key === "]" || e.key === "}") {
            if (value[selectionStart] === e.key) {
                e.preventDefault();
                textarea.selectionStart = textarea.selectionEnd = selectionStart + 1;
                updateCursorPosition();
                return;
            }
        }

        // 5. Backspace auto-deletes matching empty bracket/quote pair
        if (e.key === "Backspace" && selectionStart === selectionEnd) {
            const prevChar = value[selectionStart - 1];
            const nextChar = value[selectionStart];
            const pairs = { "(": ")", "[": "]", "{": "}", "\"": "\"", "'": "'" };

            if (pairs[prevChar] === nextChar) {
                e.preventDefault();
                const newValue = value.substring(0, selectionStart - 1) + value.substring(selectionStart + 1);
                recordState(newValue);
                setTimeout(() => {
                    textarea.selectionStart = textarea.selectionEnd = selectionStart - 1;
                    updateCursorPosition();
                }, 0);
                return;
            }
        }

        // 6. Keyboard Shortcuts: Ctrl+Z (Undo), Ctrl+Y (Redo), Ctrl+Enter (Run)
        if ((e.ctrlKey || e.metaKey) && e.key === "z") {
            e.preventDefault();
            if (e.shiftKey) handleRedo();
            else handleUndo();
            return;
        }

        if ((e.ctrlKey || e.metaKey) && e.key === "y") {
            e.preventDefault();
            handleRedo();
            return;
        }

        if ((e.ctrlKey || e.metaKey) && e.key === "Enter") {
            e.preventDefault();
            handleSubmit();
            return;
        }
    }

    async function handleSubmit() {
        if (sourceCode.trim() === "") {
            setError("Please write some code before submitting.");
            return;
        }

        try {
            setSubmitting(true);
            setResult(null);
            setError("");
            setQueueMessage("");

            const submissionID = Math.floor(Date.now() % 2000000000);
            const uname = getCurrentUsername() || "Coder";
            const uid = getCurrentUserID();

            const response = await submitSolution(
                submissionID,
                Number(problemID),
                sourceCode,
                "C++",
                uid,
                uname
            );

            setResult(response);

            // Record actual submission in localStorage live leaderboard
            const isAccepted = response?.result?.verdict === "Accepted";
            const update = recordActualSubmission(isAccepted);
            window.dispatchEvent(new Event("codeforge_user_updated"));

            if (isAccepted) {
                setQueueMessage(`🎉 Accepted! +${update.pointsAwarded} Points! You are now ranked #${update.newRank} on the Live Leaderboard!`);
            }
        } catch (err) {
            setError(err.message);
        } finally {
            setSubmitting(false);
        }
    }

    async function handleEnqueue() {
        if (sourceCode.trim() === "") {
            setError("Please write some code before submitting.");
            return;
        }

        try {
            setSubmitting(true);
            setError("");
            setResult(null);

            const submissionID = Math.floor(Date.now() % 2000000000);
            const uname = getCurrentUsername() || "Coder";
            const uid = getCurrentUserID();

            const response = await enqueueSolution(
                submissionID,
                Number(problemID),
                sourceCode,
                "C++",
                uid,
                uname
            );

            setQueueMessage(`Submission #${response.submissionID} placed into FIFO Judge Queue! [Queue Size: ${response.queueSize}]`);
        } catch (err) {
            setError(err.message);
        } finally {
            setSubmitting(false);
        }
    }

    function handleReset() {
        setUndoStack((prev) => [...prev, sourceCode]);
        setRedoStack([]);
        setSourceCode(STARTER_CODE);
        setResult(null);
        setError("");
        setQueueMessage("");
    }

    // Generate line numbers
    const totalLines = sourceCode.split("\n").length;
    const lineNumbers = Array.from({ length: totalLines }, (_, i) => i + 1);

    return (
        <div className="vscode-editor-container">
            {/* VS Code Tab Bar */}
            <div className="vscode-tab-bar">
                <div className="vscode-tab active">
                    <span className="file-icon cpp">C++</span>
                    <span className="file-name">solution.cpp</span>
                    <span className="tab-close">&times;</span>
                </div>

                <div className="vscode-tab-tools">
                    <button
                        className="vsc-tool-btn"
                        onClick={handleUndo}
                        disabled={undoStack.length === 0 || submitting}
                        title="Undo (Ctrl+Z)"
                    >
                        ↺ Undo
                    </button>
                    <button
                        className="vsc-tool-btn"
                        onClick={handleRedo}
                        disabled={redoStack.length === 0 || submitting}
                        title="Redo (Ctrl+Y)"
                    >
                        ↻ Redo
                    </button>
                    <button
                        className="vsc-tool-btn danger"
                        onClick={handleReset}
                        disabled={submitting}
                        title="Reset code template"
                    >
                        Reset
                    </button>
                </div>
            </div>

            {/* VS Code Editor Workspace (Line numbers gutter + Code Textarea) */}
            <div className="vscode-workspace">
                <div className="vscode-gutter" ref={lineNumbersRef}>
                    {lineNumbers.map((num) => (
                        <div
                            key={num}
                            className={`gutter-line ${num === cursorPos.line ? "active-line" : ""}`}
                        >
                            {num}
                        </div>
                    ))}
                </div>

                <textarea
                    ref={textareaRef}
                    className="vscode-textarea"
                    value={sourceCode}
                    onChange={(e) => {
                        recordState(e.target.value);
                        updateCursorPosition();
                    }}
                    onKeyDown={handleKeyDown}
                    onClick={updateCursorPosition}
                    onKeyUp={updateCursorPosition}
                    onScroll={handleScroll}
                    spellCheck="false"
                    autoCapitalize="off"
                    autoComplete="off"
                    autoCorrect="off"
                    disabled={submitting}
                    placeholder="// Write your C++ code with VS Code indentation and auto-closing brackets..."
                />
            </div>

            {/* VS Code Bottom Action & Status Bar */}
            <div className="vscode-action-bar">
                <div className="vscode-status-left">
                    <span className="status-item">Ln {cursorPos.line}, Col {cursorPos.col}</span>
                    <span className="status-item">Spaces: 4</span>
                    <span className="status-item">UTF-8</span>
                    <span className="status-item highlight">C++17</span>
                </div>

                <div className="vscode-actions-right">
                    <button
                        className="vsc-btn enqueue"
                        onClick={handleEnqueue}
                        disabled={submitting}
                        title="Feature 4: Push into FIFO Evaluation Queue"
                    >
                        Enqueue (FIFO)
                    </button>
                    <button
                        className="vsc-btn run"
                        onClick={handleSubmit}
                        disabled={submitting}
                        title="Submit code to online judge (Ctrl+Enter)"
                    >
                        {submitting ? "Judging..." : "▶ Run & Submit"}
                    </button>
                </div>
            </div>

            {queueMessage && (
                <div className="queue-status-banner">
                    <span className="banner-icon">ℹ</span> {queueMessage}
                </div>
            )}

            {error && (
                <div className="submission-error">
                    <strong>Error:</strong> {error}
                </div>
            )}

            {/* Judge Execution Output Drawer */}
            {result && (
                <div className="judge-drawer">
                    <div className="drawer-header">
                        <div className="drawer-title-group">
                            <h4>Execution Verdict:</h4>
                            <span
                                className={`verdict-badge ${result.result.verdict
                                    .toLowerCase()
                                    .replace(/\s+/g, "-")}`}
                            >
                                {result.result.verdict}
                            </span>
                        </div>
                        <span className="sub-ref">Submission #{result.submission.submissionID}</span>
                    </div>

                    <div className="drawer-metrics">
                        <div className="metric-box">
                            <span className="metric-label">Test Cases</span>
                            <span className="metric-val">
                                {result.result.passedTestCases} / {result.result.totalTestCases}
                            </span>
                        </div>
                        <div className="metric-box">
                            <span className="metric-label">Execution Time</span>
                            <span className="metric-val">
                                {result.result.executionTime ? `${result.result.executionTime.toFixed(2)} ms` : "0.00 ms"}
                            </span>
                        </div>
                        <div className="metric-box">
                            <span className="metric-label">Memory</span>
                            <span className="metric-val">
                                {result.result.memoryUsed ? `${result.result.memoryUsed} KB` : "1024 KB"}
                            </span>
                        </div>
                    </div>

                    {result.result.errorMessage && (
                        <div className="error-console">
                            <div className="error-console-header">Compiler & Runtime Diagnostics</div>
                            <pre>{result.result.errorMessage}</pre>
                        </div>
                    )}
                </div>
            )}
        </div>
    );
}

export default CodeEditor;
