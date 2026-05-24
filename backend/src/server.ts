import "dotenv/config"
import Fastify from "fastify"
import { engineRoutes } from "./routes/engine.routes.js"
import rateLimit from "@fastify/rate-limit";
import fastifyCors from "@fastify/cors";


const app = Fastify({
    logger: true
})

await app.register(rateLimit, { max: 100, timeWindow: "1 minute" });

await app.register(fastifyCors, {
    origin: process.env.CLIENT_ORIGIN,
    credentials: true,
});

app.get("/api/health", async () => {
    return { status: "ok" }
})

await app.register(engineRoutes, { prefix: "/api/engine" })

await app.listen({
    port: Number(process.env.PORT),
    host: process.env.HOST
})
