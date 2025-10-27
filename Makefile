id = 123
titulo = 2

SRCDIR := src
OBJDIR := objs
SRCF := $(wildcard $(SRCDIR)/*.cpp)
OBJF := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCF))

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp | $(OBJDIR)
	g++ -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY: build

build: $(OBJF)
	g++ $(filter-out $(OBJDIR)/findrec.o $(OBJDIR)/seek1.o $(OBJDIR)/seek2.o,$(OBJF)) -o bin/upload
	g++ $(filter-out $(OBJDIR)/upload.o $(OBJDIR)/seek1.o $(OBJDIR)/seek2.o,$(OBJF)) -o bin/findrec
	g++ $(filter-out $(OBJDIR)/findrec.o $(OBJDIR)/upload.o $(OBJDIR)/seek2.o,$(OBJF)) -o bin/seek1
	g++ $(filter-out $(OBJDIR)/findrec.o $(OBJDIR)/seek1.o $(OBJDIR)/upload.o,$(OBJF)) -o bin/seek2

.PHONY: docker-build

docker-build:
	docker build -t tp2 .
	mkdir -p bin
	docker run --rm -d --name temp_tp2 tp2 sleep 5
	docker cp temp_tp2:/app/bin/. ./bin/
	docker rm -f temp_tp2

.PHONY: docker-run-upload

docker-run-upload:
	docker run --rm -v $(shell pwd)/data:/data tp2 ./bin/upload /data/input.csv

.PHONY: docker-run-findrec

docker-run-findrec:
	docker run --rm -v $(shell pwd)/data:/data tp2 ./bin/findrec $(id)

.PHONY: docker-run-seek1

docker-run-seek1:
	docker run --rm -v $(shell pwd)/data:/data tp2 ./bin/seek1 $(id)

.PHONY: docker-run-seek2

docker-run-seek2:
	docker run --rm -v $(shell pwd)/data:/data tp2 ./bin/seek2 $(titulo)

.PHONY: clean

clean:
	rm -f bin/*
	rm -r $(OBJDIR)
