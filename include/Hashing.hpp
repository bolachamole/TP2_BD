#ifndef HASHING
#define HASHING

#include "../include/GerenciaBlocos.hpp"

class Hashing{
	private:
		GerenciaBlocos* blocos_gerente;
		int nBuckets;
	
		unsigned long long* tabela_hash;
		bucket bucket_vazio;

		unsigned long long funcao_hash(unsigned int id);
	public:
		Hashing(GerenciaBlocos* gerente);
		void setBucketVazio();
		void setTabela_hash();
		void insereHash(registro campos);
		void insereOverflow(registro campos, unsigned long long endereco_over);
		registro* buscaHash(unsigned int id);
		registro* buscaOverflow(unsigned int id, unsigned long long endereco_over);
};

#endif
