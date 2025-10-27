#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <cstring>
#include "../include/BPlusTreeSecundaria.hpp"
#include "../include/GerenciaBlocos.hpp"


BplusTreeSecundaria::BplusTreeSecundaria(int ordem, GerenciaBlocos* blocos_gerente, GerenciaBlocos* dados_gerente) {
	raiz = BplusTreeSecundaria::criaNovoNo(); //cria a raiz e define-a como uma folha
	raiz->isFolha = true;
	raiz->pai = nullptr;
	this->ordem = ordem; //define a ordem da árvore
	this->blocos_gerente = blocos_gerente;
    this->dados_gerente = dados_gerente;
}

//funcao que cria um novo nó e o define como intermediário
NoBplus2* BplusTreeSecundaria::criaNovoNo() {
    NoBplus2* novoNo = new NoBplus2;
    novoNo->isFolha = false;
    novoNo->pai = nullptr;
    novoNo->proximo = nullptr;
    return novoNo;
}

//Função que insere a chave e o endereço do arquivo dentro da árvore. É o que será usado para fazer o upload dos arquivos
void BplusTreeSecundaria::insere(chave_titulo chave, unsigned long long endereco) {
    NoBplus2* no = buscaNo(chave);
    insereNaFolha(no, chave, endereco);
    if(no->chaves.size() == this->ordem) {
        NoBplus2* novoNo = criaNovoNo();
        novoNo->isFolha = true;
        novoNo->pai = no->pai;
        int meio = ceil(this->ordem / 2.0);
        novoNo->chaves.assign(no->chaves.begin() + meio, no->chaves.end());
        novoNo->enderecos.assign(no->enderecos.begin() + meio, no->enderecos.end());
        novoNo->proximo = no->proximo;
        no->chaves.resize(meio);
        no->enderecos.resize(meio);
        no->proximo = novoNo;
        inserePai(no, novoNo->chaves[0], novoNo);
    }
}

//Dado um nó folha, a sua chave e o endereço para o bloco dos dados será inserido nele
void BplusTreeSecundaria::insereNaFolha(NoBplus2* no, chave_titulo chave, unsigned long long endereco) {
    if(!no->chaves.empty()) {
        for(int i = 0; i < no->chaves.size(); i++) {
            if(chave.tam_titulo < no->chaves[i].tam_titulo){
                no->chaves.insert(no->chaves.begin() + i, chave);
                no->enderecos.insert(no->enderecos.begin() + i, endereco);
                break;
            }
            if(i + 1 == no->chaves.size()) {
                no->chaves.push_back(chave);
                no->enderecos.push_back(endereco);
                break;
            }
        }
    }
    else {
        no->chaves.push_back(chave);
        no->enderecos.push_back(endereco);
    }
}


//Função usada pra inserir um valor nos nós pais e, caso o tamanho ultrapasse a ordem, dividí-los
void BplusTreeSecundaria::inserePai(NoBplus2* noEsq, chave_titulo chave, NoBplus2* noDir) {
    if (this->raiz == noEsq) {
        NoBplus2* novaRaiz = criaNovoNo();
        novaRaiz->chaves.push_back(chave);
        novaRaiz->filhos.push_back(noEsq);
        novaRaiz->filhos.push_back(noDir);
        this->raiz = novaRaiz;
        noDir->pai = novaRaiz;
        noEsq->pai = novaRaiz;
        return;
    } 
    NoBplus2* pai = noEsq->pai;
    for(int i = 0; i < pai->filhos.size(); i++) {
        if (pai->filhos[i] == noEsq) {
            pai->chaves.insert(pai->chaves.begin() + i, chave);
            pai->filhos.insert(pai->filhos.begin() + i + 1, noDir);
            if (pai->filhos.size() > this->ordem) {
                NoBplus2* paiDir = criaNovoNo();
                paiDir->pai = pai->pai;
                int meio = ceil((this->ordem+1) / 2.0);
                paiDir->chaves.assign(pai->chaves.begin() + meio, pai->chaves.end());
                paiDir->filhos.assign(pai->filhos.begin() + meio, pai->filhos.end());
                chave_titulo chaveMeio = pai->chaves[meio-1];
                pai->chaves.resize(meio-1);
                pai->filhos.resize(meio);
                for (NoBplus2* filho : paiDir->filhos) {
                    filho->pai = paiDir;
                }
                inserePai(pai, chaveMeio, paiDir);
            }
            break;
        }
    }
}

//Função de buscar um nó folha da árvore
NoBplus2* BplusTreeSecundaria::buscaNo(chave_titulo chave) {
    NoBplus2* no = raiz;
    while(!no->isFolha) {
        for(int i = 0; i < no->chaves.size(); i++) {
            if (chave.tam_titulo == no->chaves[i].tam_titulo) {
                no = no->filhos[i+1];
                break;
            }
            else if (chave.tam_titulo < no->chaves[i].tam_titulo) {
                no = no->filhos[i];
                break;
            }
            else if (i + 1 == no->chaves.size()) {
                no = no->filhos[i+1];
                break;
            }
        }
    }
    return no;
    
}

void BplusTreeSecundaria::criaIdexSecundario(){
    bucket meuBucket;
    chave_titulo chave;
    unsigned long long endereco;
    unsigned int i;

    endereco = dados_gerente->getSize_blocos();

    while(endereco <= dados_gerente->getSize_hash()){
    	dados_gerente->lerBucket(endereco, &meuBucket);
        for(i = 0; i < meuBucket.nRegs; i++){
            strcpy(chave.titulo, meuBucket.registros[i].titulo);
            chave.tam_titulo = meuBucket.registros[i].tam_titulo;
            insere(chave, endereco);
        }
    }
    blocos_gerente->escreverBloco(0, reinterpret_cast<char*>(raiz));
}