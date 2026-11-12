
import { Link, NavLink } from "react-router-dom";
import "./Navbar.css";

function Navbar() {
    return (
        <nav className="navbar">

            <Link to="/" className="navbar-logo">
                <span className="logo-bracket">{"<"}</span>
                CodeForge
                <span className="logo-bracket">{"/>"}</span>
            </Link>

            <div className="navbar-links">

                <NavLink
                    to="/"
                    className={({ isActive }) =>
                        isActive ? "nav-link active" : "nav-link"
                    }
                    end
                >
                    Home
                </NavLink>

                <NavLink
                    to="/problems"
                    className={({ isActive }) =>
                        isActive ? "nav-link active" : "nav-link"
                    }
                >
                    Problems
                </NavLink>

                <NavLink
                    to="/submissions"
                    className={({ isActive }) =>
                        isActive ? "nav-link active" : "nav-link"
                    }
                >
                    Submissions
                </NavLink>

            </div>

            <div className="navbar-right">
                <span className="navbar-status">
                    <span className="status-dot"></span>
                    Backend
                </span>
            </div>

        </nav>
    );
}

export default Navbar;
