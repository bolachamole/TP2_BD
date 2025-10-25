#ifndef HASHING
#define HASHING

#include <cstring>
#include "../include/GerenciaBlocos.hpp"

template<size_t N>
class Hashing{
	private:
		GerenciaBlocos* blocos_gerente;
		int nBuckets;
	
		unsigned long long* tabela_hash;
		bucket<N> bucket_vazio;

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

template<size_t N>
Hashing<N>::Hashing(GerenciaBlocos* gerente){
	blocos_gerente = gerente;
	nBuckets = 512; //2^9 para fins da funcao hash
	setBucketVazio();
	setTabela_hash();
}

template<size_t N>
void Hashing<N>::setBucketVazio(){
	bucket_vazio.nRegs = 0;
	bucket_vazio.registros = {};
	bucket_vazio.endereco_overf = 0;
}

template<size_t N>
void Hashing<N>::setTabela_hash(){
	int i;
	unsigned long alocado;
	tabela_hash = new unsigned long long[nBuckets];

	for(i=0; i < nBuckets; i++){
		blocos_gerente->setSize_alocados(sizeof(bucket<N>));
		alocado = blocos_gerente->getSize_alocados();
		tabela_hash[i] = alocado;
		blocos_gerente->escreverBloco(tabela_hash[i], &bucket_vazio);
	}
}

template<size_t N>
unsigned long long Hashing<N>::funcao_hash(unsigned int id){
	return (id % 2654435761) % nBuckets; //numero primo da sugestao de multiplicacao de knuth
}

template<size_t N>
void Hashing<N>::insereHash(registro campos){
	unsigned long long id_hash = funcao_hash(campos.id);
	unsigned long long offset, alocado;
	bucket<N> meuBucket;

	blocos_gerente->lerBloco(tabela_hash[id_hash], &meuBucket);
	if(meuBucket.nRegs <= blocos_gerente->getRegperbloco()){
		offset = sizeof(unsigned int) + meuBucket.nRegs * sizeof(registro); //4 bytes do nRegs + seja la quantos registros ja estiverem la
		blocos_gerente->escreverRegistro(tabela_hash[id_hash] + offset, campos);
	}else{ //escrever no bucket de overflow
		if(meuBucket.endereco_overf == 0){
			blocos_gerente->setSize_alocados(sizeof(bucket<N>));
			alocado = blocos_gerente->getSize_alocados();
			meuBucket.endereco_overf = alocado;
		}
		insereOverflow(campos, meuBucket.endereco_overf);
	}
}

template<size_t N>
void Hashing<N>::insereOverflow(registro campos, unsigned long long endereco_over){
	unsigned long long offset, alocado;
	bucket<N> meuBucket;

	blocos_gerente->lerBloco(endereco_over, &meuBucket);
	if(meuBucket.nRegs <= blocos_gerente->getRegperbloco()){
		offset = sizeof(unsigned int) + meuBucket.nRegs * sizeof(registro); //4 bytes do nRegs + seja la quantos registros ja estiverem la
		blocos_gerente->escreverRegistro(endereco_over + offset, campos);
	}else{ //escrever no bucket de overflow
		if(meuBucket.endereco_overf == 0){
			blocos_gerente->setSize_alocados(sizeof(bucket<N>));
			alocado = blocos_gerente->getSize_alocados();
			meuBucket.endereco_overf = alocado;
		}
		insereOverflow(campos, meuBucket.endereco_overf);
	}
}

template<size_t N>
registro* Hashing<N>::buscaHash(unsigned int id){
	unsigned long long id_buscar = funcao_hash(id);
	unsigned int i;
	bucket<N> meuBucket;
	
	//procura nos buckets normais
	blocos_gerente->lerBloco(tabela_hash[id_buscar], &meuBucket);
	while(i < meuBucket.nRegs){
		if(meuBucket.registros[i].id == id) return meuBucket.registros[i];
		i++;
	}

	//procura nos buckets de overflow
	buscaOverflow(id, meuBucket.endereco_overf);
}

template<size_t N>
registro* Hashing<N>::buscaOverflow(unsigned int id, unsigned long long endereco_over){
	unsigned int i;
	bucket<N> meuBucket;
	
	//procura nos buckets normais
	blocos_gerente->lerBloco(endereco_over, &meuBucket);
	while(i < meuBucket.nRegs){
		if(meuBucket.registros[i].id == id) return meuBucket.registros[i];
		i++;
	}
	if(meuBucket.endereco_overf != 0) buscaOverflow(id, meuBucket.endereco_overf);
	else return nullptr;
}

#endif
