# Instruções de build

Baixar arquivo de entrada [neste link](https://drive.google.com/file/d/1EVoP0d9Wwzj1O6eoFIkel9I3cpe43Gbv/view) no diretório do projeto e descomprimir com ```gzip -d artigo.csv.gz```

Para compilar binários localmente:

```
make build
```

Para construir via Docker:

```
make docker-build
```

Caso seja preciso eliminar os arquivos criados:

```
make clean
```

# Execução dos programas

Para fazer a carga inicial dos dados e criar banco de dados:

* Via make:

```
make docker-run-upload
```

* Dentro do container:

```
docker compose run --rm upload data/input.csv
```

Para buscar no arquivo de dados um registro com o \*\*ID** informado:

* Via make:

```
make docker-run-findrec id=**ID**
```

Caso não seja especificado o parâmetro, será feita a busca com o ID "123".

* Dentro do container:

```
docker compose run --rm findrec **ID**
```

Para buscar no arquivo de índice primário o registro com o \*\*ID** informado:

* Via make:

```
make docker-run-seek1 id=**ID**
```

Caso não seja especificado o parâmetro, será feita a busca com o ID "123".

* Dentro do container:

```
docker compose run --rm seek1 **ID**
```

Para buscar no arquivo de índice secundário o registro com o \*\*título** informado:

* Via make:

```
make docker-run-seek2 titulo=**título**
```

Caso não seja especificado o parâmetro, será feita a busca com o título "2".

* Dentro do container:

```
docker compose run --rm seek2 **título**
```

# Layout dos arquivos


# Exemplo de entrada/saída
