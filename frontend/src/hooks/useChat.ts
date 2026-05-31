import { useState } from "react";
import { useMutation, useQueries } from "@tanstack/react-query"
import { apiEngineDerive, apiEngineDisplay, apiEngineSimplify } from "@/api/engine.api";
import type { Cell } from "@/types/chat.types";

export function useChat() {
    const [cellNumber, setCellNumber] = useState<number>(1);
    const [cells, setCells] = useState<Record<string, Cell>>({"Calcul - 1": {expression: "", derivationVariable: "", mode: "simplify"}});
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

    const engineDeriveMutation = useMutation({
        mutationFn: async ({ key, expression, derivationVariable }: {key: string, expression: string, derivationVariable: string}) => {
            const res = await apiEngineDerive({ expression: expression, derivationVariable: derivationVariable })
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
                [`Calcul - ${newCellNumber}`]: {expression: "", derivationVariable: "", mode: "simplify"}
            }
        ))
    }

    const updateCell = (key: string, expression: string, derivationVariable: string, mode: string) => {
        setCells((prev) => {
            const next = { ...prev }
            next[key] = {expression: expression, derivationVariable: derivationVariable, mode: mode}
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

    const engineDerive = async (key: string, expression: string, derivationVariable: string) => {
        await engineDeriveMutation.mutateAsync({ key, expression, derivationVariable })
    }

    const engineAction = async (key: string, expression: string, derivationVariable: string, mode: string) => {
        if (mode === "simplify") await engineSimplify(key, expression)

        if (mode === "derive") await engineDerive(key, expression, derivationVariable)
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
