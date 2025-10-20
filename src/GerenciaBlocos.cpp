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
<<<<<<< HEAD

	size_blocos = tam_bloco;
}

unsigned int GerenciaBlocos::getNitems(){
	return nitems;
=======
	this->size_blocos = tam_bloco;
>>>>>>> 6a8ad93050ed84e7040ce5b1073438db32776bcc
}

unsigned int GerenciaBlocos::getSize_blocos(){
	return size_blocos;
}

//abre arquivo
std::fstream GerenciaBlocos::abreArquivo(const std::string &nome_arquivo, std::ios::openmode modo){
	std::fstream arquivo(nome_arquivo, modo | std::ios::binary);
	if(!arquivo.is_open()){ //verifica se o arquivo foi aberto
		std::cerr << "Erro ao abrir o arquivo: " << nome_arquivo << std::endl;
		exit(1);
	}
	return arquivo;
}
void GerenciaBlocos::fechaArquivo(std::fstream &arquivo){ //fecha arquivo
	arquivo.close();
}

int GerenciaBlocos::totalDeBlocos(std::fstream &arquivo){ //calcula total de blocos
	arquivo.seekg(0, std::ios::end);
	std::streampos tamanho_arquivo = arquivo.tellg();
	return static_cast<int>(tamanho_arquivo) / this->size_blocos; //total de blocos de acorodo com o tamanho do arquivo e tamanho dos blocos
}

void GerenciaBlocos::lerBloco(std::fstream &arquivo, int indice, char* buffer){ //le bloco do arquivo
	arquivo.seekg(indice * this->size_blocos, std::ios::beg); //posiciona o ponteiro no inicio do bloco
	arquivo.read(buffer, this->size_blocos); //le o bloco e armazena no buffer
}
