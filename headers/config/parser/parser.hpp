#pragma once

#include "../models/config.hpp"

namespace WebServer::Config
{
  /* @brief Static class Parser, provides method for parsing an entire config
            or one of servers from config.
  */
  static class Parser
  {
  public:

    /* @brief Function for parsing an entire config from string.
    *  @param[in] data  string with config to parse
    *  @return          Filled Config structure.
    */
    static Config       parseConfig(const std::string& data);

    /* @brief Function for parsing one server from string.
              Data string should begin from `server <name>` and 
              it will be readed until end of one server (the rest of data in string will be ignored).
       @param[in] data  string with config to parse
       @return          Filled ServerConfig structure.
    */
    static ServerConfig parseServerConfig(const std::string& data);

  }; //!class Parser
} //!namespace WebServer::Config