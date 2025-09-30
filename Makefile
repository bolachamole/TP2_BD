.PHONY build docker-build docker-run-upload docker-run-findrec docker-run-seek1 docker-run-seek2

build:
	g++ upload.cpp -o upload
	g++ findrec.cpp -o findrec
	g++ seek1.cpp -o seek1
	g++ seek2.cpp -o seek2

docker-build:
	docker build -t tp2

docker-run-upload:
	docker run --rm -v $(pwd)/data:/data tp2 ./bin/upload

docker-run-findrec:
	docker run --rm -v $(pwd)/data:/data tp2 ./bin/findrec

docker-run-seek1:
	docker run --rm -v $(pwd)/data:/data tp2 ./bin/seek1

docker-run-seek2:
	docker run --rm -v $(pwd)/data:/data tp2 ./bin/seek2
