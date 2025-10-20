#include <iostream>
#include <string>
#include <chrono> //mede o tempo de execucao
#include <filesystem> //manipulacao de arquivos e diretorios
#include <stdexcept> //trata erros de conversao
#include "../include/Hashing.hpp" //chama arquivo hashing.hpp
#include "../include/GerenciaBlocos.hpp" //chama arquivo GerenciaBlocos.hpp


void imprime_registro(const registro& reg){ //imprime os dados do registro
    std::cout << "----- Registro Encontrado -----" << std::endl;
    std::cout << "ID: " << reg.id << std::endl;
    std::cout << "Titulo: " << reg.titulo << std::endl;
    std::cout << "Ano: " << reg.ano << std::endl;
    std::cout << "Autores: " << reg.autores << std::endl;
    std::cout << "Citacoes: " << reg.citacoes << std::endl;
    std::cout << "Atualizacao: " << reg.atualizacao << std::endl;

    if(reg.snippet != nullptr){ //verifica se o snippet nao e nulo
        std::cout << "Snippet: " << reg.snippet << std::endl; //imprime o snippet se nao for nulo
    } else {
        std::cout << "Snippet: (NULL)" << std::endl; //imprime (NULL) se for nulo
    }
    std::cout << "------------------------------" << std::endl; //imprime linha de separacao
}

int main(int argc, char** argv){
    if(argc != 2){
        std::cerr << "Erro: Modo de uso: ./bin/findrec <ID>" << std::endl; //verifica se o numero de argumentos esta correto
        return 1;
    }

    std::string data_dir= std::getenv("DATA_DIR"); //pega o diretorio dos dados do ambiente
    std::string arquivo_dados_hash = data_dir + "------/arquivo_hashing----------"; //caminho do resultado do hashing

    unsigned int id_buscado;
    try{
        id_buscado = std::stoull(argv[1]); //converte o argumento para inteiro
        }
        catch(const std::invalid_argument& e){ //trata erro de conversao
        std::cerr << "Erro: ID invalido. Deve ser um numero inteiro." << std::endl;
        return 1;
    } 
    
    std::cout << "Buscando registro com ID: " << id_buscado << std::endl; //imprime o ID buscado
    std::cout << "Usando arquivo de hashing: " << arquivo_dados_hash << std::endl; //imprime o caminho do arquivo de hashing

    auto inicio = std::chrono::steady_clock::now(); //marca o tempo de inicio da busca

    //Hashing h;
    //hipoteticamente, aqui deveria haver uma funcao para carregar o arquivo de hashing
    //registro reg_encontrado= h.buscaHash(id_buscado); //busca o registro no arquivo de hashing, chamando a funcao buscaHash da classe Hashing
    //imprime_registro(reg_encontrado); //imprime o registro encontrado

    //registro Hashing::buscaHash(unsigned int id_buscado) {
}
