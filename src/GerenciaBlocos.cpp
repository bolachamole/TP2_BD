#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include "../include/GerenciaBlocos.hpp"

GerenciaBlocos::GerenciaBlocos(std::string nome_arquivo){
	this->nome_arquivo = nome_arquivo.c_str();
	setSize_blocos();
	regperbloco = size_blocos / sizeof(registro);
	size_hash = 0;
	abreArquivo();
	setMapaAddr(0);
}

void GerenciaBlocos::setSize_blocos(){
	struct stat infostat;
	unsigned int tam_bloco;

	if(stat(nome_arquivo,&infostat) == -1){
		std::cerr << "Erro ao tentar pegar o tamanho dos blocos.";
		throw std::runtime_error("stat() falhou.");
	}
	tam_bloco = (unsigned int)infostat.st_blksize;
	quant_bloco = (unsigned int)infostat.st_blocks;

	this->size_blocos = tam_bloco;
}

void GerenciaBlocos::setMapaAddr(unsigned long long offset){
	if (aloc_mapa > (quant_bloco - offset)){ //se o tamanho da area mapeada estiver maior que a area desejada
		aloc_mapa = (quant_bloco - offset) * size_blocos;
	} else{
		aloc_mapa = quant_bloco * size_blocos;
	}
	mapaAddr = mmap(nullptr, aloc_mapa, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset);
	if (mapaAddr == MAP_FAILED){
		std::cerr << "Erro ao tentar mapear arquivo na memória.";
		throw std::runtime_error("mmap() falhou.");
	}
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
	fd = open(nome_arquivo, O_RDWR | O_CREAT, 0600);
	if(fd == -1){ //verifica se o arquivo foi aberto
		std::cerr << "Erro ao abrir o arquivo: " << nome_arquivo << std::endl;
		throw std::runtime_error("open() falhou.");
	}
}

void GerenciaBlocos::abreArquivoFstream(){
	arquivo.open(nome_arquivo, std::ios::in | std::ios::out | std::ios::binary);
	if(!arquivo.is_open()){ //verifica se o arquivo foi aberto
		std::cerr << "Erro ao abrir o arquivo: " << nome_arquivo << std::endl;
		throw std::runtime_error("std::fstream::open() falhou.");
	}
}

void GerenciaBlocos::fechaArquivo(){
	close(fd);
}

void GerenciaBlocos::fechaArquivoFstream(){
	arquivo.close();
}

unsigned int GerenciaBlocos::totalDeBlocosArquivo(){ //calcula total de blocos
	arquivo.seekg(0, std::ios::end);
	std::streampos tamanho_arquivo = arquivo.tellg();
	return static_cast<unsigned int>(tamanho_arquivo) / this->size_blocos; //total de blocos de acorodo com o tamanho do arquivo e tamanho dos blocos
}

void GerenciaBlocos::escreverBlocoMemoria(unsigned long long endereco, void* bloco){
	unsigned long long endereco_em_mem;

	if (endereco < aloc_mapa){
		sincronizaMapa();
		if (munmap(mapaAddr, aloc_mapa) == 1){
			std::cerr << "Erro ao tentar desmapear arquivo na memória.";
			throw std::runtime_error("munmap() falhou.");
		}
		setMapaAddr(endereco);
	}
	endereco_em_mem = (endereco - aloc_mapa) * size_blocos;
	memcpy(static_cast<char*>(mapaAddr) + endereco, bloco, this->size_blocos);
	blocos_escritos++;
}

void GerenciaBlocos::escreverBucketDisco(unsigned long long endereco, bucket bloco){
	int i;
	arquivo.seekg(endereco, std::ios::beg); //posiciona o ponteiro no inicio do bloco
	
	arquivo.write(reinterpret_cast<char*>(&bloco.nRegs), sizeof(unsigned int));
	for(i = 0; i < bloco.nRegs; i++){
		arquivo.write(reinterpret_cast<char*>(&bloco.registros[i].total_size), sizeof(unsigned int));
		arquivo.write(reinterpret_cast<char*>(&bloco.registros[i].id), sizeof(registro));
		arquivo.write(reinterpret_cast<char*>(&bloco.registros[i].tam_titulo), sizeof(unsigned int));
		arquivo.write(reinterpret_cast<char*>(&bloco.registros[i].titulo), bloco.registros[i].tam_titulo * sizeof(char));
		arquivo.write(reinterpret_cast<char*>(&bloco.registros[i].ano), sizeof(unsigned int));
		arquivo.write(reinterpret_cast<char*>(&bloco.registros[i].tam_autores), sizeof(unsigned int));
		arquivo.write(reinterpret_cast<char*>(&bloco.registros[i].autores), bloco.registros[i].tam_autores * sizeof(char));
		arquivo.write(reinterpret_cast<char*>(&bloco.registros[i].citacoes), sizeof(unsigned int));
		arquivo.write(reinterpret_cast<char*>(&bloco.registros[i].atualizacao), 20 * sizeof(char));
		arquivo.write(reinterpret_cast<char*>(&bloco.registros[i].tam_snippet), sizeof(unsigned int));
		arquivo.write(reinterpret_cast<char*>(&bloco.registros[i].snippet), bloco.registros[i].tam_snippet * sizeof(char));
	}
	arquivo.write(reinterpret_cast<char*>(&bloco.endereco_overf), sizeof(unsigned int));

	if (arquivo.fail()){
		std::cerr << "Erro ao tentar escrever no bloco " << endereco << std::endl;
		throw std::runtime_error("std::ostream::write() falhou.");
	}
	arquivo.flush(); //garante que escreveu no disco

	arquivo.clear(); //limpa flags
	blocos_escritos++;
}

