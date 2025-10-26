#include <iostream>
#include <chrono>
#include <filesystem>
#include "../include/GerenciaBlocos.hpp"
#include "../include/Hashing.hpp"
#include "../include/ParserCSV.hpp"

int main(int argc, char** argv){
	std::chrono::time_point<std::chrono::steady_clock> inicio;
	std::chrono::time_point<std::chrono::steady_clock> fim;
	std::filesystem::create_directories("data/db");
	GerenciaBlocos gerente_blocos("data/db/arqdados.dat");
	Hashing tabela_hash(&gerente_blocos);
	ParserCSV parser(argv[1]);

	std::cout << "Caminho do arquivo de entrada: " << argv[1] << '\n';
	std::cout << "Processando linhas...\n";

	inicio = std::chrono::steady_clock::now();
	parser.lerArquivo(&tabela_hash);
	parser.fechaArquivo();
	fim = std::chrono::steady_clock::now();

	std::cout << "Linhas processadas: " << parser.getLinhasProc() << '\n';
	std::cout << "Blocos lidos: " << gerente_blocos.getBlocos_lidos() << '\n';
	std::cout << "Blocos escritos: " << gerente_blocos.getBlocos_escritos() << '\n';
	std::cout << "Tempo total para processar os dados do arquivo de entrada: " << std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count() << "ms \n";

	std::cout << "Realizando a indexação primária em árvore B+...\n";
	inicio = std::chrono::steady_clock::now();
	fim = std::chrono::steady_clock::now();
	std::cout << "Blocos lidos: " << gerente_blocos.getBlocos_lidos() << '\n';
	std::cout << "Blocos escritos: " << gerente_blocos.getBlocos_escritos() << '\n';
	std::cout << "Tempo para realizar a indexação primária: " << std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count() << "ms \n";
	
	std::cout << "Realizando a indexação secundária em árvore B+...\n";
	inicio = std::chrono::steady_clock::now();
	fim = std::chrono::steady_clock::now();
	std::cout << "Blocos lidos: " << gerente_blocos.getBlocos_lidos() << '\n';
	std::cout << "Blocos escritos: " << gerente_blocos.getBlocos_escritos() << '\n';
	std::cout << "Tempo para realizar a indexação secundária: " << std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count() << "ms \n";

	return 0;
}
