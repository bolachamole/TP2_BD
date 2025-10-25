#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <queue>
#include "../include/BPlusTree.hpp"
#include "../include/GerenciaBlocos.hpp"


BplusTree::BplusTree(int ordem) {
    raiz = BplusTree::criaNovoNo();
    raiz->isFolha = true;
    raiz->pai = nullptr;
    this->ordem = ordem;
}

NoBplus* BplusTree::criaNovoNo() {
    NoBplus* novoNo = new NoBplus;
    novoNo->isFolha = false;
    novoNo->pai = nullptr;
    novoNo->proximo = nullptr;
    return novoNo;
}

void BplusTree::insere(int chave, int endereco) {
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

void BplusTree::insereNaFolha(NoBplus* no, int chave, int endereco) {
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

void BplusTree::inserePai(NoBplus* noEsq, int chave, NoBplus* noDir) {
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
                int chaveMeio = pai->chaves[meio-1];
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

NoBplus* BplusTree::buscaNo(int chave) {
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

int main() {
    BplusTree robert(3);
    for(int i = 0; i < 25; i++) {
        robert.insere(i, 100);
    }
    robert.imprimeArvore();
    return 0;
}


