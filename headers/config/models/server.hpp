#pragma once

#include <string>
#include <vector>

#include "./location.hpp"

namespace WS { namespace Config
{
  /* @brief Server configuration structure that contains information of one server 
            (such as listen ip, port, locations and etc.)
  */
  struct ServerConfig
  {

    std::string ip_addr;
    std::string port;
    std::string root;
    std::string autoindex;
    std::string buff_size_body;
    std::string error_page;
    std::vector<std::string>    server_name;
    std::vector<ServerLocation> location_list;

  }; //!class ServerConfig
}} //!namespace WS::Config
