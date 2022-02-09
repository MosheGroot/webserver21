#include <iostream>
#include <iomanip>

#include "../../headers/utils/logger.hpp"
#include "../../headers/utils/time.hpp"

namespace WS { namespace Utils
{
    /// Singleton part
    Logger  Logger::instance_;


    /// Logic Part
    int  Logger::error(const std::string& message, int status_code)
    {
      Logger::log(message, std::cerr, "ERROR", "\x1B[1;31m");
      // std::cerr << "\x1B[1;31m[ERROR]\x1B[m " << message << std::endl;
      return status_code;
    }

    void Logger::info(const std::string& message)
    {
      Logger::log(message, std::cout, "INFO", "\x1B[32m");
      // std::cout << "\x1B[32m[INFO]\x1B[m " << message << std::endl;
    }

    void Logger::debug(const std::string& message)
    {
      Logger::log(message, std::cout, "DEBUG", "\x1B[33m");
      // std::cout << "\x1B[33m[DEBUG]\x1B[m " << message << std::endl;
    }

    void Logger::log(const std::string& message, std::ostream& os,
                      const char *prompt, const char *colors)
    {
      // timestamp
      if (Logger::timestamp_enabled_)
        os << '[' << Utils::Time::getTimestamp() << "] ";

      // prompt
      os << colors << '[' << std::setiosflags(std::ios_base::left)
          << std::setw(Logger::prompt_max_width_) << prompt << "]\x1B[m ";

      // message
      os << message << std::endl;
    }

}} //!namespace WS::Utils
