#ifndef PARSER_CSV
#define PARSER_CSV

#include <fstream>
#include <string>
#include <vector>
#include "../include/Hashing.hpp"

class ParserCSV{
    private:
	int nitems;
        std::string arq_caminho;
        std::ifstream arq;
	std::vector<registro*> registros;

    public:
        ParserCSV(std::string arq_nome);
        void lerArquivo();
        int getNitems();
	void fechaArquivo();
};

#endif
