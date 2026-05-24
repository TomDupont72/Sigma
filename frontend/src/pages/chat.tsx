import { Button } from "@/components/ui/button";
import { Card, CardContent, CardHeader } from "@/components/ui/card";
import { Input } from "@/components/ui/input";
import { useChat } from "@/hooks/useChat";
import { Trash2 } from "lucide-react";
import ReactMarkdown from "react-markdown";
import remarkMath from "remark-math";
import rehypeKatex from "rehype-katex";
import "katex/dist/katex.min.css";

export default function Chat() {
    const { cells, renderedCells, addCell, updateCell, deleteCell } = useChat();

    return (
        <main className="min-h-dvh flex flex-col items-center gap-6 p-6">
            {Object.entries(cells).map(([key, cell]) => (
                <Card key={key} className="relative w-5/10">
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
                                onChange={(e) =>
                                    updateCell(key, e.target.value)
                                }
                            ></Input>
                            <Button>Calculer</Button>
                        </div>
                        <h1 className="text-lg">Expression :</h1>
                        <div className="text-lg text-center">
                            <ReactMarkdown
                                remarkPlugins={[remarkMath]}
                                rehypePlugins={[rehypeKatex]}
                            >{`$${renderedCells[key]}$`}</ReactMarkdown>
                        </div>
                    </CardContent>
                </Card>
            ))}

            <Button size="icon" onClick={addCell}>
                +
            </Button>
        </main>
    );
}
