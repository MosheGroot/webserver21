#pragma once

#include "../http/http.hpp"
#include "../config/models/server.hpp"
#include "../config/models/location.hpp"
#include "../config/models/config.hpp"


#define CONNECTION_CLOSE     -1
#define CONNECTION_KEEPALIVE  0


namespace WS { namespace Core
{
  /* @brief Handler for client's requests
  */
  class RequestHandler
  {
  private:
    RequestHandler() {}

  public:
  /// General
    /* @brief Handle one request from specific ip and port

      @return Serialized response
    */
    static std::string  handle(const std::string& raw_request, 
                                const std::string& ip, 
                                const std::string& port, 
                                const Config::Config& conf,
                                int& connection_status);


  private:
  /// Select
    /* @brief Select server from global config for specified client's request
    */
    static const Config::ServerConfig*    selectServer(const Http::Request& request,
                                                        const std::string& ip,
                                                        const std::string& port,
                                                        const Config::Config& conf);

    /* @brief Select location from targeted server for specified client's request
    */
    static const Config::ServerLocation*  selectLocation(const Http::Request& request,
                                                          const Config::ServerConfig& server);


  /// Response
    /* @brief Creates response based on client's request for targeted server and location inside it
    */
    static std::string      createResponse(const Http::Request& request, 
                                              const Config::ServerConfig* server,
                                              const Config::ServerLocation* location);

    /* @brief Creates response with error page specified in server's config 
    */
    static std::string      createErrorResponse(Http::StatusCode code,
                                                    const Http::Request& request, 
                                                    const Config::ServerConfig* server);

    /* @brief Creates response with default page  
    */
    static std::string      createDefaultPageResponse(void);

    /* @brief Creates redirect response to specified redirect_url  
    */
    static std::string      createRedirectResponse(const std::string& redirect_url);

    /* @brief Creates CGI response for specific CGI
    */
    static std::string      createCGIResponse(const Http::Request& request,
                                              const Config::ServerConfig* server,
                                              const Config::ServerLocation* location);


  /// Methods
    static std::string      responseFromGet(const std::string& absolute_path,
                                              const Http::Request& request, 
                                              const Config::ServerConfig* server,
                                              const Config::ServerLocation* location);

    static std::string      responseFromPost(const std::string& absolute_path,
                                              const Http::Request& request, 
                                              const Config::ServerConfig* server);

    static std::string      responseFromDelete(const std::string& absolute_path,
                                                const Http::Request& request, 
                                                const Config::ServerConfig* server);


  /// Index
    static std::string   responseFromLocationIndex(const std::string& absolute_path,
                                                    const Http::Request& request,
                                                    const Config::ServerLocation& location);

    static std::string   responseFromAutoIndex(const std::string& absolute_path,
                                                const Http::Request& request);


  /// Utils
    static std::string  getAbsolutePath(const Http::Request& request,
                                          const Config::ServerLocation* location);

    static bool         methodIsAllowed(const Http::Request& request, const Config::ServerLocation& location);

    static std::string  getContentType(const std::string& content_extention);

  }; //!class RequestHandler
}} //!namespace WS::Core