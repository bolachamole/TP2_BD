#include <iostream>
#include "../include/Hashing.hpp"

Hashing::Hashing(GerenciaBlocos* gerente){
	blocos_gerente = gerente;
	nBuckets = 512; //2^9 para fins da funcao hash
	setBucketVazio();
	setTabela_hash();
}

void Hashing::setBucketVazio(){
	int i;
	bucket_vazio.nRegs = 0;
	for(i = 0; i < blocos_gerente->getRegperbloco(); i++){
		bucket_vazio.registros[i] = registro{};
	}
	bucket_vazio.endereco_overf = 0;
}

void Hashing::setTabela_hash(){
	int i;
	unsigned long alocado;
	tabela_hash = new unsigned long long[nBuckets];

	for(i = 0; i < nBuckets; i++){
		blocos_gerente->somaSize_hash(sizeof(blocos_gerente->getSize_blocos()));
		alocado = blocos_gerente->getSize_hash();
		tabela_hash[i] = alocado;
		blocos_gerente->escreverBlocoMemoria(tabela_hash[i], &bucket_vazio);
	}
}

unsigned long long Hashing::funcao_hash(unsigned int id){
	return (id % 2654435761U) % nBuckets; //numero primo da sugestao de multiplicacao de knuth
}

void Hashing::insereHash(registro campos){
	unsigned long long id_hash = funcao_hash(campos.id);
	unsigned long long offset, alocado;
	unsigned int tam_regs, i;
	bucket* meuBucket;

	meuBucket = static_cast<bucket*>(blocos_gerente->lerBlocoMemoria(tabela_hash[id_hash]));
	if(meuBucket->nRegs < blocos_gerente->getRegperbloco()){
		tam_regs = 0;
		for(i = 0; i < meuBucket->nRegs; i++){
			tam_regs += meuBucket->registros[i].total_size;
		}
		offset = sizeof(unsigned int) + tam_regs; //4 bytes do nRegs + seja la quantos registros ja estiverem la
		blocos_gerente->escreverRegistroMemoria(tabela_hash[id_hash] + offset, campos);

	} else{ //escrever no bucket de overflow
		std::cout << "Houve colisão ao inserir no bucket " << id_hash << '\n';
		if(meuBucket->endereco_overf == 0){ //cria bucket de overflow se nao existir
			blocos_gerente->somaSize_hash(sizeof(blocos_gerente->getSize_blocos()));
			alocado = blocos_gerente->getSize_hash();
			meuBucket->endereco_overf = alocado;
		}
		insereOverflow(campos, meuBucket->endereco_overf);
	}
}

void Hashing::insereOverflow(registro campos, unsigned long long endereco_over){
	unsigned long long offset, alocado;
	bucket* meuBucket;

	meuBucket = static_cast<bucket*>(blocos_gerente->lerBlocoMemoria(endereco_over));
	if(meuBucket->nRegs < blocos_gerente->getRegperbloco()){
		offset = sizeof(unsigned int) + meuBucket->nRegs * sizeof(registro); //4 bytes do nRegs + seja la quantos registros ja estiverem la
		blocos_gerente->escreverRegistroMemoria(endereco_over + offset, campos);

	} else{ //escrever no bucket de overflow
		if(meuBucket->endereco_overf == 0){ //cria bucket de overflow se nao existir
			blocos_gerente->somaSize_hash(sizeof(blocos_gerente->getSize_blocos()));
			alocado = blocos_gerente->getSize_hash();
			meuBucket->endereco_overf = alocado;
		}
		insereOverflow(campos, meuBucket->endereco_overf);
	}
}

registro* Hashing::buscaHash(unsigned int id){
	unsigned long long id_buscar = funcao_hash(id);
	unsigned int i;
	bucket* meuBucket;
	
	//procura nos buckets normais
	meuBucket = static_cast<bucket*>(blocos_gerente->lerBlocoMemoria(tabela_hash[id_buscar]));
	while(i < meuBucket->nRegs){
		if(meuBucket->registros[i].id == id) return &meuBucket->registros[i];
		i++;
	}

	//procura nos buckets de overflow
	return buscaOverflow(id, meuBucket->endereco_overf);
}

registro* Hashing::buscaOverflow(unsigned int id, unsigned long long endereco_over){
	unsigned int i;
	bucket* meuBucket;

	meuBucket = static_cast<bucket*>(blocos_gerente->lerBlocoMemoria(endereco_over));
	while(i < meuBucket->nRegs){
		if(meuBucket->registros[i].id == id) return &meuBucket->registros[i];
		i++;
	}
	if(meuBucket->endereco_overf != 0) buscaOverflow(id, meuBucket->endereco_overf);
	
	return nullptr;
}

void Hashing::hash2Disco(){
	int i;
	bucket* meuBucket;

	blocos_gerente->abreArquivoFstream();

	for(i = 0; i < nBuckets; i++){
		meuBucket = static_cast<bucket*>(blocos_gerente->lerBlocoMemoria(tabela_hash[i]));
		blocos_gerente->escreverBucketDisco(tabela_hash[i], *(meuBucket));
	}

	blocos_gerente->sincronizaMapa();
	blocos_gerente->fechaArquivoFstream();
}

void Hashing::disco2Hash(){
	int i, pos;
	bucket meuBucket;

	blocos_gerente->abreArquivoFstream();
	pos = 0;

	for(i = 0; i < nBuckets; i++){
		blocos_gerente->lerBucketDisco(pos, meuBucket);
		pos += blocos_gerente->getSize_blocos();
		tabela_hash[i] = pos;
		blocos_gerente->escreverBlocoMemoria(tabela_hash[i], &meuBucket);
	}

	blocos_gerente->fechaArquivoFstream();
}