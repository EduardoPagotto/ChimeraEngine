#ifndef __CHIMERA_LOGGER__HPP
#define __CHIMERA_LOGGER__HPP
#include <cstdarg>
#include <fstream>
#include <iostream>
#include <string>
#include <time.h>

// ref: https://cppcodetips.wordpress.com/2014/01/02/a-simple-logger-class-in-c/
// ref: https://en.wikipedia.org/wiki/ANSI_escape_code

namespace Chimera {

class Logger {
  public:
    void info(const std::string& sMessage);
    void info(const char* format, ...);
    void warn(const char* format, ...);
    void warn(const std::string& sMessage);
    void debug(const char* format, ...);
    void debug(const std::string& sMessage);
    void error(const char* format, ...);
    void error(const std::string& sMessage);

    enum class Level { ERROR = 0, WARNNING = 1, INFO = 2, DEBUG = 3 };

    Logger& operator<<(const std::string& sMessage);

    static Logger* get();

    // 0 -> error
    // 1 -> warning , error
    // 2 -> info, warning , error
    // 3 -> debug, info, warning, error (default)
    inline void setLevel(Level _level) { level = (int)_level; }

  private:
    Logger();
    Logger(const Logger&){};                            // copy constructor is private
    Logger& operator=(const Logger&) { return *this; }; // assignment operator is private

    int level;
    static Logger* pLogger;
    const std::string currentDateTime();
};
} // namespace Chimera
#endif