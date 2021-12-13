#include <iostream>
#include "../../headers/utils/logger.hpp"

namespace WS::Utils
{
    /// Singleton part
    Logger&   Logger::getInstance(void)
    {
      if (instance_ == nullptr)
        instance_ = new Logger();
      return *instance_;
    }
    
    /// Logic Part
    int  Logger::error(const std::string& message, int status_code)
    {
        std::cerr << message << std::endl;
        return status_code;
    }

    void Logger::info(const std::string& message)
    {
        std::cout << message << std::endl;
    }

} //!namespace WS::Utils