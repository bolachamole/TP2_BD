#ifndef BPLUS_TREE
#define BPLUS_TREE

#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <queue>

struct NoBplus {
    bool isFolha; //dita se um nó é folha ou não
    std::vector<int> chaves;
    std::vector<NoBplus*> filhos; //somente para nós internos
    std::vector<int> enderecos;  //somente para nós folhas
    NoBplus* proximo;            //somente para nós folhas
    NoBplus* pai;
};

class BplusTree {
    std::fstream arquivo; //arquivo que está salvo os dados da árvore B+. Usado pra achar os nós dentro do disco
    int ordem;
    NoBplus* raiz;

    public: 
        BplusTree(int ordem);
        NoBplus* criaNovoNo();
        void insere(int chave, int endereco);
        void insereNaFolha(NoBplus* no, int chave, int endereco);
        void inserePai(NoBplus* noEsq, int chave, NoBplus* noDir);
        NoBplus* buscaNo(int chave);
        void imprimeArvore();
};

#endif