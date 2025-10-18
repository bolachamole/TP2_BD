#ifndef HASHING
#define HASHING

#include <cstdint>
#include <vector>
#include "../include/GerenciaBlocos.hpp"

struct registro{
	int id;
	unsigned int tam_titulo;
	char titulo[300];
	int ano;
	unsigned int tam_autores;
	char autores[150];
	int citacoes;
	char atualizacao[20];
	unsigned int tam_snippet;
	char* snippet;
};

class Hashing{
	private:
		int tam_vetor;
		std::vector<int> blocos;
		
		std::uint64_t funcao_hash(unsigned int id);
	public:
		Hashing(){
			tam_vetor = 0;
		}
		void insereHash();
		registro buscaHash(unsigned int id);
};

#endif
