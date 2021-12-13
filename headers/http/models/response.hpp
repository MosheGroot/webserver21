#pragma once

#include <string>

namespace WS::Http
{
  /* @brief Http response class
  */
  class Response
  {
  public:
    std::string header;
    std::string data;
  }; //!class Response
} //!namespace WS::Http