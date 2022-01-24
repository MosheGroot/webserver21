#pragma once

#include <string>
#include <map>

#include "methods.hpp"

namespace Http
{
  /* @brief Http request class
  */
  class Request
  {
  public:

    // start-line
    Http::Method    method;     //> method of request
    std::string     uri;        //> target uri
    std::string     version;    //> http version of request

    // headers
    std::map<std::string, std::string>  headers;    //> full list of headers

    // body
    std::string body;   //> body of POST request

  }; //!class Request
} //!namespace Http
