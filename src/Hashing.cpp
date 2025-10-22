#include "../include/Hashing.hpp"

Hashing::Hashing(GerenciaBlocos* gerente){
	blocos_gerente = gerente;
}

unsigned long long Hashing::funcao_hash(unsigned int id){
	return (id % NPRIMO) % nBuckets;
}

void Hashing::insereHash(registro* campos){

}

registro Hashing::buscaHash(unsigned int id){
	unsigned long long i = funcao_hash(id);
	
}
