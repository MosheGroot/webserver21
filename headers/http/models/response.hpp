#pragma once

#include <string>

namespace WebServer::Http
{
  /* @brief Http Response class
  */
  class Response
  {
  public:
    std::string header;
    std::string data;
  }; //!class WebServer
} //!namespace WebServer::Core