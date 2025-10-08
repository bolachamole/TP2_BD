#include <iostream>
#include <fstream>

int main(int argc, char** argv){
    std::ifstream file(argv[1]);

    std::cout << "Caminho do arquivo de entrada: " << argv[1] << '\n';

    return 0;
}
