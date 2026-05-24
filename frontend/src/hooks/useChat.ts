import type { Cell } from "@/types/chat.types";
import { useState } from "react";

export function useChat() {
    const [cells, setCells] = useState<Record<string, Cell>>({})

    return {cells, setCells}
}
