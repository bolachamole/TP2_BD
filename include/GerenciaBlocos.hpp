#ifndef GERENCIA_BLOCO
#define GERENCIA_BLOCO

#include <fstream>//biblioteca para manipulacao de arquivos
#include <string>

class GerenciaBlocos{
	private:
		const char* arq_entrada;
		int nitems;
		unsigned int size_blocos;

	public:
		GerenciaBlocos(std::string arq_nome);

		void setNitems(int n);
		void setSize_blocos();
		int getNitems();
		unsigned int getSize_blocos();

		static std::fstream abreArquivo(const std::string &nome_arquivo, std::ios::openmode modo);
		static void fechaArquivo(std::fstream &arquivo);
		static void lerBloco(std::fstream &arquivo,int indice, char* buffer);
		static int totalDeBlocos(std::fstream &arquivo, unsigned int size_blocos);
};

#endif
