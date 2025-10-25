#ifndef GERENCIA_BLOCO
#define GERENCIA_BLOCO

#include <fstream>
#include <string>
#include <array>

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

template<size_t N>
struct bucket{
	unsigned int nRegs; //quantos registros tem dentro
	std::array<registro,N> registros;
	unsigned long long endereco_overf; //ponteiro de registro = endereco do bucket de overflow + endereco do registro
};

class GerenciaBlocos{
	private:
		const char* nome_arquivo;
		std::fstream arquivo;
		unsigned int size_blocos;
		unsigned int reg_per_bloco;
		unsigned long long size_alocados;

	public:
		GerenciaBlocos(std::string nome_arquivo);
		void setSize_blocos();
		void setSize_alocados(unsigned long long soma);

		unsigned int getSize_blocos();
		unsigned long long getSize_alocados();
		unsigned int getRegperbloco();

		void abreArquivo(std::ios::openmode modo);
		void fechaArquivo();
		void escreverBloco(unsigned long long endereco, void* bloco);
		void escreverRegistro(unsigned long long endereco, registro campos);
		void lerBloco(unsigned long long endereco, void* buffer);
		unsigned int totalDeBlocos();
};

#endif
