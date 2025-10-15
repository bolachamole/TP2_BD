#ifndef PARSER_CSV
#define PARSER_CSV

#include <fstream>
#include <string>
#include <vector>

struct registro{
	int id;
    unsigned int tam_titulo;
	char titulo[300];
	int ano;
    unsigned int tam_autores;
	char autores[150];
	int citacoes;
	char atualizacao[20];
    unsigned int tam_snippet;
	char snippet[1024];
};

class ParserCSV{
    private:
        std::string arq_caminho;
        std::ifstream arq;

    public:
        ParserCSV(std::string arq_nome){}
        std::vector<registro> lerArquivo();
        void fechaArquivo();
};

#endif