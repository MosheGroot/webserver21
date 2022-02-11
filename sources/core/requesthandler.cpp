#include "../../headers/core/requesthandler.hpp"

namespace WS { namespace Core {

  std::string  RequestHandler::handle(const std::string& request, const std::string& ip, const std::string& port)
  {
    // TODO

    (void)request;
    (void)ip;
    (void)port;

    return "";
  }

  const Config::ServerConfig&   RequestHandler::selectServer(const Http::Request& request,
                                                              const std::string& ip,
                                                              const std::string& port)
  {
    // TODO
  }

  /* @brief Select location from targeted server for specified client's request
  */
  const Config::ServerLocation& RequestHandler::selectLocation(const Http::Request& request,
                                                                const Config::ServerConfig& server)
  {
    // TODO
  }

  /* @brief Creates response based on client's request for targeted server and location inside it
  */
  const std::string             RequestHandler::createResponse(const Http::Request& request, 
                                                                const Config::ServerConfig& server,
                                                                const Config::ServerLocation& location)
  {
    // TODO
  }

}} //!namespace WS::Core