#pragma once

#include "./server.hpp"

namespace WS { namespace Config
{
  /* @brief Config structure that contains 
            all settings and servers of WebServer
  */
  struct Config 
  {

    static std::vector<ServerConfig> server_list; 

  }; //!class Config
}} //!namespace WS::Config
