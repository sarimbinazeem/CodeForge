
import { BrowserRouter, Routes, Route } from "react-router-dom";

import Navbar from "./components/Navbar";
import Problems from "./pages/Problems";
import ProblemDetails from "./pages/ProblemDetails";

import "./App.css";

function Home() {
    return (
        <div className="page-container">
            <h1>Welcome to CodeForge</h1>
            <p>
                Practice Data Structures and Algorithms.
                Write C++ solutions and get instant feedback.
            </p>
        </div>
    );
}



function Submissions() {
    return (
        <div className="page-container">
            <h1>Submission History</h1>
            <p>View your previous coding submissions.</p>
        </div>
    );
}

function App() {
    return (
        <BrowserRouter>

            <Navbar />

            <main>
                <Routes>

                    <Route path="/" element={<Home />} />

                    <Route
                        path="/problems"
                        element={<Problems />}
                    />

                    <Route
                        path="/problems/:id"
                        element={<ProblemDetails />}
                    />
                    
                    <Route
                        path="/submissions"
                        element={<Submissions />}
                    />

                </Routes>
            </main>

        </BrowserRouter>
    );
}

export default App;
