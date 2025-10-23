#ifndef HASHING
#define HASHING

#include "../include/GerenciaBlocos.hpp"

class Hashing{
	private:
		GerenciaBlocos* blocos_gerente;
		int nBuckets;
		int blk_per_bucket;

		struct p_registro{
			unsigned long long endereco_blk;
			unsigned int pos_reg;
		};
		
		struct bucket{
			unsigned long long** chaves; //enderecos do blocos
			p_registro* ponteiro;
		};

		bucket* tabela_hash;

		unsigned long long funcao_hash(unsigned int id);
	public:
		Hashing(GerenciaBlocos* gerente);
		void insereHash(registro* campos);
		registro* buscaHash(unsigned int id);
		void salvaArqDados();
};

#endif
