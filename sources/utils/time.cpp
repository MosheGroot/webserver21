#include "../../headers/utils/time.hpp"
#include <ctime>

namespace WS { namespace Utils
{

  std::string Time::getTimestamp()
  {
    time_t      rawtime = std::time(NULL);
    struct tm*  tm_info = std::localtime(&rawtime);

    char    buffer[20] = { '\0' };
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);

    return std::string(buffer);
  }

}} //!namespace WS::Utils
