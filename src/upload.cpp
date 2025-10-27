#include <iostream>
#include <chrono>
#include <filesystem>
#include "../include/GerenciaBlocos.hpp"
#include "../include/Hashing.hpp"
#include "../include/ParserCSV.hpp"
#include "../include/BPlusTree.hpp"
#include "../include/BPlusTreeSecundaria.hpp"

int main(int argc, char** argv){
	std::cout << "Caminho do arquivo de entrada: " << argv[1] << '\n';

	std::chrono::time_point<std::chrono::steady_clock> inicio;
	std::chrono::time_point<std::chrono::steady_clock> fim;
	std::filesystem::create_directories("data/db");
	GerenciaBlocos gerente_blocos("data/db/arqdados.dat");
	GerenciaBlocos gerente_blocos_primario("data/db/index1.db");
	GerenciaBlocos gerente_blocos_secundario("data/db/index2.db");
	Hashing tabela_hash(&gerente_blocos);
	ParserCSV parser(argv[1]);
	BplusTree arvore1(3, &gerente_blocos_primario, &gerente_blocos); //arvore de ordem 3 para indexacao primaria
	BplusTreeSecundaria arvore2(3, &gerente_blocos_secundario, &gerente_blocos); //arvore de ordem 3 para indexacao secundaria

	std::cout << "Processando linhas...\n";

	inicio = std::chrono::steady_clock::now();
	parser.lerArquivo(&tabela_hash);
	parser.fechaArquivo();
	arvore1.criaIdexPrimario();
	arvore2.criaIdexSecundario();
	fim = std::chrono::steady_clock::now();

	std::cout << "Linhas processadas: " << parser.getLinhasProc() << '\n';
	std::cout << "Estatísticas da carregamento em hash:\n";
	std::cout << "	- Blocos lidos: " << gerente_blocos.getBlocos_lidos() << '\n';
	std::cout << "	- Blocos escritos: " << gerente_blocos.getBlocos_escritos() << '\n';

	std::cout << "Estatísticas da indexação primária:\n";
	std::cout << "	- Blocos lidos: " << gerente_blocos_primario.getBlocos_lidos() << '\n';
	std::cout << "	- Blocos escritos: " << gerente_blocos_primario.getBlocos_escritos() << '\n';
	
	std::cout << "Estatísticas da indexação secundária:\n";
	std::cout << "	- Blocos lidos: " << gerente_blocos_secundario.getBlocos_lidos() << '\n';
	std::cout << "	- Blocos escritos: " << gerente_blocos_secundario.getBlocos_escritos() << '\n';

	std::cout << "	Tempo total para processar os dados do arquivo de entrada: " << std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count() << "ms \n";

	return 0;
}
