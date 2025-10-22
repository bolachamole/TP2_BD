#include <string>
#include <regex>
#include <cstring>
#include "../include/ParserCSV.hpp"

ParserCSV::ParserCSV(std::string arq_nome){
	this->arq_caminho = arq_nome;
	arq.open(arq_nome);
}

void ParserCSV::lerArquivo(Hashing tabela){
	std::string linha;
	std::smatch campos;
	std::regex exprecao("\"([0-9]+)\";\"((?:.)*)\";\"([0-9]{4})\";\"((?:.)*)\";\"([0-9]+)\";\"([0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2})\";(?:NULL|\"((?:.)*)\")", std::regex::ECMAScript);
	std::regex exprecao_2("\"([0-9]+)\";\"((?:.)*)", std::regex::ECMAScript); //casos em que o titulo esta cortado por \n
	std::regex exprecao_3("((?:.)*)\";\"([0-9]{4})\";\"((?:.)*)\";\"([0-9]+)\";\"([0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2})\";(?:NULL|\"((?:.)*)\")", std::regex::ECMAScript); //resto do caso
	registro* novoRegistro = new registro;

	while(std::getline(arq, linha)){
		if(std::regex_search(linha, campos, exprecao)){
			novoRegistro->id = std::stoi(campos.str(1));
			strcpy(novoRegistro->titulo, campos.str(2).c_str());
			novoRegistro->ano = std::stoi(campos.str(3));
			if(campos.str(4) == "\" \""){ //se o campo autores for literalmente " "
				novoRegistro->tam_autores = 0;
				novoRegistro->autores[0] = '\0';
			}
			else{
				strcpy(novoRegistro->autores, campos.str(4).c_str());
			}
			novoRegistro->citacoes = std::stoi(campos.str(5));
			strcpy(novoRegistro->atualizacao, campos.str(6).c_str());
			if((campos.str(7).size() > 99) && (campos.str(7).size() < 1025)){
				novoRegistro->tam_snippet = campos.str(7).size();
				strcpy(novoRegistro->snippet, campos.str(7).c_str());
			} else{
				novoRegistro->tam_snippet = 0;
				novoRegistro->snippet[0] = '\0';
			}
			tabela.insereHash(novoRegistro);
		} else if(std::regex_search(linha, campos, exprecao_2)){
			novoRegistro->id = std::stoi(campos.str(1));
			strcpy(novoRegistro->titulo, campos.str(2).c_str());
		} else if(std::regex_search(linha, campos, exprecao_3)){
			novoRegistro->ano = std::stoi(campos.str(2));
			if(campos.str(3) == "\" \""){ //se o campo autores for literalmente " "
				novoRegistro->tam_autores = 0;
				novoRegistro->autores[0] = '\0';
			}
			else{
				strcpy(novoRegistro->autores, campos.str(3).c_str());
			}
			novoRegistro->citacoes = std::stoi(campos.str(4));
			strcpy(novoRegistro->atualizacao, campos.str(5).c_str());
			if((campos.str(6).size() > 99) && (campos.str(6).size() < 1025)){
				strcpy(novoRegistro->snippet, campos.str(6).c_str());
			} else {
				novoRegistro->tam_snippet = 0;
				novoRegistro->snippet[0] = '\0';
			}
			tabela.insereHash(novoRegistro);
		}
	}
	delete novoRegistro;
}

void ParserCSV::fechaArquivo(){
	this->arq.close();
}
