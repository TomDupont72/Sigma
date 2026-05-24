import { Button } from "@/components/ui/button";
import { Card, CardContent, CardHeader } from "@/components/ui/card";
import { Input } from "@/components/ui/input";
import { useChat } from "@/hooks/useChat";
import { Trash2 } from "lucide-react";

export default function Chat() {
    const { cells, setCells, addCell, deleteCell } = useChat();

    return (
        <main className="min-h-dvh flex flex-col items-center gap-6 p-6">
            {Object.entries(cells).map(([key, cell]) => (
                <Card key={key} className="relative w-5/10">
                    <Button
                        variant="destructive"
                        size="icon"
                        className="absolute top-2 right-2"
                        onClick={() =>
                    deleteCell(key)
                        }
                    >
                        <Trash2 />
                    </Button>
                    <CardHeader>
                        <h1 className="font-bold text-xl">{key}</h1>
                    </CardHeader>
                    <CardContent>
                        <div className="flex flex-row gap-4">
                            <Input></Input>
                            <Button>Calculer</Button>
                        </div>
                    </CardContent>
                </Card>
            ))}

            <Button
                size="icon"
                onClick={addCell}
            >
                +
            </Button>
        </main>
    );
}
