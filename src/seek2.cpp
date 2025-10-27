#include <iostream>
#include <string>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include "../include/GerenciaBlocos.hpp"
#include "../include/BPlusTreeSecundaria.hpp"

void imprime_registro(const registro& reg){ 
    std::cout << "----- Registro Encontrado -----\n";
    std::cout << "ID: " << reg.id << "\n";
    std::cout << "Titulo: " << reg.titulo << "\n";
    std::cout << "Ano: " << reg.ano << "\n";
    std::cout << "Autores: " << reg.autores << "\n";
    std::cout << "Citacoes: " << reg.citacoes << "\n";
    std::cout << "Atualizacao: " << reg.atualizacao << "\n";
    if(reg.snippet[0] != '\0')
        std::cout << "Snippet: " << reg.snippet << "\n";
    else
        std::cout << "Snippet: (NULL)\n";
    std::cout << "------------------------------\n";
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " \"<Titulo>\"\n";
        return 1;
    }

    std::string titulo_busca= argv[1];

    const char* data_dir_env= std::getenv("DATA_DIR");
    std::string data_file_path= data_dir_env ? std::string(data_dir_env) + "/arqdados.dat" : "data/db/arqdados.dat";
    std::string index_file_path= data_dir_env ? std::string(data_dir_env) + "/index2.db" : "data/db/index2.db";

    std::cout << "Buscando titulo \"" << titulo_busca << "\" via indice secundario...\n";

    GerenciaBlocos gerente_idx(index_file_path);
    GerenciaBlocos gerente_dados(data_file_path);
    BplusTreeSecundaria arvore(3, &gerente_idx, &gerente_dados);

    auto inicio= std::chrono::steady_clock::now();

    chave_titulo chave{};//prepara a chave de busca
    chave.tam_titulo = titulo_busca.size();
    strncpy(chave.titulo, titulo_busca.c_str(), sizeof(chave.titulo));

    NoBplus2* folha= arvore.buscaNo(chave); //busca o no folha onde o título deve estar

    bool achou= false;
    registro reg_encontrado{};

    for (size_t i= 0; i < folha->chaves.size(); i++) {
        if (strcmp(folha->chaves[i].titulo, titulo_busca.c_str()) == 0) {
            unsigned long long endereco= folha->enderecos[i];
            bucket meuBucket{};
            gerente_dados.lerBucket(endereco, &meuBucket);

            for (unsigned int j= 0; j < meuBucket.nRegs; j++) {
                if (strcmp(meuBucket.registros[j].titulo, titulo_busca.c_str()) == 0) {
                    reg_encontrado = meuBucket.registros[j];
                    achou = true;
                    break;
                }
            }
            break;
        }
    }

    auto fim= std::chrono::steady_clock::now();
    auto duracao= std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count();

    if (achou){
        imprime_registro(reg_encontrado);

    }
    else{
        std::cout << "Registro com título \"" << titulo_busca << "\" não encontrado.\n";
    }

    std::cout << "Tempo de execução: " << duracao << " ms\n";
    std::cout << "Blocos lidos no indice: " << gerente_idx.getBlocos_lidos() << "\n";
    std::cout << "Total de blocos no indice: " << gerente_idx.totalDeBlocosArquivo() << "\n";

    gerente_idx.fechaArquivoFstream();
    gerente_dados.fechaArquivoFstream();
    return 0;
}
