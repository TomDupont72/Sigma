export type Cell = {
    expression: string,
    derivationVariable: string,
    mode: string
}

export type EngineDisplayBody = {
    expression: string
}

export type EngineSimplifyBody = EngineDisplayBody

export type EngineDeriveBody = {
    expression: string,
    derivationVariable: string
}
