#pragma once

#include <string>

namespace WS::Config
{
  /* @brief Server configuration structure that contains information of one server 
            (such as listen ip, port, resources directories and etc.)
  */
  struct ServerConfig
  {
    
    std::string ip;
    std::string port;

  }; //!class ServerConfig
} //!namespace WS::Config