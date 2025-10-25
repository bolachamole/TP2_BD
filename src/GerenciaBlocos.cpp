#include <sys/stat.h>
#include <iostream>
#include "../include/GerenciaBlocos.hpp"

GerenciaBlocos::GerenciaBlocos(std::string nome_arquivo){
	this->nome_arquivo = nome_arquivo.c_str();
	setSize_blocos();
	reg_per_bloco = size_blocos / sizeof(registro);
	size_alocados = 0;
}

void GerenciaBlocos::setSize_blocos(){
	struct stat infostat;
	unsigned int tam_bloco;

	if(lstat(nome_arquivo,&infostat) == -1){
		std::cout << "Erro ao tentar pegar o tamanho dos blocos.";
		exit(1);
	}
	tam_bloco = (unsigned int)infostat.st_blksize;

	this->size_blocos = tam_bloco;
}

void GerenciaBlocos::setSize_alocados(unsigned long long soma){
	size_alocados += soma;
}

unsigned int GerenciaBlocos::getSize_blocos(){
	return this->size_blocos;
}

unsigned int GerenciaBlocos::getRegperbloco(){
	return this->reg_per_bloco;
}

unsigned long long GerenciaBlocos::getSize_alocados(){
	return this->size_alocados;
}

void GerenciaBlocos::abreArquivo(std::ios::openmode modo){
	arquivo.open(nome_arquivo, modo | std::ios::binary);
	if(!arquivo.is_open()){ //verifica se o arquivo foi aberto
		std::cerr << "Erro ao abrir o arquivo: " << nome_arquivo << std::endl;
		exit(1);
	}
}

void GerenciaBlocos::fechaArquivo(){
	arquivo.close();
}

unsigned int GerenciaBlocos::totalDeBlocos(){ //calcula total de blocos
	arquivo.seekg(0, std::ios::end);
	std::streampos tamanho_arquivo = arquivo.tellg();
	return static_cast<unsigned int>(tamanho_arquivo) / this->size_blocos; //total de blocos de acorodo com o tamanho do arquivo e tamanho dos blocos
}

void GerenciaBlocos::escreverBloco(unsigned long long endereco, void* bloco){
	arquivo.seekg(endereco, std::ios::beg); //posiciona o ponteiro no inicio do bloco
	arquivo.write(reinterpret_cast<char*>(&bloco), this->size_blocos); //escreve no bloco
	arquivo.clear(); //limpa flags
}

void GerenciaBlocos::lerBloco(unsigned long long endereco, void* buffer){ //le bloco do arquivo
	arquivo.seekg(endereco, std::ios::beg); //posiciona o ponteiro no inicio do bloco
	arquivo.read(reinterpret_cast<char*>(&buffer), this->size_blocos); //le o bloco e armazena no buffer
	arquivo.clear();
}

void GerenciaBlocos::escreverRegistro(unsigned long long endereco, registro campos){
	arquivo.seekg(endereco, std::ios::beg); //posiciona o ponteiro no inicio da posicao onde por o registro
	arquivo.write(reinterpret_cast<char*>(&campos), sizeof(registro));
	arquivo.clear();
}
