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
              in default format:
      @return Timestamp in std::string container.
    */
    static std::string getTimestamp();

  }; //!class File
}} //!namespace WS::Utils
