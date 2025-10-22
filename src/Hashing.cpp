#include "../include/Hashing.hpp"

Hashing::Hashing(GerenciaBlocos* gerente){
	blocos_gerente = gerente;
	nBuckets = 1000;
	reg_per_blk = gerente->getSize_blocos() / TAM_MAX_REGISTRO;
	blk_per_bucket = gerente->getSize_blocos() / nBuckets;
}

unsigned long long Hashing::funcao_hash(unsigned int id){
	return (id % NPRIMO) % nBuckets;
}

void Hashing::insereHash(registro* campos){
	
}

registro Hashing::buscaHash(unsigned int id){
	unsigned long long id_buscar = funcao_hash(id);
	registro meuResgitro;

	return meuResgitro;
}
