#ifndef BPLUS_TREE
#define BPLUS_TREE

#include <vector>
#include <fstream>
#include <math.h>

struct NoBplus {
    bool isFolha; //dita se um nó é folha ou não
    std::vector<unsigned int> chaves;
    std::vector<NoBplus*> filhos; //somente para nós internos
    std::vector<unsigned long long> enderecos;  //somente para nós folhas
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
        void insere(unsigned int chave, unsigned long long endereco);
        void insereNaFolha(NoBplus* no, unsigned int chave, unsigned long long endereco);
        void inserePai(NoBplus* noEsq, unsigned int chave, NoBplus* noDir);
        NoBplus* buscaNo(unsigned int chave);
        void imprimeArvore();
};

#endif
