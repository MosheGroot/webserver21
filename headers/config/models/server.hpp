#pragma once

#include <string>
#include <vector>

#include "./location.hpp"
#include "../../http/http.hpp"

namespace WS { namespace Config
{
  /* @brief Server configuration structure that contains information of one server 
            (such as listen ip, port, locations and etc.)
  */
  struct ServerConfig
  {

    std::string ip_addr;
    std::string port;

    std::vector<std::string>    server_name;

    std::map<Http::StatusCode, std::string> error_page;
    
    std::string max_body_size;

    std::vector<ServerLocation> location_list;

  }; //!class ServerConfig
}} //!namespace WS::Config
