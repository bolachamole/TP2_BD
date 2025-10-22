#include <iostream>
#include <chrono>
#include "../include/ParserCSV.hpp"

int main(int argc, char** argv){
	std::chrono::time_point<std::chrono::steady_clock> inicio;
	std::chrono::time_point<std::chrono::steady_clock> fim;
	ParserCSV parser(argv[1]);

	std::cout << "Caminho do arquivo de entrada: " << argv[1] << '\n';
	
	inicio = std::chrono::steady_clock::now();
	fim = std::chrono::steady_clock::now();
	std::cout << "Tempo para armazenar os dados da entrada em registros: " << std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count() << "ms \n";
	parser.fechaArquivo();

	inicio = std::chrono::steady_clock::now();
	//colocar registros em blocos
	fim = std::chrono::steady_clock::now();
	std::cout << "Tempo para armazenar os registros em blocos: " << std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count() << "ms \n";
	
	inicio = std::chrono::steady_clock::now();
	//colocar blocos no arquivo de dados
	fim = std::chrono::steady_clock::now();
	std::cout << "Tempo para armazenar blocos no arquivo de dados: " << std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count() << "ms \n";
	
	inicio = std::chrono::steady_clock::now();
	//indexacao primaria
	fim = std::chrono::steady_clock::now();
	std::cout << "Tempo para realizar a indexação primária: " << std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count() << "ms \n";
	
	inicio = std::chrono::steady_clock::now();
	//indexacao secundaria
	fim = std::chrono::steady_clock::now();
	std::cout << "Tempo para realizar a indexação secundária: " << std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count() << "ms \n";
	
	return 0;
}
