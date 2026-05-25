import { useState } from "react";
import { useMutation, useQueries } from "@tanstack/react-query"
import { apiEngineDisplay, apiEngineSimplify } from "@/api/engine.api";

export function useChat() {
    const [cellNumber, setCellNumber] = useState<number>(0);
    const [cells, setCells] = useState<Record<string, string>>({});
    const [resultCells, setResultCells] = useState<Record<string, string>>({});

    const engineDisplayQueries = useQueries({
        queries: Object.entries(cells).map(([key, expression]) => ({
            queryKey: ["display", key, expression],
            queryFn: async () => {
                const res = await apiEngineDisplay({ expression: expression });
                return res.data;
            },
            enabled: !!expression,
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

    const engineSimplify = async (key: string, expression: string) => {
        await engineSimplifyMutation.mutateAsync({ key, expression })
    }

    const renderedCells = Object.fromEntries(
        Object.keys(cells).map((key, i) => [
            key,
            engineDisplayQueries[i].data?.expressionLatex ?? null
        ])
    );

    return { cells, setCells, renderedCells, resultCells, addCell, updateCell, deleteCell, engineSimplify }
}
