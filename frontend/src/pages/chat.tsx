import { Button } from "@/components/ui/button";
import { Card, CardContent, CardHeader } from "@/components/ui/card";
import { Input } from "@/components/ui/input";
import { useChat } from "@/hooks/useChat";

export default function Chat() {
    const { cells, setCells } = useChat();

    return (
        <main className="min-h-dvh flex flex-col items-center gap-6 p-6">
            {cells.map((cell) => (
                <Card key={cell.name} className="w-5/10">
                    <CardHeader>
                        <h1 className="font-bold text-xl">{cell.name}</h1>
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
                onClick={() =>
                    setCells((last) => [
                        ...last,
                        {
                            name: `Calcul - ${last.length + 1}`,
                            expression: "",
                            latexExpression: "",
                            result: "",
                            latexResult: "",
                        },
                    ])
                }
            >
                +
            </Button>
        </main>
    );
}
