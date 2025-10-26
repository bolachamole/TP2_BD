#include <string>
#include <regex>
#include <cstring>
#include "../include/ParserCSV.hpp"

ParserCSV::ParserCSV(std::string arq_nome){
	this->arq_caminho = arq_nome;
	arq.open(arq_nome, std::ios::in);
	linhas_proc = 0;
}

int ParserCSV::getLinhasProc(){
	return linhas_proc;
}

void ParserCSV::lerArquivo(Hashing* tabela){
	std::string linha, lin_proc;
	std::smatch campos;
	std::regex exprecao("\"([0-9]+)\";\"((?:.)*)\";\"([0-9]{4})\";\"((?:.)*)\";\"([0-9]+)\";\"([0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2})\";(?:NULL|\"((?:.)*)\")", std::regex::ECMAScript);

	while(std::getline(arq, linha)){
		if(linha.back() == '"' || linha.back() == 'L'){
			lin_proc.append(linha);
		} else{ //se teve um \0 (o \n e pulado automaticamente pelo getline)
			lin_proc.append(linha, linha.size() - 1); //tira o \0
		}
		
		if(std::regex_search(lin_proc, campos, exprecao)){
			registro novoRegistro;
			novoRegistro.total_size = 0;
			
			//pega o id (inteiro)
			novoRegistro.id = std::stoi(campos.str(1));
			novoRegistro.total_size += sizeof(unsigned int);
			
			//pega o titulo (alfa 300)
			if(campos.str(2).size() > 300){
				novoRegistro.tam_titulo = 300;
				strncpy(novoRegistro.titulo, campos.str(2).c_str(), 300);
				novoRegistro.titulo[299] = '\0';
			} else{
				novoRegistro.tam_titulo = campos.str(2).size();
				strcpy(novoRegistro.titulo, campos.str(2).c_str());
			}
			novoRegistro.total_size += novoRegistro.tam_titulo;
			
			//pega o ano (inteiro)
			novoRegistro.ano = std::stoi(campos.str(3));
			novoRegistro.total_size += sizeof(unsigned int);
			
			//pega os autores (alfa 150)
			if(campos.str(4).size() > 150){
				novoRegistro.tam_autores = 150;
				strncpy(novoRegistro.autores, campos.str(4).c_str(), 150);
				novoRegistro.autores[149] = '\0';
			} else if(campos.str(4) == "\" \""){ //se o campo autores for literalmente " "
				novoRegistro.tam_autores = 0;
				novoRegistro.autores[0] = '\0';
			} else{
				novoRegistro.tam_autores = campos.str(4).size();
				strcpy(novoRegistro.autores, campos.str(4).c_str());
			}
			novoRegistro.total_size += novoRegistro.tam_autores;
			
			//pega as citacoes (inteiro)
			novoRegistro.citacoes = std::stoi(campos.str(5));
			novoRegistro.total_size += sizeof(unsigned int);
			
			//pega a data de atualizacao (interpretada como alfa 20)
			strcpy(novoRegistro.atualizacao, campos.str(6).c_str());
			novoRegistro.total_size += campos.str(6).size();
			
			//pega o snippet (alfa entre 100 e 1024)
			if((campos.str(7).size() > 99) && (campos.str(7).size() < 1024)){
				novoRegistro.tam_snippet = campos.str(7).size();
				strcpy(novoRegistro.snippet, campos.str(7).c_str());
			} else if(campos.str(7).size() > 1024){
				novoRegistro.tam_snippet = 1024;
				strncpy(novoRegistro.snippet, campos.str(7).c_str(), 1024);
				novoRegistro.snippet[1023] = '\0';
			} else{
				novoRegistro.tam_snippet = 0;
				novoRegistro.snippet[0] = '\0';
			}
			novoRegistro.total_size += novoRegistro.tam_snippet;
			tabela->insereHash(novoRegistro);
		}
		linhas_proc += 1;
	}
}

void ParserCSV::fechaArquivo(){
	this->arq.close();
}
