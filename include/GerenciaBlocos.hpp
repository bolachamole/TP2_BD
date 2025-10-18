#ifndef GERENCIA_BLOCO
#define GERENCIA_BLOCO

#include <string>

class GerenciaBlocos{
	private:
		const char* arq_entrada;
		unsigned int size_blocos;

	public:
		GerenciaBlocos(std::string arq_nome);

		void setSize_blocos(int fd);
		unsigned int getSize_blocos();
};

#endif
