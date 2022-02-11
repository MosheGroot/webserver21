#pragma once

#include "../http/http.hpp"
#include "../config/models/server.hpp"
#include "../config/models/location.hpp"
#include "../config/models/config.hpp"

namespace WS { namespace Core
{
  /* @brief Handler for client's requests
  */
  class RequestHandler
  {
  private:
    RequestHandler() {}

  public:
    /* @brief Handle one request from specific ip and port

      @return Serialized response
    */
    static std::string  handle(const std::string& request, const std::string& ip, const std::string& port);

  private:
    /* @brief Select server from global config for specified client's request
    */
    static const Config::ServerConfig&    selectServer(const Http::Request& request,
                                                        const std::string& ip,
                                                        const std::string& port);

    /* @brief Select location from targeted server for specified client's request
    */
    static const Config::ServerLocation&  selectLocation(const Http::Request& request,
                                                          const Config::ServerConfig& server);

    /* @brief Creates response based on client's request for targeted server and location inside it
    */
    static const std::string              createResponse(const Http::Request& request, 
                                                          const Config::ServerConfig& server,
                                                          const Config::ServerLocation& location);

  }; //!class RequestHandler
}} //!namespace WS::Core