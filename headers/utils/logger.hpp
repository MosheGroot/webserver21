#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sys/types.h>

namespace WS { namespace Utils
{
  /* @brief Thread-Unsafe logger as singleton class
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
  // Log functions part
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
                    const char *prompt = "LOG", const char *colors = NULL);

    static const bool timestamp_enabled_  = true; //< use timestamps
    static const int  prompt_max_width_   = 5;    //< max prompt width

  // Filesystem part
  public:
    static void init();

  private:
    static const bool         duplicate_to_file_ = true;  //< log to file
    static const std::string  logs_directory_;            //< directory for logs
    static std::ofstream      output_fstream_;

  }; //!class Logger
}} //!namespace Utils
