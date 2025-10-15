#include <fstream>
#include <string>
#include <regex>
#include <cstring>
#include "../include/ParserCSV.hpp"

ParserCSV::ParserCSV(std::string arq_nome){
	this->arq_caminho = arq_nome;
	arq.open(arq_nome);
}

std::vector<registro> ParserCSV::lerArquivo(){
	int i;
	std::string linha;
	std::vector<registro> registros;

	std::smatch campos;
	std::regex exprecao("\"([0-9]+)\";\"((?:.)*)\";\"([0-9]{4})\";\"((?:.)*)\";\"([0-9]+)\";\"([0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2})\";(?:NULL|\"((?:.)*)\")", std::regex::ECMAScript);
	
	while(std::getline(arq, linha)){
		if(std::regex_search(linha, campos, exprecao)){
			registro novoRegistro;
			novoRegistro.id = std::stoi(campos.str(1));
			novoRegistro.tam_titulo = campos.str(2).size();
			strcpy(novoRegistro.titulo, campos.str(2).c_str());
			novoRegistro.ano = std::stoi(campos.str(3));
			novoRegistro.tam_autores = campos.str(4).size();
			strcpy(novoRegistro.autores, campos.str(4).c_str());
			novoRegistro.citacoes = std::stoi(campos.str(5));
			strcpy(novoRegistro.atualizacao, campos.str(6).c_str());
			novoRegistro.tam_snippet = campos.str(7).size();
			strcpy(novoRegistro.snippet, campos.str(7).c_str());

			registros.push_back(novoRegistro);
		}
	}

	return registros;
}

void ParserCSV::fechaArquivo(){
	this->arq.close();
}