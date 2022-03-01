#include "../../headers/core/requesthandler.hpp"
#include "../../headers/core/pagegenerator.hpp"
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
      server = RequestHandler::selectServer(request, ip, port, conf);
      if (server)
        location = RequestHandler::selectLocation(request, *server);

      return Http::Parser::serializeResponse(
        RequestHandler::createResponse(request, server, location)
      );
    }
    catch(const std::exception& e)
    {
      Utils::Logger::error(e.what());
      return Http::Parser::serializeResponse(
        RequestHandler::createErrorResponse(Http::InternalServerError, request, server)
      );
    }
  }


  /// Select
  const Config::ServerConfig*   RequestHandler::selectServer(const Http::Request& request,
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
          return &curr;
        if (!default_server)
          default_server = &curr;
      }  
    }

    return (default_server);
  }


  const Config::ServerLocation* RequestHandler::selectLocation(const Http::Request& request,
                                                                const Config::ServerConfig& server)
  {
    for (size_t i = 0; i < server.location_list.size(); ++i)
    {
      const Config::ServerLocation& curr = server.location_list[i];

      if (curr.path == request.uri)
        return &curr;
    }

    return NULL;
  }


  /// Response
  Http::Response             RequestHandler::createResponse(const Http::Request& request, 
                                                                const Config::ServerConfig* server,
                                                                const Config::ServerLocation* location)
  {
    if (location && !methodIsAllowed(request, *location))
      return RequestHandler::createErrorResponse(Http::MethodNotAllowed, request, server);

    Http::Response response;
    if (request.method == Http::GET)
      response = responseFromGet(request, server, location);
    else if (request.method == Http::POST)
      response = responseFromPost(request, server, location);
    else if (request.method == Http::DELETE)
      response = responseFromDelete(request, server, location);
    
    return response;
  }


  Http::Response             RequestHandler::createErrorResponse(Http::StatusCode code,
                                                                const Http::Request& request, 
                                                                const Config::ServerConfig* server)
  {
    Utils::Logger::error(request.headers.at("Host") + request.uri + ": " + Http::Parser::statusToString(code));

    Http::Response  response;
    response.version = "HTTP/1.1";
    response.status_code = code;

    if (server && server->error_page.find(code) != server->error_page.end())
    {
      // redirect to error page
      
    }
    else
    {
      // default error page
      response.body = PageGenerator::generateErrorPage(Http::Parser::statusToString(code));
    }
    
    return response;
  }


  Http::Response             RequestHandler::createDefaultPageResponse()
  {
    Http::Response  response;
    response.version = "HTTP/1.1";
    response.status_code = Http::Ok;
    
    response.body = PageGenerator::generateDefaultPage();

    return response;
  }


  /// Methods

  Http::Response    RequestHandler::responseFromGet(const Http::Request& request, 
                                                      const Config::ServerConfig* server,
                                                      const Config::ServerLocation* location)
  {
    (void)request;
    (void)server;
    (void)location;

    if (!server)
      return createDefaultPageResponse();
    return Http::Response();
  }

  Http::Response    RequestHandler::responseFromPost(const Http::Request& request, 
                                                      const Config::ServerConfig* server,
                                                      const Config::ServerLocation* location)
  {
    (void)request;
    (void)server;
    (void)location;
    return Http::Response();
  }

  Http::Response    RequestHandler::responseFromDelete(const Http::Request& request, 
                                                        const Config::ServerConfig* server,
                                                        const Config::ServerLocation* location)
  {
    (void)request;
    (void)server;
    (void)location;
    return Http::Response();
  }

  /// Index
  Http::Response    RequestHandler::responseFromAutoIndex(std::string absolute_path)
  {
    Http::Response  response;

    response.version = "HTTP/1.1";
    response.status_code = Http::Ok;
    
    response.body = PageGenerator::generateIndexPage(absolute_path);
    response.headers.insert(std::make_pair("Content-Type", "text/html"));
    
    return response;
  }

  /// Utils
  std::string RequestHandler::getAbsolutePath(const Http::Request& request,
                                              const Config::ServerConfig* server,
                                              const Config::ServerLocation* location)
  {
    // location with root
    if (location && !location->root.empty())
    {
      // std::string subpath = request.uri.substr(request.uri.find(location->path) + );

      // return location->root + subpath;
    }

    // server with root
    if (server && !server->root.empty())
      return server->root + request.uri;
    
    // default ????
    return request.uri;
  }

  bool        RequestHandler::methodIsAllowed(const Http::Request& request, const Config::ServerLocation& location)
  {
    return (std::find(location.method.begin(), location.method.end(),
      Http::Parser::methodToString(request.method)) != location.method.end());
  }

}} //!namespace WS::Core