#include <sys/stat.h>
#include <iostream>
#include "../include/GerenciaBlocos.hpp"

GerenciaBlocos::GerenciaBlocos(std::string arq_nome){
	this->arq_entrada = arq_nome.c_str();
}

void GerenciaBlocos::setSize_blocos(int fd){
	struct stat infostat;
	unsigned int tam_bloco;

	if(lstat(this->arq_entrada,&infostat) == -1){
		std::cout << "Erro ao tentar pegar o tamanho dos blocos.";
		exit(1);
	}
	tam_bloco = (unsigned int)infostat.st_blksize;

	this->size_blocos = tam_bloco;
}

unsigned int GerenciaBlocos::getSize_blocos(){
	return this->size_blocos;
}
