#include <iostream>
#include "../include/Hashing.hpp"

Hashing::Hashing(GerenciaBlocos* gerente){
	blocos_gerente = gerente;
	nBuckets = 512; //2^9 para fins da funcao hash
	setTabela_hash();
}

void Hashing::setTabela_hash(){
	int i;
	unsigned long alocado;
	tabela_hash = new unsigned long long[nBuckets];

	for(i = 0; i < nBuckets; i++){
		char* buffer = new char[blocos_gerente->getSize_blocos()];
		blocos_gerente->somaSize_hash(blocos_gerente->getSize_blocos());
		alocado = blocos_gerente->getSize_hash();
		tabela_hash[i] = alocado;
		blocos_gerente->escreverBloco(tabela_hash[i], buffer);
		delete[] buffer;
	}
}

unsigned long long Hashing::funcao_hash(unsigned int id){
	return (id % 2654435761U) % nBuckets; //numero primo da sugestao de multiplicacao de knuth
}

void Hashing::insereHash(registro campos){
	unsigned long long id_hash = funcao_hash(campos.id);
	unsigned long long offset, alocado;
	unsigned int tam_regs, i;
	bucket meuBucket;

	blocos_gerente->lerBucket(tabela_hash[id_hash], &meuBucket);
	if(meuBucket.nRegs < blocos_gerente->getRegperbloco()){
		tam_regs = 0;
		for(i = 0; i < meuBucket.nRegs; i++){
			tam_regs += meuBucket.registros[i].total_size;
		}
		offset = sizeof(unsigned int) + tam_regs; //4 bytes do nRegs + seja la quantos registros ja estiverem la
		blocos_gerente->escreverRegistroBloco(tabela_hash[id_hash] + offset, campos);

	} else{ //escrever no bucket de overflow
		std::cout << "Houve colisão ao inserir no bucket " << id_hash << '\n';
		if(meuBucket.endereco_overf == 0){ //cria bucket de overflow se nao existir
			blocos_gerente->somaSize_hash(sizeof(blocos_gerente->getSize_blocos()));
			alocado = blocos_gerente->getSize_hash();
			meuBucket.endereco_overf = alocado;
		}
		insereOverflow(campos, meuBucket.endereco_overf);
	}
}

void Hashing::insereOverflow(registro campos, unsigned long long endereco_over){
	unsigned long long offset, alocado;
	bucket* meuBucket;

	blocos_gerente->lerBucket(endereco_over, meuBucket);
	if(meuBucket->nRegs < blocos_gerente->getRegperbloco()){
		offset = sizeof(unsigned int) + meuBucket->nRegs * sizeof(registro); //4 bytes do nRegs + seja la quantos registros ja estiverem la
		blocos_gerente->escreverRegistroBloco(endereco_over + offset, campos);

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
	blocos_gerente->lerBucket(tabela_hash[i], meuBucket);
	while(i < meuBucket->nRegs){
		if(meuBucket->registros[i].id == id) return &meuBucket->registros[i];
		i++;
	}
	//procura nos buckets de overflow
	if(meuBucket->endereco_overf != 0) return buscaOverflow(id, meuBucket->endereco_overf);
	
	return nullptr;
}

registro* Hashing::buscaOverflow(unsigned int id, unsigned long long endereco_over){
	unsigned int i;
	bucket* meuBucket;

	blocos_gerente->lerBucket(endereco_over, meuBucket);
	while(i < meuBucket->nRegs){
		if(meuBucket->registros[i].id == id) return &meuBucket->registros[i];
		i++;
	}
	if(meuBucket->endereco_overf != 0) buscaOverflow(id, meuBucket->endereco_overf);
	
	return nullptr;
}
