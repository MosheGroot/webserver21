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
<<<<<<< HEAD
    static std::string  handle(const std::string& raw_request, 
                                const std::string& ip, 
                                const std::string& port, 
                                const Config::Config& conf);
=======
    static std::string  handle(const std::string& request, const std::string& ip, const std::string& port);
>>>>>>> 7eead6099a7f863458bdc51a17d844c0329ed728

  private:
    /* @brief Select server from global config for specified client's request
    */
    static const Config::ServerConfig&    selectServer(const Http::Request& request,
                                                        const std::string& ip,
<<<<<<< HEAD
                                                        const std::string& port,
                                                        const Config::Config& conf);
=======
                                                        const std::string& port);
>>>>>>> 7eead6099a7f863458bdc51a17d844c0329ed728

    /* @brief Select location from targeted server for specified client's request
    */
    static const Config::ServerLocation&  selectLocation(const Http::Request& request,
                                                          const Config::ServerConfig& server);

    /* @brief Creates response based on client's request for targeted server and location inside it
    */
    static const std::string              createResponse(const Http::Request& request, 
                                                          const Config::ServerConfig& server,
                                                          const Config::ServerLocation& location);

<<<<<<< HEAD
    /* @brief Creates response with error page specified in server's config 
    */
    static const std::string              createErrorResponse(int error_code,
                                                              const Http::Request& request, 
                                                              const Config::ServerConfig& server);

  public:
    /* @brief Exception if location not found
    */
    struct	ServerNotFoundException: public std::exception {
      virtual const char  *what() const throw();
    };

    /* @brief Exception if location not found
    */
    struct	LocationNotFoundException: public std::exception {
      virtual const char  *what() const throw();
    };

=======
>>>>>>> 7eead6099a7f863458bdc51a17d844c0329ed728
  }; //!class RequestHandler
}} //!namespace WS::Core