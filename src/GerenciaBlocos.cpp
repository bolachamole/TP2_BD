#include <sys/stat.h>
#include <iostream>
#include "../include/GerenciaBlocos.hpp"

GerenciaBlocos::GerenciaBlocos(std::string arq_nome){
	arq_entrada = arq_nome.c_str();
}

void GerenciaBlocos::setNitems(int n){
	nitems = n;
}

void GerenciaBlocos::setSize_blocos(){
	struct stat infostat;
	unsigned int tam_bloco;

	if(lstat(arq_entrada,&infostat) == -1){
		std::cout << "Erro ao tentar pegar o tamanho dos blocos.";
		exit(1);
	}
	tam_bloco = (unsigned int)infostat.st_blksize;

	size_blocos = tam_bloco;
}

unsigned int GerenciaBlocos::getNitems(){
	return nitems;
}

unsigned int GerenciaBlocos::getSize_blocos(){
	return size_blocos;
}
