FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y --no-install-recommends g++ make cmake && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . /app

RUN make build
VOLUME ["/data"]

CMD ["bash", "-lc", "echo 'Modo de uso: docker run --rm -v $(pwd)/data:/data tp2 [ caminho do arquivo binario ] [ parametros ]'; ls -l bin/"]
