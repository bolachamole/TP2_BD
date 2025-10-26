#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <queue>
#include "../include/BPlusTree.hpp"
#include "../include/GerenciaBlocos.hpp"


BplusTree::BplusTree(int ordem) {
    raiz = BplusTree::criaNovoNo(); //cria a raiz e define-a como uma folha
    raiz->isFolha = true;
    raiz->pai = nullptr;
    this->ordem = ordem; //define a ordem da árvore
}

//funcao que cria um novo nó e o define como intermediário
NoBplus* BplusTree::criaNovoNo() {
    NoBplus* novoNo = new NoBplus;
    novoNo->isFolha = false;
    novoNo->pai = nullptr;
    novoNo->proximo = nullptr;
    return novoNo;
}

//Função que insere a chave e o endereço do arquivo dentro da árvore. É o que será usado para fazer o upload dos arquivos
void BplusTree::insere(unsigned int chave, unsigned long long endereco) {
    NoBplus* no = buscaNo(chave);
    insereNaFolha(no, chave, endereco);
    if(no->chaves.size() == this->ordem) {
        NoBplus* novoNo = criaNovoNo();
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
void BplusTree::insereNaFolha(NoBplus* no, unsigned int chave, unsigned long long endereco) {
    if(!no->chaves.empty()) {
        for(int i = 0; i < no->chaves.size(); i++) {
            if(chave < no->chaves[i]){
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
    //salva bloco em memoria secundaria
}


//Função usada pra inserir um valor nos nós pais e, caso o tamanho ultrapasse a ordem, dividí-los
void BplusTree::inserePai(NoBplus* noEsq, unsigned int chave, NoBplus* noDir) {
    if (this->raiz == noEsq) {
        NoBplus* novaRaiz = criaNovoNo();
        novaRaiz->chaves.push_back(chave);
        novaRaiz->filhos.push_back(noEsq);
        novaRaiz->filhos.push_back(noDir);
        this->raiz = novaRaiz;
        noDir->pai = novaRaiz;
        noEsq->pai = novaRaiz;
        return;
    } 
    NoBplus* pai = noEsq->pai;
    for(int i = 0; i < pai->filhos.size(); i++) {
        if (pai->filhos[i] == noEsq) {
            pai->chaves.insert(pai->chaves.begin() + i, chave);
            pai->filhos.insert(pai->filhos.begin() + i + 1, noDir);
            if (pai->filhos.size() > this->ordem) {
                NoBplus* paiDir = criaNovoNo();
                paiDir->pai = pai->pai;
                int meio = ceil((this->ordem+1) / 2.0);
                paiDir->chaves.assign(pai->chaves.begin() + meio, pai->chaves.end());
                paiDir->filhos.assign(pai->filhos.begin() + meio, pai->filhos.end());
                unsigned int chaveMeio = pai->chaves[meio-1];
                pai->chaves.resize(meio-1);
                pai->filhos.resize(meio);
                for (NoBplus* filho : paiDir->filhos) {
                    filho->pai = paiDir;
                }
                inserePai(pai, chaveMeio, paiDir);
            }
            break;
        }
    }
}

//Função de buscar um nó folha da árvore
NoBplus* BplusTree::buscaNo(unsigned int chave) {
    NoBplus* no = raiz;
    while(!no->isFolha) {
        for(int i = 0; i < no->chaves.size(); i++) {
            if (chave == no->chaves[i]) {
                no = no->filhos[i+1];
                break;
            }
            else if (chave < no->chaves[i]) {
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

//funcao usada pra imprimir o conteúdo das árvores e foi usada nos testes da árvore  
void BplusTree::imprimeArvore() {
    std::queue<NoBplus*> enderecos;
    enderecos.push(raiz);
    while(!enderecos.empty()) {
        NoBplus* noAtual = enderecos.front();
        enderecos.pop();
        if(!noAtual->isFolha) {
            std::cout<<"no nó intermediário atual, temos "<<noAtual->filhos.size()<<" filhos, com as seguintes chaves:\n";
            for(int i = 0; i < noAtual->chaves.size(); i++) {
                std::cout<<noAtual->chaves[i]<<" ";
                enderecos.push(noAtual->filhos[i]);
            }
            std::cout<<"\n";
            enderecos.push(noAtual->filhos.back());
        }
        else {
            std::cout<<"o nó folha seguinte tem as chaves para os seguintes endereços:\n";
            for(int i = 0; i < noAtual->chaves.size(); i++) {
                std::cout<<noAtual->chaves[i]<<" ";
            }
            std::cout<<"\n";
        }
    }

}



