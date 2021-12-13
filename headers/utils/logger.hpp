#pragma once

#include <string>

namespace WS::Utils
{
  /* @brief Logger as singleton class
  */
  class Logger
  {
  /// singleton part
    static Logger *instance_;

    Logger() {}

  public:
    Logger(Logger& other) = delete;

    void operator=(const Logger& other) = delete;

    static Logger&     getInstance(void);

  /// logic part
  public:
    /*  @brief Print error message to stderr.
        @param message      String to print
        @param status_code  Status to return from function (-1 by default)
    */
    static int  error(const std::string& message, int status_code = -1);
    
    /*  @brief Print info message to stdout.
        @param message      String to print
    */
    static void info(const std::string& message);

  }; //!class Logger
} //!namespace WS::Utils