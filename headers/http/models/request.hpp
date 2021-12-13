#pragma once

#include <string>

namespace WebServer::Http
{
  /* @brief Http request class
  */
  class Request
  {
  public:
    std::string header;
    std::string data;
  }; //!class WebServer
} //!namespace WebServer::Core