void* GerenciaBlocos::lerBlocoMemoria(unsigned long long endereco){
	void* buffer = malloc(this->size_blocos);
	blocos_lidos++;
	memcpy(buffer, static_cast<char*>(mapaAddr) + endereco, this->size_blocos);

	return buffer;
}

void GerenciaBlocos::lerBucketDisco(unsigned long long endereco, bucket buffer){ //le bloco do arquivo
	int i;
	arquivo.seekg(endereco, std::ios::beg); //posiciona o ponteiro no inicio do bloco

	arquivo.read(reinterpret_cast<char*>(&buffer.nRegs), sizeof(unsigned int));
	for(i = 0; i < buffer.nRegs; i++){
		arquivo.read(reinterpret_cast<char*>(&buffer.registros[i].total_size), sizeof(unsigned int));
		arquivo.read(reinterpret_cast<char*>(&buffer.registros[i].id), sizeof(registro));
		arquivo.read(reinterpret_cast<char*>(&buffer.registros[i].tam_titulo), sizeof(unsigned int));
		arquivo.read(reinterpret_cast<char*>(&buffer.registros[i].titulo), buffer.registros[i].tam_titulo * sizeof(char));
		arquivo.read(reinterpret_cast<char*>(&buffer.registros[i].ano), sizeof(unsigned int));
		arquivo.read(reinterpret_cast<char*>(&buffer.registros[i].tam_autores), sizeof(unsigned int));
		arquivo.read(reinterpret_cast<char*>(&buffer.registros[i].autores), buffer.registros[i].tam_autores * sizeof(char));
		arquivo.read(reinterpret_cast<char*>(&buffer.registros[i].citacoes), sizeof(unsigned int));
		arquivo.read(reinterpret_cast<char*>(&buffer.registros[i].atualizacao), 20 * sizeof(char));
		arquivo.read(reinterpret_cast<char*>(&buffer.registros[i].tam_snippet), sizeof(unsigned int));
		arquivo.read(reinterpret_cast<char*>(&buffer.registros[i].snippet), buffer.registros[i].tam_snippet * sizeof(char));
	}
	arquivo.read(reinterpret_cast<char*>(&buffer.endereco_overf), sizeof(unsigned int));

	if (arquivo.fail() && !(arquivo.eof())){
		std::cerr << "Erro ao tentar ler o bloco " << endereco << std::endl;
		throw std::runtime_error("std::istream::read() falhou.");
	}

	arquivo.clear();
	blocos_lidos++;
}

void GerenciaBlocos::escreverRegistroMemoria(unsigned long long endereco, registro campos){
	memcpy(endereco + reinterpret_cast<char*>(mapaAddr), &campos.total_size, sizeof(unsigned int));
	memcpy(endereco + reinterpret_cast<char*>(mapaAddr), &campos.id, sizeof(unsigned int));
	memcpy(endereco + reinterpret_cast<char*>(mapaAddr), &campos.tam_titulo, sizeof(unsigned int));
	memcpy(endereco + reinterpret_cast<char*>(mapaAddr), &campos.titulo, campos.tam_titulo * sizeof(char));
	memcpy(endereco + reinterpret_cast<char*>(mapaAddr), &campos.ano, sizeof(unsigned int));
	memcpy(endereco + reinterpret_cast<char*>(mapaAddr), &campos.tam_autores, sizeof(unsigned int));
	memcpy(endereco + reinterpret_cast<char*>(mapaAddr), &campos.autores, campos.tam_autores * sizeof(char));
	memcpy(endereco + reinterpret_cast<char*>(mapaAddr), &campos.citacoes, sizeof(unsigned int));
	memcpy(endereco + reinterpret_cast<char*>(mapaAddr), &campos.atualizacao, 20 * sizeof(char));
	memcpy(endereco + reinterpret_cast<char*>(mapaAddr), &campos.tam_snippet, sizeof(unsigned int));
	memcpy(endereco + reinterpret_cast<char*>(mapaAddr), &campos.snippet, campos.tam_snippet * sizeof(char));
}

void GerenciaBlocos::sincronizaMapa(){
	if (msync(mapaAddr, aloc_mapa, MS_SYNC) == 1){
		std::cerr << "Erro ao tentar sincronizar memória e disco.\n";
		throw std::runtime_error("msync() falhou.");
	}
}