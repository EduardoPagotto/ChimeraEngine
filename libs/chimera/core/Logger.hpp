#ifndef __CHIMERA_LOGGER__HPP
#define __CHIMERA_LOGGER__HPP
#include <cstdarg>
#include <fstream>
#include <iostream>
#include <string>
#include <time.h>

// ref:https://cppcodetips.wordpress.com/2014/01/02/a-simple-logger-class-in-c/

//#define SMART_LOG(message) smart_log(__FUNCTION__, message)
//#define (message) smart_log(__FUNCTION__, message)
//#define LOGGERZ Chimera::Logger::GetLogger()

// using namespace std;

namespace Chimera {

/**
 *   Singleton Logger Class.
 */
class Logger {
  public:
    /**
     *   Logs a message
     *   @param sMessage message to be logged.
     */
    void info(const std::string& sMessage);
    /**
     *   Variable Length Logger function
     *   @param format string for the message to be logged.
     */
    void info(const char* format, ...);

    void warn(const char* format, ...);

    void warn(const std::string& sMessage);

    void debug(const char* format, ...);

    void debug(const std::string& sMessage);

    void error(const char* format, ...);

    void error(const std::string& sMessage);

    /**
     *   << overloaded function to Logs a message
     *   @param sMessage message to be logged.
     */
    Logger& operator<<(const std::string& sMessage);
    /**
     *   Funtion to create the instance of logger class.
     *   @return singleton object of Clogger class..
     */
    static Logger* get();

  private:
    /**
     *    Default constructor for the Logger class.
     */
    Logger();
    /**
     *   copy constructor for the Logger class.
     */
    Logger(const Logger&){}; // copy constructor is private
    /**
     *   assignment operator for the Logger class.
     */
    Logger& operator=(const Logger&) { return *this; }; // assignment operator is private
    /**
     *   Log file name.
     **/

    const std::string currentDateTime();

    // static const std::string sFileName;
    /**
     *   Singleton logger class object pointer.
     **/
    static Logger* pThis;
    /**
     *   Log file stream object.
     **/
    // static std::ofstream logfile;
};
} // namespace Chimera
#endif