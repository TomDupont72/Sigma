import { useState } from "react";
import { useMutation, useQueries } from "@tanstack/react-query"
import { apiEngineDisplay, apiEngineSimplify } from "@/api/engine.api";
import type { Cell } from "@/types/chat.types";

export function useChat() {
    const [cellNumber, setCellNumber] = useState<number>(1);
    const [cells, setCells] = useState<Record<string, Cell>>({"Calcul - 1": {expression: "", mode: "simplify"}});
    const [resultCells, setResultCells] = useState<Record<string, string>>({"Calcul - 1": ""});

    const engineDisplayQueries = useQueries({
        queries: Object.entries(cells).map(([key, cell]) => ({
            queryKey: ["display", key, cell.expression],
            queryFn: async () => {
                const res = await apiEngineDisplay({ expression: cell.expression });
                return res.data;
            },
            enabled: !!cell.expression,
        })),
    });

    const engineSimplifyMutation = useMutation({
        mutationFn: async ({ key, expression }: {key: string, expression: string}) => {
            const res = await apiEngineSimplify({ expression: expression })
            console.log(res)
            return { key, data: res.data }
        },
        onSuccess: ({ key, data }) => {
            setResultCells((prev) => ({ ...prev, [key]: data.expressionLatex}))
        }
    })

    const addCell = () => {
        const newCellNumber = cellNumber + 1;
        setCellNumber(newCellNumber)
        setCells((prev) => (
            {
                ...prev,
                [`Calcul - ${newCellNumber}`]: {expression: "", mode: "simplify"}
            }
        ))
    }

    const updateCell = (key: string, expression: string, mode: string) => {
        setCells((prev) => {
            const next = { ...prev }
            next[key] = {expression: expression, mode: mode}
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

    const engineSimplify = async (key: string, expression: string) => {
        await engineSimplifyMutation.mutateAsync({ key, expression })
    }

    const engineAction = async (key: string, expression: string, mode: string) => {
        if (mode === "simplify") await engineSimplify(key, expression)

        if (mode === "derive") console.log("derive")//Todo
    }

    const renderedCells = Object.fromEntries(
        Object.keys(cells).map((key, i) => [
            key,
            engineDisplayQueries[i].data?.expressionLatex ?? null
        ])
    );

    console.log(cells)

    return { cells, renderedCells, resultCells, addCell, updateCell, deleteCell, engineAction }
}
