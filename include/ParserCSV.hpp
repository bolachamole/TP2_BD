#ifndef PARSER_CSV
#define PARSER_CSV

#include <fstream>
#include <string>
#include "../include/Hashing.hpp"

class ParserCSV{
    private:
        std::string arq_caminho;
        std::ifstream arq;

    public:
        ParserCSV(std::string arq_nome);
        void lerArquivo(Hashing tabela);
	void fechaArquivo();
};

#endif
