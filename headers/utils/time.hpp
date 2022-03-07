#pragma once

#include <string>

namespace WS { namespace Utils
{
  /* @brief Utils for time processing
  */
  class Time
  {
  private:
    Time(void) {}

  public:
    /* @brief Get timestamp of current time
              with specified format
              ("%Y-%m-%d %H:%M:%S" by default)

      @param format   Format of timestamp to return

      @return Timestamp in std::string container.
    */
    static std::string getTimestamp(const char *format = "%Y-%m-%d %H:%M:%S", bool localtime=true);

  }; //!class File
}} //!namespace WS::Utils
