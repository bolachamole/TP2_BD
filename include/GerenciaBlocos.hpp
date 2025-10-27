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
		std::fstream arquivo;
		unsigned int size_blocos;
		unsigned int regperbloco;
		unsigned long long size_hash;
		int blocos_lidos=0;
		int blocos_escritos=0;

	public:
		GerenciaBlocos(std::string nome_arquivo);
		void somaSize_hash(unsigned long long soma);
		void setSize_blocos();

		unsigned int getSize_blocos();
		unsigned long long getSize_hash();
		unsigned int getRegperbloco();
		int getBlocos_lidos();
		int getBlocos_escritos();
		void zeraBlocosLidos(){blocos_lidos=0;}

		void abreArquivo();
		void fechaArquivoFstream();
		unsigned int totalDeBlocosArquivo();
		void escreverBloco(unsigned long long endereco, char* bloco);
		void lerBloco(unsigned long long endereco, char* buffer);
		void lerBucket(unsigned long long endereco, bucket* buffer);
		void escreverRegistroBloco(unsigned long long endereco, registro campos);
		void* lerBlocoMemoria(unsigned long long endereco);
};

#endif
