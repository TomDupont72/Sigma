import { FastifyInstance, FastifyReply, FastifyRequest } from "fastify";
import { EngineDisplayBody, EngineSimplifyBody } from "../types/engine.types.js";
import { apiEngineDisplay, apiEngineSimplify } from "../services/engine.service.js";
import { execFile } from "child_process";
import { promisify } from "util";

const execFileAsync = promisify(execFile);

export async function engineRoutes(fastify: FastifyInstance) {
    fastify.get(
        "/display",
        async (request: FastifyRequest, reply: FastifyReply) => {
            const { expression } = request.query as EngineDisplayBody;

            const data = await apiEngineDisplay(expression);

            return reply.send({ data })
        }
    )

    fastify.post(
        "/simplify",
        async (request: FastifyRequest, reply: FastifyReply) => {
            const { expression } = request.body as EngineSimplifyBody;

            const data = await apiEngineSimplify(expression);

            return reply.send({ data })
        }
    )
}
