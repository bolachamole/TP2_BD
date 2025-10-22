#ifndef HASHING
#define HASHING

#include "../include/GerenciaBlocos.hpp"

#define NPRIMO 2654435761

class Hashing{
	private:
		GerenciaBlocos* blocos_gerente;
		int nBuckets;
		int reg_per_blk;
		int blk_per_bucket;

		struct bucket{
			bloco_dados* blocos;
			struct bucket* prox;
		};

		struct bucket_overflow{
			bloco_dados* blocos;
		};

		unsigned long long funcao_hash(unsigned int id);
	public:
		Hashing(GerenciaBlocos* gerente);
		void insereHash(registro* campos);
		registro buscaHash(unsigned int id);
};

#endif
