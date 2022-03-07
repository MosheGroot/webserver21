#include "../../headers/utils/time.hpp"
#include <ctime>

namespace WS { namespace Utils
{

  std::string Time::getTimestamp(const char *format, bool localtime)
  {
    time_t      rawtime = std::time(NULL);

    struct tm*  tm_info = NULL;
    if (localtime)
      tm_info = std::localtime(&rawtime);
    else
      tm_info = std::gmtime(&rawtime);

    char    buffer[30] = { '\0' };
    strftime(buffer, sizeof(buffer), format, tm_info);

    return std::string(buffer);
  }

}} //!namespace WS::Utils
