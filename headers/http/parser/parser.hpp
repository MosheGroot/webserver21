#pragma once

#include <sstream>

#include "../models/request.hpp"
#include "../models/response.hpp"
#include "../models/methods.hpp"

namespace WS { namespace Http
{
  /* @brief Static class Parser, provides method for parsing
            HTTP methods from string
  */
  class Parser
  {
    //  Helpful docs:
    //    https://developer.mozilla.org/ru/docs/Web/HTTP/Messages

  public:

    /* @brief Function for deserializing Request from client.
    *  @param[in] data  client's request to deserialize.
    *  @return          filled Request structure.
    */
    static Request      deserializeRequest(const std::string& data);

    /* @brief Function for serializing server Response to ready-to-send
        data in std::string container.
       @param[in] data  string with raw data to parse
       @return          filled Response structure.
       Raw serialized request in std::string container
    */
    static std::string  serializeResponse(const Response& data);


  private:
    static Method       stringToMethod(const std::string& source);
    static std::string  statusToString(StatusCode status_code);


  }; //!class Parser
}} //!namespace WS::Http
