FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y --no-install-recommends g++ make cmake && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . /app

VOLUME ["/data"]

ENV CSV_PATH=/data/input.csv \
    DATA_DIR=/data/db \
    LOG_LEVEL=info

CMD ["bash", "-lc", "echo 'Modo de uso: docker run --rm -v $(pwd)/data:/data tp2 [ CAMINHO ] [ PARAMETRO ]'; ls -l bin/"]
