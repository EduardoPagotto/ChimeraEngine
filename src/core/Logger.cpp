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

// const std::string Logger::sFileName = "Log.txt";
Logger* Logger::pThis = nullptr;
// std::ofstream Logger::logfile;

Logger::Logger() {}

Logger* Logger::get() {
    if (pThis == nullptr) {
        pThis = new Logger();
        // logfile.open(sFileName.c_str(), std::ios::out | std::ios::app);
    }
    return pThis;
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

    std::cout << currentDateTime() << "\t";
    std::cout << sMessage << "\n";

    va_end(args);

    delete[] sMessage;
}

void Logger::debug(const std::string& sMessage) {
    std::cout << currentDateTime() << "\t";
    std::cout << sMessage << "\n";
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

    std::cout << currentDateTime() << "\t";
    std::cout << sMessage << "\n";

    va_end(args);

    delete[] sMessage;
}

void Logger::error(const std::string& sMessage) {
    std::cout << currentDateTime() << "\t";
    std::cout << sMessage << "\n";
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

    std::cout << currentDateTime() << "\t";
    std::cout << sMessage << "\n";

    va_end(args);

    delete[] sMessage;
}

void Logger::warn(const std::string& sMessage) {
    std::cout << currentDateTime() << "\t";
    std::cout << sMessage << "\n";
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

    std::cout << currentDateTime() << "\t";
    std::cout << sMessage << "\n";

    va_end(args);

    delete[] sMessage;
}

void Logger::info(const std::string& sMessage) {
    std::cout << currentDateTime() << "\t";
    std::cout << sMessage << "\n";
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
    strftime(buffer, 80, "%Y%m%d-%X", timeinfo);
    return buffer;
}

} // namespace Chimera