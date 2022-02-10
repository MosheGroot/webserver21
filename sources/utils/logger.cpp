#include <iostream>
#include <iomanip>

#include "../../headers/utils/logger.hpp"
#include "../../headers/utils/time.hpp"
#include "../../headers/utils/file.hpp"

namespace WS { namespace Utils
{
    /// Singleton part
    Logger  Logger::instance_;


    /// Logic Part
    // General
    void  Logger::init(LogLevel minimum_log_level, bool duplicate_to_file)
    {
      if (duplicate_to_file)
      {
        Utils::File::createPath(Logger::logs_directory_.c_str());
        Logger::output_fstream_.open(
          (
            Logger::logs_directory_
            + "/log_"
            + Utils::Time::getTimestamp("%Y-%m-%d")
            + ".txt"
          ).c_str(),
          std::ios_base::app
        );
      }

      Logger::minimum_log_level_ = minimum_log_level;
    }

    // Log functions part
    int  Logger::error(const std::string& message, int status_code)
    {
      if (minimum_log_level_ < LOGLEV_ERROR)
        return status_code;

      Logger::log(message, std::cerr, "ERROR", "\x1B[1;31m");

      if (Logger::output_fstream_.is_open())
        Logger::log(message, Logger::output_fstream_, "ERROR");

      return status_code;
    }

    void Logger::info(const std::string& message)
    {
      if (minimum_log_level_ < LOGLEV_INFO)
        return;

      Logger::log(message, std::cout, "INFO", "\x1B[32m");

      if (Logger::output_fstream_.is_open())
        Logger::log(message, Logger::output_fstream_, "INFO");
    }

    void Logger::debug(const std::string& message)
    {
      if (minimum_log_level_ < LOGLEV_DEBUG)
        return;

      Logger::log(message, std::cout, "DEBUG", "\x1B[33m");

      if (Logger::output_fstream_.is_open())
        Logger::log(message, Logger::output_fstream_, "DEBUG");
    }
    void Logger::log(const std::string& message, std::ostream& os,
                      const char *prompt, const char *colors)
    {
      // timestamp
      if (Logger::timestamp_enabled_)
        os << '[' << Utils::Time::getTimestamp() << "] ";


      // prompt
      if (colors)
        os << colors;

      os << '[' << std::setiosflags(std::ios_base::left)
          << std::setw(Logger::prompt_max_width_) << prompt << "] ";

      if (colors)
        os << "\x1B[m";


      // message
      os << message << std::endl;
    }

  Logger::LogLevel  Logger::minimum_log_level_ = LOGLEV_ERROR;

  // Filesystem part
  const std::string Logger::logs_directory_ = "resources/logs";
  std::ofstream     Logger::output_fstream_;



}} //!namespace WS::Utils
