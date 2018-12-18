#include "chimera/core/Logger.hpp"

namespace Chimera {

int _vscprintf(const char* format, va_list pargs) {
    int retval;
    va_list argcopy;
    va_copy(argcopy, pargs);
    retval = vsnprintf(nullptr, 0, format, argcopy);
    va_end(argcopy);
    return retval;
}

Logger* Logger::pLogger = nullptr;
Logger::Logger() : level(3) {}

Logger* Logger::get() {
    if (pLogger == nullptr)
        pLogger = new Logger();

    return pLogger;
}

void Logger::debug(const char* format, ...) {
    char* sMessage = nullptr;
    int nLength = 0;
    va_list args;
    va_start(args, format);
    nLength = _vscprintf(format, args) + 1;
    sMessage = new char[nLength];
    // vsprintf(sMessage, nLength, format, args);
    vsprintf(sMessage, format, args);

    debug(std::string(sMessage));

    va_end(args);

    delete[] sMessage;
}

void Logger::info(const char* format, ...) {
    char* sMessage = nullptr;
    int nLength = 0;
    va_list args;
    va_start(args, format);
    nLength = _vscprintf(format, args) + 1;
    sMessage = new char[nLength];
    // vsprintf(sMessage, nLength, format, args);
    vsprintf(sMessage, format, args);

    info(std::string(sMessage));

    va_end(args);

    delete[] sMessage;
}

void Logger::warn(const char* format, ...) {
    char* sMessage = nullptr;
    int nLength = 0;
    va_list args;
    va_start(args, format);
    nLength = _vscprintf(format, args) + 1;
    sMessage = new char[nLength];
    // vsprintf(sMessage, nLength, format, args);
    vsprintf(sMessage, format, args);

    warn(sMessage);

    va_end(args);

    delete[] sMessage;
}

void Logger::error(const char* format, ...) {
    char* sMessage = nullptr;
    int nLength = 0;
    va_list args;
    va_start(args, format);
    nLength = _vscprintf(format, args) + 1;
    sMessage = new char[nLength];
    // vsprintf(sMessage, nLength, format, args);
    vsprintf(sMessage, format, args);

    error(std::string(sMessage));

    va_end(args);

    delete[] sMessage;
}

void Logger::debug(const std::string& sMessage) {

    if (level > 2)
        std::cout << "\033[96m" << currentDateTime() << " " << sMessage << "\033[39m"
                  << "\n";
}

void Logger::info(const std::string& sMessage) {

    if (level > 1)
        std::cout << "\033[97m" << currentDateTime() << " " << sMessage << "\033[39m"
                  << "\n";
}

void Logger::warn(const std::string& sMessage) {

    if (level > 0)
        std::cout << "\033[93m" << currentDateTime() << " " << sMessage << "\033[39m"
                  << "\n";
}

void Logger::error(const std::string& sMessage) {
    std::cout << "\033[91m" << currentDateTime() << " " << sMessage << "\033[39m"
              << "\n";
}

Logger& Logger::operator<<(const std::string& sMessage) {
    std::cout << "\n" << currentDateTime() << "\t";
    std::cout << sMessage << "\n";
    return *this;
}

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string Logger::currentDateTime() {
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    // strftime(buffer, 80, "%Y%m%d-%X", timeinfo);
    strftime(buffer, 80, "%X", timeinfo);
    return buffer;
}
} // namespace Chimera