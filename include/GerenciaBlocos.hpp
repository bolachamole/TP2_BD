#ifndef GERENCIA_BLOCO
#define GERENCIA_BLOCO

#include <fstream>
#include <string>

#define TAM_MAX_REGISTRO 1518U //tamanho maximo do registro

struct registro{
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

struct bloco_dados{
	unsigned int nRegs; //quantos registros tem
	registro* registros;
};

class GerenciaBlocos{
	private:
		const char* arq_entrada;
		unsigned int size_blocos;

	public:
		GerenciaBlocos(std::string nome_arquivo);
		void setSize_blocos();
		unsigned int getSize_blocos();
		std::fstream abreArquivoDeDados(std::string nome_arquivo, std::ios::openmode modo);
		void fechaArquivo(std::fstream &arquivo);
		void lerBloco(std::fstream &arquivo,int indice, char* buffer);
		unsigned long long totalDeBlocos(std::fstream &arquivo);
};

#endif
