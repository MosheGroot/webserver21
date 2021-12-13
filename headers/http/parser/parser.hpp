#pragma once

#include "../models/request.hpp"
#include "../models/response.hpp"

namespace WebServer::Http
{
  /* @brief Static class Parser, provides method for parsing
            HTTP methods from string
  */
  static class Parser
  {
  public:

    /* @brief Function for serializing Request to ready-to-send string.
    *  @param[in] data  Request to serialize
    *  @return          Raw serialized request in std::string container.
    */
    static std::string  serializeRequest(const Request& data);

    /* @brief Function for deserializing response 
              from raw data in std::string container
       @param[in] data  string with raw data to parse
       @return          filled Response structure.
    */
    static Response     deserializeResponse(const std::string& data);

  }; //!class Parser
} //!namespace WebServer::Config