#include "../../headers/core/requesthandler.hpp"
#include "../../headers/utils/logger.hpp"

#include <algorithm>

namespace WS { namespace Core {

  /// General

  std::string  RequestHandler::handle(const std::string& raw_request, 
                                      const std::string& ip, 
                                      const std::string& port, 
                                      const Config::Config& conf)
  {
    const Http::Request           request = Http::Parser::deserializeRequest(raw_request);
    const Config::ServerConfig*   server = NULL;
    const Config::ServerLocation* location = NULL;

    try
    {
      server = &RequestHandler::selectServer(request, ip, port, conf);  
      location = &RequestHandler::selectLocation(request, *server);

      return RequestHandler::createResponse(request, *server, *location);
    }
    catch(const RequestHandler::ServerNotFoundException& e)
    {
      Utils::Logger::error(e.what());
      return RequestHandler::createErrorResponse(404, request, *server);
    }
    catch(const RequestHandler::LocationNotFoundException& e)
    {
      Utils::Logger::error(e.what());
      return RequestHandler::createErrorResponse(404, request, *server);
    }
    catch(const std::exception& e)
    {
      Utils::Logger::error(e.what());
      return RequestHandler::createErrorResponse(500, request, *server);
    }
  }


  const Config::ServerConfig&   RequestHandler::selectServer(const Http::Request& request,
                                                              const std::string& ip,
                                                              const std::string& port,
                                                              const Config::Config& conf)
  {
    const Config::ServerConfig* default_server = NULL;
    
    for (size_t i = 0; i < conf.server_list.size(); ++i)
    {
      const Config::ServerConfig& curr = conf.server_list[i];

      if (curr.ip_addr == ip && curr.port == port)
      {
        std::vector<std::string>::const_iterator res = std::find(
          curr.server_name.begin(), curr.server_name.end(), request.headers.at("Host"));

        if (res != curr.server_name.end())
          return curr;
        if (!default_server)
          default_server = &curr;
      }  
    }

    if (!default_server)
      throw RequestHandler::ServerNotFoundException();

    return (*default_server);
  }


  const Config::ServerLocation& RequestHandler::selectLocation(const Http::Request& request,
                                                                const Config::ServerConfig& server)
  {
    for (size_t i = 0; i < server.location_list.size(); ++i)
    {
      const Config::ServerLocation& curr = server.location_list[i];

      if (curr.path == request.uri)
        return curr;
    }

    throw RequestHandler::LocationNotFoundException();
  }


  const std::string             RequestHandler::createResponse(const Http::Request& request, 
                                                                const Config::ServerConfig& server,
                                                                const Config::ServerLocation& location)
  {
    if (!methodIsAllowed(request, location))
      return RequestHandler::createErrorResponse(Http::MethodNotAllowed, request, server);

    return "";
  }

  
  const std::string             RequestHandler::createErrorResponse(Http::StatusCode code,
                                                                const Http::Request& request, 
                                                                const Config::ServerConfig& server)
  {
    Utils::Logger::error(request.headers.at("Host") + request.uri + ": " + Http::Parser::statusToString(code));

    Http::Response  response;
    response.status_code = code;

    return Http::Parser::serializeResponse(response);
  }

  /// Utils
  bool   RequestHandler::methodIsAllowed(const Http::Request& request, const Config::ServerLocation& location)
  {
    return (std::find(location.method.begin(), location.method.end(),
      Http::Parser::methodToString(request.method)) != location.method.end());
  }


  /// Exceptions

  const char        *RequestHandler::ServerNotFoundException::what() const throw()
  {
    return "Exception thrown: could not find server for such request";
  }

  const char        *RequestHandler::LocationNotFoundException::what() const throw()
  {
    return "Exception thrown: could not find location for such request";
  }

}} //!namespace WS::Core