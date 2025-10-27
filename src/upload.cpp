#include <iostream>
#include <chrono>
#include <filesystem>
#include "../include/GerenciaBlocos.hpp"
#include "../include/Hashing.hpp"
#include "../include/ParserCSV.hpp"
#include "../include/BPlusTree.hpp"
#include "../include/BPlusTreeSecundaria.hpp"
#include "../include/LogLevels.hpp"

int main(int argc, char** argv){
	if (argc != 2) {
        LogLevels::logErro("Uso: " + std::string(argv[0]) + " <ID>");
        return 1;
    }

	LogLevels::logInfo("Caminho do arquivo de entrada: " + std::string(argv[1]));

	std::chrono::time_point<std::chrono::steady_clock> inicio;
	std::chrono::time_point<std::chrono::steady_clock> fim;
	
	std::filesystem::create_directories("data/db");
	LogLevels::logDebug("Diretório data/db criado.");

	GerenciaBlocos gerente_blocos("data/db/arqdados.dat");
	GerenciaBlocos gerente_blocos_primario("data/db/index1.db");
	GerenciaBlocos gerente_blocos_secundario("data/db/index2.db");
	Hashing tabela_hash(&gerente_blocos);
	ParserCSV parser(argv[1]);
	BplusTree arvore1(3, &gerente_blocos_primario, &gerente_blocos); //arvore de ordem 3 para indexacao primaria
	BplusTreeSecundaria arvore2(3, &gerente_blocos_secundario, &gerente_blocos); //arvore de ordem 3 para indexacao secundaria

	LogLevels::logDebug("Classes instanciadas.");
	LogLevels::logInfo("Processando linhas...");

	inicio = std::chrono::steady_clock::now();
	parser.lerArquivo(&tabela_hash);
	parser.fechaArquivo();
	arvore1.criaIdexPrimario();
	arvore2.criaIdexSecundario();
	fim = std::chrono::steady_clock::now();

	LogLevels::logInfo("Linhas processadas: " + parser.getLinhasProc());
	LogLevels::logInfo("Estatísticas da carregamento em hash:");
	LogLevels::logInfo("	- Blocos lidos: " + gerente_blocos.getBlocos_lidos());
	LogLevels::logInfo("	- Blocos escritos: " + gerente_blocos.getBlocos_escritos());

	LogLevels::logInfo("Estatísticas da indexação primária:");
	LogLevels::logInfo("	- Blocos lidos: " + gerente_blocos_primario.getBlocos_lidos());
	LogLevels::logInfo("	- Blocos escritos: " + gerente_blocos_primario.getBlocos_escritos());
	
	LogLevels::logInfo("Estatísticas da indexação secundária:");
	LogLevels::logInfo("	- Blocos lidos: " + gerente_blocos_secundario.getBlocos_lidos());
	LogLevels::logInfo("	- Blocos escritos: " + gerente_blocos_secundario.getBlocos_escritos());

	LogLevels::logInfo("	Tempo total para processar os dados do arquivo de entrada: " + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count()) + "ms");

	return 0;
}
