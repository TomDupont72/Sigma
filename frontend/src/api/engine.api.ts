import type { EngineDisplayParams, EngineSimplifyBody } from "@/types/chat.types";

const API_URL = import.meta.env.VITE_API_URL;

export async function apiEngineDisplay(formData: EngineDisplayParams) {
    const res = await fetch(`${API_URL}/engine/display?expression=${encodeURIComponent(formData.expression)}`, {
        method: "GET"
    })

    if (!res.ok) {
        throw new Error("Failed to display expression");
    }

    return res.json()
}

export async function apiEngineSimplify(formData: EngineSimplifyBody) {
    console.log("coucou")
    const res = await fetch(`${API_URL}/engine/simplify`, {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify(formData)
    })

    if (!res.ok) {
        throw new Error("Failed to simplify expression");
    }

    return res.json()
}
