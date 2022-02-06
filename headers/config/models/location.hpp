#pragma once

#include <string>

namespace WS { namespace Config
{
  /* @brief Server's location 
            with specified URI and acceptable HTTP method
  */
  struct ServerLocation
  {

    std::string path;
    std::string method;

  }; //!class ServerLocation
}} //!namespace WS::Config