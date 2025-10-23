#include "../include/Hashing.hpp"

Hashing::Hashing(GerenciaBlocos* gerente){
	blocos_gerente = gerente;
	nBuckets = 1000; //arbitrario
	blk_per_bucket = gerente->getSize_blocos() / nBuckets;
	tabela_hash = new bucket[nBuckets];
}

unsigned long long Hashing::funcao_hash(unsigned int id){ //sugestao de multiplicacao de knuth
	return (id % 2654435761) % nBuckets;
}

void Hashing::insereHash(registro* campos){
	
}

registro* Hashing::buscaHash(unsigned int id){
	unsigned long long id_buscar = funcao_hash(id);
	registro* meuResgitro;

	

	return meuResgitro;
}

void Hashing::salvaArqDados(){

}
