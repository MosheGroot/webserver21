#pragma once

#include <sstream>

#include "../models/request.hpp"
#include "../models/response.hpp"
#include "../models/methods.hpp"

namespace WS::Http
{
  /* @brief Static class Parser, provides method for parsing
            HTTP methods from string
  */
  class Parser
  {
  public:

    /* @brief Function for serializing Request to ready-to-send string.
    *  @param[in] data  Request to serialize
    *  @return          Raw serialized request in std::string container.
    */
    static std::string  serializeRequest(const Request& data, Method method);

    /* @brief Function for deserializing response 
              from raw data in std::string container
       @param[in] data  string with raw data to parse
       @return          filled Response structure.
    */
    static Response     deserializeResponse(const std::string& data, Method method);

    static const std::map<Method, std::string>  methodToString;
    static const std::map<std::string, Method>  StringToMethod;

  }; //!class Parser
} //!namespace WS::Http