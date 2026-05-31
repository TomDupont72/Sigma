import { Button } from "@/components/ui/button";
import { Card, CardContent, CardHeader } from "@/components/ui/card";
import { Input } from "@/components/ui/input";
import { useChat } from "@/hooks/useChat";
import { Trash2 } from "lucide-react";
import ReactMarkdown from "react-markdown";
import remarkMath from "remark-math";
import rehypeKatex from "rehype-katex";
import "katex/dist/katex.min.css";
import { AnimatePresence, motion } from "framer-motion";
import { ToggleGroup, ToggleGroupItem } from "@/components/ui/toggle-group";

const MotionCard = motion(Card);

export default function Chat() {
    const {
        cells,
        renderedCells,
        resultCells,
        addCell,
        updateCell,
        deleteCell,
        engineAction,
    } = useChat();

    return (
        <main className="min-h-dvh flex flex-col items-center gap-6 p-6">
            <AnimatePresence>
                {Object.entries(cells).map(([key, cell]) => (
                    <MotionCard
                        key={key}
                        initial={{ scale: 0, opacity: 0 }}
                        animate={{ scale: 1, opacity: 1 }}
                        transition={{ duration: 0.1 }}
                        exit={{ scale: 0, opacity: 0 }}
                        className="relative w-full sm:w-8/10 md:w-7/10 lg:w-6/10 xl:w-5/10"
                    >
                        <Button
                            variant="destructive"
                            size="icon"
                            className="absolute top-2 right-2"
                            onClick={() => deleteCell(key)}
                        >
                            <Trash2 />
                        </Button>
                        <CardHeader>
                            <h1 className="font-bold text-xl">{key}</h1>
                        </CardHeader>
                        <CardContent className="flex flex-col gap-4">
                            <div className="flex flex-row gap-4">
                                <Input
                                    value={cell.expression}
                                    onChange={(e) =>
                                        updateCell(
                                            key,
                                            e.target.value,
                                            cell.derivationVariable,
                                            cell.mode,
                                        )
                                    }
                                ></Input>
                                <Button
                                    onClick={() =>
                                        engineAction(
                                            key,
                                            cell.expression,
                                            cell.derivationVariable,
                                            cell.mode,
                                        )
                                    }
                                >
                                    Calculer
                                </Button>
                            </div>
                            <ToggleGroup
                                variant="outline"
                                type="single"
                                value={cell.mode}
                                onValueChange={(value) => {
                                    if (value)
                                        updateCell(
                                            key,
                                            cell.expression,
                                            cell.derivationVariable,
                                            value,
                                        );
                                }}
                            >
                                <ToggleGroupItem value="simplify">
                                    Simplifier
                                </ToggleGroupItem>
                                <ToggleGroupItem value="derive">
                                    Dériver
                                </ToggleGroupItem>
                            </ToggleGroup>
                            {cell.mode === "derive" ? (
                                <div className="flex flex-row items-center gap-4">
                                    <p className="whitespace-nowrap">
                                        Dériver par rapport à :
                                    </p>
                                    <Input
                                        value={cell.derivationVariable}
                                        onChange={(e) =>
                                            updateCell(
                                                key,
                                                cell.expression,
                                                e.target.value,
                                                cell.mode,
                                            )
                                        }
                                    ></Input>
                                </div>
                            ) : null}
                            <h1 className="text-lg">Expression :</h1>
                            <div className="text-lg text-center">
                                <ReactMarkdown
                                    remarkPlugins={[remarkMath]}
                                    rehypePlugins={[rehypeKatex]}
                                >
                                    {renderedCells[key]
                                        ? `$${renderedCells[key]}$`
                                        : ""}
                                </ReactMarkdown>
                            </div>
                            {resultCells[key] ? (
                                <>
                                    <h1 className="text-lg">Résultat :</h1>
                                    <div className="text-lg text-center">
                                        <ReactMarkdown
                                            remarkPlugins={[remarkMath]}
                                            rehypePlugins={[rehypeKatex]}
                                        >{`$${resultCells[key]}$`}</ReactMarkdown>
                                    </div>
                                </>
                            ) : null}
                        </CardContent>
                    </MotionCard>
                ))}
            </AnimatePresence>

            <Button size="icon" onClick={addCell}>
                +
            </Button>
        </main>
    );
}
