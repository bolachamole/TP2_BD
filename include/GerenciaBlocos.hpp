#ifndef GERENCIA_BLOCO
#define GERENCIA_BLOCO

#include <string>

class GerenciaBlocos{
	private:
		const char* arq_entrada;
		int nitems;
		unsigned int size_blocos;

	public:
		GerenciaBlocos(std::string arq_nome);

		void setNitems(int n);
		void setSize_blocos();
		int getNitems();
		unsigned int getSize_blocos();
};

#endif
