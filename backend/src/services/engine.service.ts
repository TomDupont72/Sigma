import { execFile } from "child_process";
import { promisify } from "util";

const ENGINE_PATH = process.env.ENGINE_PATH;

const execFileAsync = promisify(execFile);

export async function apiEngineDisplay(expression: string) {
    const { stdout } = await execFileAsync(
        `${ENGINE_PATH}/app`,
        ["display", expression]
    );

    return { expressionLatex: stdout }
}

export async function apiEngineSimplify(expression: string) {
    const { stdout } = await execFileAsync(
        `${ENGINE_PATH}/app`,
        ["simplify", expression]
    );

    return { expressionLatex: stdout }
}

export async function apiDeriveSimplify(expression: string, derivationVariable: string) {
    console.log(expression, derivationVariable)
    const { stdout } = await execFileAsync(
        `${ENGINE_PATH}/app`,
        ["derive", expression, derivationVariable]
    );

    return { expressionLatex: stdout }
}
