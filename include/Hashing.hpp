#ifndef HASHING
#define HASHING

#include <cstdint>
#include "../include/GerenciaBlocos.hpp"

#define TAM_REGISTRO = 1542U; //tamanho maximo do registro

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
		GerenciaBlocos* blocos_gerente;
		int quant_buckets;
		int blk_per_bucket;

		std::uint64_t funcao_hash(unsigned int id);
	public:
		Hashing(GerenciaBlocos* gerente);
		void insereHash();
		registro buscaHash(unsigned int id);
};

#endif
