#pragma once

#include <string>
#include <iostream>

namespace WS { namespace Utils
{
  /* @brief Logger as singleton class
  */
  class Logger
  {
  /// singleton part
    Logger() {}

    Logger(Logger& other) { (void)other; }                // deleted (private)

    void operator=(const Logger& other) { (void)other; }  // deleted (private)

  public:
    static Logger instance_;

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

    /*  @brief Print debug message to stdout.
        @param message      String to print
    */
    static void debug(const std::string& message);

  private:
    /*  @brief Log message with prompt to specified stream.
        @param message      String to print
        @param os           Output stream
        @param prompt       Prompt string to print before main message
                              (LOG by default)
        @param colors       Terminal colors for prompt
                              (empty by default)
    */
    static void log(const std::string& message, std::ostream& os,
                    const char *prompt = "LOG", const char *colors = "");

    static const bool timestamp_enabled_  = true; //< use timestamps
    static const int  prompt_max_width_   = 5;    //< max prompt width

  }; //!class Logger
}} //!namespace Utils
