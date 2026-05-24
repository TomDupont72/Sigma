const API_URL = import.meta.env.VITE_API_URL;

export async function apiEngineDisplay(formData) {
    const res = await fetch(`${API_URL}/engine/display?expression=${encodeURIComponent(formData.expression)}`, {
        method: "GET"
    })

    if (!res.ok) {
        throw new Error("Failed display expression");
    }

    return res.json()
}
