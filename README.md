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

Caso seja preciso eliminar tudo:

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
docker compose run --rm upload input.csv
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

Sejam info, debug, erro e warn níveis de log, adicione -e LOG_LEVEL=\*\*nível de log** ao comando para mudar a configuração do nível de mensagens de log no docker.

Exemplo:

```
docker compose run --rm -e LOG_LEVEL=erro upload input.csv
```

# Layout dos arquivos

Arquivo de dados (arqdados.dat): armazena dados organizados por hash com base no ID. Dividido em registros com os seguintes campos:

* total_size (unsigned int): tamanho total do registro
* id (unsigned int): ID do artigo
* tam_titulo (unsigned int): tamanho do campo de título
* titulo (char[300]): título do artigo
* ano (unsigned int): ano de publicação do artigo
* tam_autores (unsigned int): tamanho do campo de autores
* autores (char[150]): lista de autores do artigo
* citacoes (unsigned int): número de vezes que o artigo foi citado
* atualizacao (char[20]): data e hora da última atualização dos dados
* tam_snippet (unsigned int): tamanho do campo snippet
* snippet (mínimo char[100], máximo char[1024]): resumo textual do artigo

Os buckets possuem a seguinte estrutura:

* nRegs (unsigned int): quantidade de registros armazenados no bloco.
* registros (registro*): lista dos nRegs registros armazenados.
* endereco_overf (unsigned long long): ponteiro de registro para o bucket de overflow.

Arquivo de indexação primária (arqindex1.dat): organiza índices com uma árvore B+ em memória secundária com base no campo de ID.

Arquivo de indexação secundária (arqindex2.dat): organiza índices com uma árvore B+ em memória secundária com base no campo de título.

# Exemplo de entrada/saída

```
$ make docker-run-findrec id=265606
"Caminho do arquivo de dados: data/db/arqdados.dat
----- Registro Encontrado -----
ID: 265606
Titulo: Airplane Boarding, Disk Scheduling and Space-Time Geometry
Ano: 2005
Autores: Eitan Bachmat|Daniel Berend|Luba Sapir|Steven Skiena
Citacoes: 7
Atualizacao: 2016-10-06 17:26:00
"Snippet: Airplane boarding, disk scheduling and space-time geometry. E Bachmat, D Berend, L Sapir, S Skiena -   Applications in Management, 2005 - Springer. Abstract We show how the process of passengers boarding an airplane and the process of  optimal I/O scheduling to a disk drive with a linear seek function can be asymptotically  modeled by 2-dimensional space-time geometry. We relate the space-time geometry of  ..
------------------------------
```