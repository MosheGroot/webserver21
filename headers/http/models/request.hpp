#pragma once

#include <string>
#include <map>

namespace WS::Http
{
  /* @brief Http request class
  */
  class Request
  {
  public:
    const std::string version = "HTTP/1.1";

    std::string                         uri;
    std::map<std::string, std::string>  headers;
    std::string                         data;
  
  }; //!class Request
} //!namespace WS::Http