#ifndef GERENCIA_BLOCO
#define GERENCIA_BLOCO

#include <fstream>
#include <string>

struct registro{
	unsigned int total_size;
	unsigned int id;
	unsigned int tam_titulo;
	char titulo[300];
	unsigned int ano;
	unsigned int tam_autores;
	char autores[150];
	unsigned int citacoes;
	char atualizacao[20];
	unsigned int tam_snippet;
	char snippet[1024];
};

struct bucket{
	unsigned int nRegs; //quantos registros tem dentro
	registro* registros;
	unsigned long long endereco_overf; //ponteiro de registro = endereco do bucket de overflow + endereco do registro
};

class GerenciaBlocos{
	private:
		const char* nome_arquivo;
		int fd;
		std::fstream arquivo;
		void* mapaAddr;
		unsigned int size_blocos;
		unsigned int quant_bloco;
		unsigned int regperbloco;
		unsigned long long aloc_mapa = 4000; //4KB
		unsigned long long size_hash;
		int blocos_lidos;
		int blocos_escritos;

	public:
		GerenciaBlocos(std::string nome_arquivo);
		GerenciaBlocos(std::string nome_arquivo, unsigned long long offset);
		void setSize_blocos();
		void setMapaAddr(unsigned long long endereco);
		void somaSize_hash(unsigned long long soma);

		unsigned int getSize_blocos();
		unsigned long long getSize_hash();
		unsigned int getRegperbloco();
		int getBlocos_lidos();
		int getBlocos_escritos();

		void abreArquivo();
		void abreArquivoFstream();
		void fechaArquivo();
		void fechaArquivoFstream();
		unsigned int totalDeBlocosArquivo();
		void escreverBlocoMemoria(unsigned long long endereco, void* bloco);
		void escreverBucket(unsigned long long endereco, bucket bloco);
		void* lerBlocoMemoria(unsigned long long endereco);
		void lerBucket(unsigned long long endereco, bucket* buffer);
		void escreverRegistroMemoria(unsigned long long endereco, registro campos);
		void escreverRegistroBucket(unsigned long long endereco, registro campos);
		void sincronizaMapa();
};

#endif
