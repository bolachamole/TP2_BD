id = 123
titulo = Spatially-multiplexed MIMO markers.

.PHONY: build

build:
	g++ src/upload.cpp -o bin/upload
	g++ src/findrec.cpp -o bin/findrec
	g++ src/seek1.cpp -o bin/seek1
	g++ src/seek2.cpp -o bin/seek2

.PHONY: docker-build

docker-build:
	docker build -t tp2 .

.PHONY: docker-run-upload

docker-run-upload:
	docker run --rm -v $(shell pwd)/artigo.csv:/data/input.csv tp2 ./bin/upload /data/input.csv

.PHONY: docker-run-findrec

docker-run-findrec:
	docker run --rm -v $(shell pwd)/artigo.csv:/data/input.csv tp2 ./bin/findrec $(id)

.PHONY: docker-run-seek1

docker-run-seek1:
	docker run --rm -v $(shell pwd)/artigo.csv:/data/input.csv tp2 ./bin/seek1 $(id)

.PHONY: docker-run-seek2

docker-run-seek2:
	docker run --rm -v $(shell pwd)/artigo.csv:/data/input.csv tp2 ./bin/seek2 $(titulo)
