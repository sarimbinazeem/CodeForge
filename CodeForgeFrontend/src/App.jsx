import { Routes, Route } from "react-router-dom";
import Navbar from "./components/Navbar";
import Home from "./pages/Home";
import Problems from "./pages/Problems";
import ProblemDetails from "./pages/ProblemDetails";
import Submissions from "./pages/Submissions";
import Leaderboard from "./pages/Leaderboard";
import Roadmap from "./pages/Roadmap";
import SkillTree from "./pages/SkillTree";

function App() {
  return (
    <div className="app-container">
      <Navbar />
      <main className="main-content">
        <Routes>
          <Route path="/" element={<Home />} />
          <Route path="/problems" element={<Problems />} />
          <Route path="/problems/:id" element={<ProblemDetails />} />
          <Route path="/leaderboard" element={<Leaderboard />} />
          <Route path="/roadmap" element={<Roadmap />} />
          <Route path="/skills" element={<SkillTree />} />
          <Route path="/submissions" element={<Submissions />} />
        </Routes>
      </main>
    </div>
  );
}

export default App;