#pragma once

#include <string>

namespace WS::Http
{
  /* @brief Http request class
  */
  class Request
  {
  public:
    std::string header;
    std::string data;
  }; //!class Request
} //!namespace WS::Http