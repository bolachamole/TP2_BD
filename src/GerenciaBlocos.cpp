#include <sys/stat.h>
#include <iostream>
#include "../include/GerenciaBlocos.hpp"

GerenciaBlocos::GerenciaBlocos(std::string nome_arquivo){
	arq_entrada = nome_arquivo.c_str();
	setSize_blocos();
	reg_per_bloco = size_blocos / TAM_MAX_REGISTRO;
}

void GerenciaBlocos::setSize_blocos(){
	struct stat infostat;
	unsigned int tam_bloco;

	if(lstat(arq_entrada,&infostat) == -1){
		std::cout << "Erro ao tentar pegar o tamanho dos blocos.";
		exit(1);
	}
	tam_bloco = (unsigned int)infostat.st_blksize;

	this->size_blocos = tam_bloco;
}

unsigned int GerenciaBlocos::getSize_blocos(){
	return this->size_blocos;
}

std::fstream GerenciaBlocos::abreArquivoDeDados(std::string nome_arquivo, std::ios::openmode modo){
	std::fstream arquivo(nome_arquivo, modo | std::ios::binary);
	if(!arquivo.is_open()){ //verifica se o arquivo foi aberto
		std::cerr << "Erro ao abrir o arquivo: " << nome_arquivo << std::endl;
		exit(1);
	}
	return arquivo;
}

void GerenciaBlocos::fechaArquivo(std::fstream &arquivo){
	arquivo.close();
}

unsigned long long GerenciaBlocos::totalDeBlocos(std::fstream &arquivo){ //calcula total de blocos
	arquivo.seekg(0, std::ios::end);
	std::streampos tamanho_arquivo = arquivo.tellg();
	return static_cast<unsigned long long>(tamanho_arquivo) / this->size_blocos; //total de blocos de acorodo com o tamanho do arquivo e tamanho dos blocos
}

void GerenciaBlocos::lerBloco(std::fstream &arquivo, int indice, char* buffer){ //le bloco do arquivo
	arquivo.seekg(indice * this->size_blocos, std::ios::beg); //posiciona o ponteiro no inicio do bloco
	arquivo.read(buffer, this->size_blocos); //le o bloco e armazena no buffer
}
