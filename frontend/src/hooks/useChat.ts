import type { Cell } from "@/types/chat.types";
import { useState } from "react";

export function useChat() {
    const [cells, setCells] = useState<Cell[]>([])

    return {cells, setCells}
}
