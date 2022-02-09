#include "../../headers/utils/time.hpp"
#include <ctime>

namespace WS { namespace Utils
{

  std::string Time::getTimestamp(const char *format)
  {
    time_t      rawtime = std::time(NULL);
    struct tm*  tm_info = std::localtime(&rawtime);

    char    buffer[20] = { '\0' };
    strftime(buffer, sizeof(buffer), format, tm_info);

    return std::string(buffer);
  }

}} //!namespace WS::Utils
