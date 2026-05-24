import type { Cell } from "@/types/chat.types";
import { useState } from "react";

export function useChat() {
    const [cellNumber, setCellNumber] = useState<number>(0);
    const [cells, setCells] = useState<Record<string, Cell>>({});

    const addCell = () => {
        const newCellNumber = cellNumber + 1;
        setCellNumber(newCellNumber)
        setCells((prev) => (
            {
                ...prev,
                [`Calcul - ${newCellNumber}`]: {
                    expression: "",
                    latexExpression: "",
                    result: "",
                    latexResult: ""
                }
            }
        ))
    }

    const deleteCell = (key: string) => {
        setCells((prev) => {
            const next = { ...prev };
            delete next[key];
            return next

        })
    }

    return { cells, setCells, addCell, deleteCell }
}
