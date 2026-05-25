FROM node:20 AS engine-builder

WORKDIR /app

RUN apt-get update \
    && apt-get install -y --no-install-recommends build-essential make \
    && rm -rf /var/lib/apt/lists/*

COPY engine ./engine

WORKDIR /app/engine

RUN rm -f app app.exe NUL \
    && find src \( -name '*.o' -o -name '*.d' \) -delete \
    && make app

FROM node:20 AS builder

WORKDIR /app

COPY backend/package*.json ./
RUN npm ci

COPY backend ./

RUN npm run build

FROM node:20

WORKDIR /app

COPY backend/package*.json ./
RUN npm ci --omit=dev

COPY --from=builder /app/dist ./dist
COPY --from=engine-builder /app/engine/app ./engine/app
RUN chmod +x ./engine/app

EXPOSE 8000

CMD ["node", "dist/server.js"]
