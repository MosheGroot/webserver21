#include <iostream>
#include "../../headers/utils/logger.hpp"

namespace WS { namespace Utils
{
    /// Singleton part
    Logger  Logger::instance_;

    /// Logic Part
    int  Logger::error(const std::string& message, int status_code)
    {
        std::cerr << "\x1B[1;31m[ERROR]\x1B[m " << message << std::endl;
        return status_code;
    }

    void Logger::info(const std::string& message)
    {
        std::cout << "\x1B[32m[INFO]\x1B[m " << message << std::endl;
    }

    void Logger::debug(const std::string& message)
    {
        std::cout << "\x1B[33m[DEBUG]\x1B[m " << message << std::endl;
    }
    
}} //!namespace WS::Utils
