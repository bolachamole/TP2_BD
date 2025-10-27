#include <sys/stat.h>
#include <iostream>
#include <stdexcept>
#include "../include/GerenciaBlocos.hpp"
#include "../include/LogLevels.hpp"

GerenciaBlocos::GerenciaBlocos(std::string nome_arquivo){
	this->nome_arquivo = nome_arquivo.c_str();
	setSize_blocos();
	regperbloco = size_blocos / sizeof(registro);
	size_hash = 0;
	abreArquivo();
}

void GerenciaBlocos::setSize_blocos(){
	struct stat infostat;
	unsigned int tam_bloco;

	if(stat("data/input.csv", &infostat) == -1){
		LogLevels::logErro("Erro ao tentar pegar o tamanho dos blocos.");
		throw std::runtime_error("stat() falhou.");
	}
	tam_bloco = (unsigned int)infostat.st_blksize;

	this->size_blocos = tam_bloco;
}

void GerenciaBlocos::somaSize_hash(unsigned long long soma){
	size_hash += soma;
}

unsigned int GerenciaBlocos::getSize_blocos(){
	return size_blocos;
}

unsigned int GerenciaBlocos::getRegperbloco(){
	return regperbloco;
}

unsigned long long GerenciaBlocos::getSize_hash(){
	return size_hash;
}

int GerenciaBlocos::getBlocos_lidos(){
	return blocos_lidos;
}

int GerenciaBlocos::getBlocos_escritos(){
	return blocos_escritos;
}

void GerenciaBlocos::abreArquivo(){
	arquivo.open(nome_arquivo, std::ios::in | std::ios::out | std::ios::binary);
	if(!arquivo.is_open()){ //verifica se o arquivo foi aberto
		LogLevels::logErro("Erro ao abrir o arquivo " + std::string(nome_arquivo));
		throw std::runtime_error("std::fstream::open() falhou.");
	}
}

void GerenciaBlocos::fechaArquivo(){
	arquivo.close();
}

unsigned int GerenciaBlocos::totalDeBlocosArquivo(){ //calcula total de blocos
	arquivo.seekg(0, std::ios::end);
	std::streampos tamanho_arquivo = arquivo.tellg();
	return static_cast<unsigned int>(tamanho_arquivo) / this->size_blocos; //total de blocos de acorodo com o tamanho do arquivo e tamanho dos blocos
}

void GerenciaBlocos::escreverBloco(unsigned long long endereco, char* bloco){
	int i;
	arquivo.seekg(endereco, std::ios::beg); //posiciona o ponteiro no inicio do bloco
	arquivo.write(bloco, this->size_blocos); //escreve no bloco
	if (arquivo.fail()){
		LogLevels::logErro("Erro ao tentar escrever no bloco " + std::to_string(endereco));
		throw std::runtime_error("std::ostream::write() falhou.");
	}
	arquivo.flush(); //garante que escreveu no disco

	arquivo.clear(); //limpa flags
	blocos_escritos++;
}

void GerenciaBlocos::lerBloco(unsigned long long endereco, char* buffer){ //le bloco do arquivo
	arquivo.seekg(endereco, std::ios::beg); //posiciona o ponteiro no inicio do bloco
	arquivo.read(buffer, this->size_blocos); //le o bloco e armazena no buffer
	if (arquivo.fail() && !(arquivo.eof())){
		LogLevels::logErro("Erro ao tentar ler o bloco " + endereco);
		throw std::runtime_error("std::istream::read() falhou.");
	}
	arquivo.clear();
}

void GerenciaBlocos::lerBucket(unsigned long long endereco, bucket* buffer){ //le bloco do arquivo
	int i;
	arquivo.seekg(endereco, std::ios::beg); //posiciona o ponteiro no inicio do bloco

	arquivo.read(reinterpret_cast<char*>(buffer->nRegs), sizeof(unsigned int));
	for(i = 0; i < buffer->nRegs; i++){
		arquivo.read(reinterpret_cast<char*>(buffer->registros[i].total_size), sizeof(unsigned int));
		arquivo.read(reinterpret_cast<char*>(buffer->registros[i].id), sizeof(registro));
		arquivo.read(reinterpret_cast<char*>(buffer->registros[i].tam_titulo), sizeof(unsigned int));
		arquivo.read(reinterpret_cast<char*>(buffer->registros[i].titulo), buffer->registros[i].tam_titulo * sizeof(char));
		arquivo.read(reinterpret_cast<char*>(buffer->registros[i].ano), sizeof(unsigned int));
		arquivo.read(reinterpret_cast<char*>(buffer->registros[i].tam_autores), sizeof(unsigned int));
		arquivo.read(reinterpret_cast<char*>(buffer->registros[i].autores), buffer->registros[i].tam_autores * sizeof(char));
		arquivo.read(reinterpret_cast<char*>(buffer->registros[i].citacoes), sizeof(unsigned int));
		arquivo.read(reinterpret_cast<char*>(buffer->registros[i].atualizacao), 20 * sizeof(char));
		arquivo.read(reinterpret_cast<char*>(buffer->registros[i].tam_snippet), sizeof(unsigned int));
		arquivo.read(reinterpret_cast<char*>(buffer->registros[i].snippet), buffer->registros[i].tam_snippet * sizeof(char));
	}
	arquivo.read(reinterpret_cast<char*>(buffer->endereco_overf), sizeof(unsigned int));

	if (arquivo.fail() && !(arquivo.eof())){
		LogLevels::logErro("Erro ao tentar ler o bloco " + std::to_string(endereco));
		throw std::runtime_error("std::istream::read() falhou.");
	}

	arquivo.clear();
	blocos_lidos++;
}

void GerenciaBlocos::escreverRegistroBloco(unsigned long long endereco, registro campos){
	arquivo.seekg(endereco, std::ios::beg);
	arquivo.write(reinterpret_cast<char*>(&campos.total_size), sizeof(unsigned int));
	arquivo.write(reinterpret_cast<char*>(&campos.id), sizeof(unsigned int));
	arquivo.write(reinterpret_cast<char*>(&campos.tam_titulo), sizeof(unsigned int));
	arquivo.write(reinterpret_cast<char*>(&campos.titulo), campos.tam_titulo * sizeof(char));
	arquivo.write(reinterpret_cast<char*>(&campos.ano), sizeof(unsigned int));
	arquivo.write(reinterpret_cast<char*>(&campos.tam_autores), sizeof(unsigned int));
	arquivo.write(reinterpret_cast<char*>(&campos.autores), campos.tam_autores * sizeof(char));
	arquivo.write(reinterpret_cast<char*>(&campos.citacoes), sizeof(unsigned int));
	arquivo.write(reinterpret_cast<char*>(&campos.atualizacao), 20 * sizeof(char));
	arquivo.write(reinterpret_cast<char*>(&campos.tam_snippet), sizeof(unsigned int));
	if (arquivo.fail()){
		LogLevels::logErro("Erro ao tentar escrever registro " + std::to_string(campos.id) + " no bloco.");
		throw std::runtime_error("std::ostream::write() falhou.");
	}
	arquivo.write(reinterpret_cast<char*>(&campos.snippet), campos.tam_snippet * sizeof(char));
	arquivo.flush();
	arquivo.clear();
}
