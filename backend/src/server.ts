import "dotenv/config"
import Fastify from "fastify"
import { engineRoutes } from "./routes/engine.routes.js"

const app = Fastify({
    logger: true
})

app.get("/api/health", async () => {
    return { status: "ok" }
})

await app.register(engineRoutes, { prefix: "/api/engine" })

await app.listen({
    port: Number(process.env.PORT),
    host: process.env.HOST
})
