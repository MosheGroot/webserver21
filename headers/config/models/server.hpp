#pragma once

#include <string>
#include <vector>

#include "./location.hpp"

namespace WS::Config
{
  /* @brief Server configuration structure that contains information of one server 
            (such as listen ip, port, resources directories and etc.)
  */
  struct ServerConfig

  {

    std::string listen;
    std::string port;
    std::string server_name;
    std::string root;
    std::string autoindex;
    std::string buff_size_body;
    
    // < list of locations
    std::vector<ServerLocation> location_list;

  }; //!class ServerConfig
} //!namespace WS::Config