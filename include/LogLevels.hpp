#ifndef LOG_LEVELS
#define LOG_LEVELS

#include <string>

class LogLevels{
    public:
        static void logErro(std::string msg);
        static void logWarn(std::string msg);
        static void logInfo(std::string msg);
        static void logDebug(std::string msg);
};

#endif