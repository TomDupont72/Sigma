import type { Cell } from "@/types/chat.types";
import { useState } from "react";
import { useMutation, useQueries, useQuery } from "@tanstack/react-query"
import { apiEngineDisplay } from "@/api/engine.api";

export function useChat() {
    const [cellNumber, setCellNumber] = useState<number>(0);
    const [cells, setCells] = useState<Record<string, string>>({});

    const displayQueries = useQueries({
        queries: Object.entries(cells).map(([key, expression]) => ({
            queryKey: ["display", key, expression],
            queryFn: async () => {
                const res = await apiEngineDisplay({ expression: expression });
                console.log(res.data)
                return res.data;
            },
            enabled: !!expression,
        })),
    });

    const addCell = () => {
        const newCellNumber = cellNumber + 1;
        setCellNumber(newCellNumber)
        setCells((prev) => (
            {
                ...prev,
                [`Calcul - ${newCellNumber}`]: ""
            }
        ))
    }

    const updateCell = (key: string, expression: string) => {
        setCells((prev) => {
            const next = { ...prev }
            next[key] = expression
            return next
        })
    }

    const deleteCell = (key: string) => {
        setCells((prev) => {
            const next = { ...prev };
            delete next[key];
            return next

        })
    }

    const renderedCells = Object.fromEntries(
        Object.keys(cells).map((key, i) => [
            key,
            displayQueries[i].data?.expressionPlain ?? null
        ])
    );

    return { cells, setCells, renderedCells, addCell, updateCell, deleteCell }
}
