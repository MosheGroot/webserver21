#pragma once

#include "../config/config.hpp"

namespace WS { namespace Utils
{
  /* @brief Debug utils
  */
  class Debug
  {
  private:
    Debug(void) {}

  public:
    /* @brief Print entire webserver config to stdout

        @param conf   Config to print
    */
    static void   printConf(const Config::Config &conf);

  }; //!class Debug
}} //!namespace WS::Utils
