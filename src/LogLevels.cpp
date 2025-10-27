#include <cstdlib>
#include <cstring>
#include <iostream>
#include "../include/LogLevels.hpp"

void LogLevels::logErro(std::string msg){
    char* nivel = std::getenv("LOG_LEVEL");

    if(strcmp(nivel, "error") == 0) std::cerr << msg << std::endl;
}

void LogLevels::logWarn(std::string msg){
    char* nivel = std::getenv("LOG_LEVEL");

    if(strcmp(nivel, "warn") == 0) std::cout << msg << std::endl;
}

void LogLevels::logInfo(std::string msg){
    char* nivel = std::getenv("LOG_LEVEL");

    if(strcmp(nivel, "info") == 0) std::cout << msg << std::endl;
}

void LogLevels::logDebug(std::string msg){
    char* nivel = std::getenv("LOG_LEVEL");

    if(strcmp(nivel, "debug") == 0) std::cout << msg << std::endl;
}