import { Routes, Route, Navigate } from "react-router-dom";
import Chat from "./pages/chat";
import { useEffect } from "react";

export default function App() {
    useEffect(() => {
        document.documentElement.classList.toggle("dark", true);
    });

    return (
        <Routes>
            <Route path="/chat" element={<Chat />} />
            <Route path="*" element={<Navigate to="/chat" />} />
        </Routes>
    );
}
