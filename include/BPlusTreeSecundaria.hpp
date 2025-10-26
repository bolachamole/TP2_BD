#ifndef BPLUS_TREE_SEC
#define BPLUS_TREE_SEC

#include <vector>
#include <math.h>
#include "../include/GerenciaBlocos.hpp"

struct chave_titulo{
    unsigned int tam_titulo;
    char titulo[300];
};

struct NoBplus2 {
    bool isFolha; //dita se um nó é folha ou não
    std::vector<chave_titulo> chaves;
    std::vector<NoBplus2*> filhos; //somente para nós internos
    std::vector<unsigned long long> enderecos;  //somente para nós folhas
    NoBplus2* proximo;            //somente para nós folhas
    NoBplus2* pai;
};

class BplusTreeSecundaria {
    int ordem;
    NoBplus2* raiz;
    GerenciaBlocos* blocos_gerente;
    GerenciaBlocos* dados_gerente;

    public:
		BplusTreeSecundaria(int ordem, GerenciaBlocos* blocos_gerente, GerenciaBlocos* dados_gerente);
		NoBplus2* criaNovoNo();
		void insere(chave_titulo chave, unsigned long long endereco);
		void insereNaFolha(NoBplus2* no, chave_titulo chave, unsigned long long endereco);
		void inserePai(NoBplus2* noEsq, chave_titulo chave, NoBplus2* noDir);
		NoBplus2* buscaNo(chave_titulo chave);
        void criaIdexSecundario();
};

#endif
