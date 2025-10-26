#ifndef BPLUS_TREE
#define BPLUS_TREE

#include <vector>
#include <math.h>
#include "../include/GerenciaBlocos.hpp"

struct NoBplus {
    bool isFolha; //dita se um nó é folha ou não
    std::vector<unsigned int> chaves;
    std::vector<NoBplus*> filhos; //somente para nós internos
    std::vector<unsigned long long> enderecos;  //somente para nós folhas
    NoBplus* proximo;            //somente para nós folhas
    NoBplus* pai;
};

class BplusTree {
    int ordem;
    NoBplus* raiz;
    GerenciaBlocos* blocos_gerente;
    GerenciaBlocos* dados_gerente;

    public:
		BplusTree(int ordem, GerenciaBlocos* blocos_gerente, GerenciaBlocos* dados_gerente);
		NoBplus* criaNovoNo();
		void insere(unsigned int chave, unsigned long long endereco);
		void insereNaFolha(NoBplus* no, unsigned int chave, unsigned long long endereco);
		void inserePai(NoBplus* noEsq, unsigned int chave, NoBplus* noDir);
		NoBplus* buscaNo(unsigned int chave);
		void criaIdexPrimario();
};

#endif
