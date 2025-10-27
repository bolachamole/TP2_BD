#include <iostream>
#include <string>
#include <chrono> //mede o tempo de execucao
#include <stdexcept> //trata erros de conversao
#include <cstdlib> //pega variaveis de ambiente
#include <iomanip> //
#include "../include/GerenciaBlocos.hpp"
#include "../include/Hashing.hpp"
#include "../include/LogLevels.hpp"

void imprime_registro(const registro& reg){ //imprime os dados do registro
    std::cout << "----- Registro Encontrado -----" << std::endl;
    std::cout << "ID: " << reg.id << std::endl;
    std::cout << "Titulo: " << reg.titulo << std::endl;
    std::cout << "Ano: " << reg.ano << std::endl;
    std::cout << "Autores: " << reg.autores << std::endl;
    std::cout << "Citacoes: " << reg.citacoes << std::endl;
    std::cout << "Atualizacao: " << reg.atualizacao << std::endl;
    if(reg.snippet[0] != '\0'){ //verifica se o snippet nao e nulo
        std::cout << "Snippet: " << reg.snippet << std::endl; //imprime o snippet se nao for nulo
    } else {
        std::cout << "Snippet: (NULL)" << std::endl; //imprime (NULL) se for nulo
    }
    std::cout << "------------------------------" << std::endl;
}

int main(int argc, char** argv){
    if (argc != 2) {
        LogLevels::logErro("Uso: " + std::string(argv[0]) + " <ID>");
        return 1;
    }

    unsigned int id_busca;
    try {
        id_busca= std::stoul(argv[1]);
    } catch (const std::invalid_argument& e) {
        LogLevels::logErro("Erro: ID invalido. Deve ser um numero inteiro.");
        return 1;
    } catch (const std::out_of_range& e) {
        LogLevels::logErro("Erro: ID fora do intervalo de inteiros.");
        return 1;
    }

    const char* data_dir_env = std::getenv("DATA_DIR");//pega caminho do arquivo de dados
    std::string data_file_path;
    if (data_dir_env) {
        data_file_path= std::string(data_dir_env) + "/arqdados.dat";
    } else {
        data_file_path= "data/db/arqdados.dat"; //
    }

    LogLevels::logInfo("Caminho do arquivo de dados: " + data_file_path);

    auto inicio = std::chrono::steady_clock::now(); //inicio da contagem de tempo

    GerenciaBlocos gerente_blocos(data_file_path);
    Hashing tabela_hash(&gerente_blocos); //

    gerente_blocos.zeraBlocosLidos(); // Zera contagem de blocos lidos
    registro* encontrado = tabela_hash.buscaHash(id_busca);

    auto fim = std::chrono::steady_clock::now(); //fim da contagem de tempo
    long long duracao= std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count(); //calcula duracao em ms

    if (encontrado != nullptr){ //se encontrou o registro
        imprime_registro(*encontrado);
        delete encontrado; //libera a memoria alocada por buscaHash
    } else {
        LogLevels::logInfo("Registro com ID " + std::to_string(id_busca) + " nao encontrado.");
    }
    LogLevels::logInfo("Tempo de execucao: " + std::to_string(duracao) + " ms");
    LogLevels::logInfo("Blocos lidos: " + gerente_blocos.getBlocos_lidos());
    LogLevels::logInfo("Total de blocos no arquivo: " + gerente_blocos.totalDeBlocosArquivo());

    gerente_blocos.fechaArquivo();


    return 0;
}
