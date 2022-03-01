#include "../../headers/core/requesthandler.hpp"
#include "../../headers/core/pagegenerator.hpp"
#include "../../headers/utils/logger.hpp"
#include "../../headers/utils/file.hpp"
#include "../../headers/utils/string.hpp"
#include "../../headers/utils/exceptions.hpp"

#include <algorithm>
#include <exception>

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

    Http::Response  response;
    
    try
    {
      server = RequestHandler::selectServer(request, ip, port, conf);
      if (!server)
      {
        response = RequestHandler::createDefaultPageResponse();
      }
      else
      {
        location = RequestHandler::selectLocation(request, *server);      
        response = RequestHandler::createResponse(request, server, location);
      }
    }
    catch(const std::exception& e)
    {
      Utils::Logger::error(e.what());
      response = RequestHandler::createErrorResponse(Http::InternalServerError, request, server);
    }

    return Http::Parser::serializeResponse(response);
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

    std::string path = RequestHandler::getAbsolutePath(request, location);

    Http::Response response;
    if (request.method == Http::GET)
      response = responseFromGet(path, request, server, location);
    else if (request.method == Http::POST)
      response = responseFromPost(path, request, server, location);
    else if (request.method == Http::DELETE)
      response = responseFromDelete(path, request, server, location);
    
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
      // TODO
      response.body = PageGenerator::generateErrorPage(Http::Parser::statusToString(code));
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

  Http::Response    RequestHandler::responseFromGet(const std::string& absolute_path,
                                                      const Http::Request& request,
                                                      const Config::ServerConfig* server,
                                                      const Config::ServerLocation* location)
  {
    // case 1. Location processing
    if (location && (
      Utils::File::isDir(absolute_path.c_str()) 
      || absolute_path.find(location->path) != std::string::npos
    ))
    {
      try
      {
        return responseFromLocationIndex(absolute_path, location);
      }
      catch(const Utils::Exceptions::FileDoesNotExist& e)
      {
        if (location->autoindex == "on")
          return responseFromAutoIndex(absolute_path);

        return createErrorResponse(Http::NotFound, request, server);
      }
      catch(...)
      {
        throw;
      }
    }

    // case 2. Open file
    std::string data;
    try
    {
      data = Utils::File::readFile(absolute_path.c_str());
    }
    catch(const Utils::Exceptions::FileDoesNotExist& e)
    {
      return createErrorResponse(Http::NotFound, request, server);
    }
    catch(...)
    {
      throw;
    }

    // case 3. HEAD method, but it's not implemented
    return createErrorResponse(Http::NotImplemented, request, server);
  }

  Http::Response    RequestHandler::responseFromPost(const std::string& absolute_path,
                                                      const Http::Request& request, 
                                                      const Config::ServerConfig* server,
                                                      const Config::ServerLocation* location)
  {
    (void)absolute_path;
    (void)request;
    (void)server;
    (void)location;

    // TODO

    return createErrorResponse(Http::NotImplemented, request, server);
  }

  Http::Response    RequestHandler::responseFromDelete(const std::string& absolute_path,
                                                        const Http::Request& request, 
                                                        const Config::ServerConfig* server,
                                                        const Config::ServerLocation* location)
  {
    (void)absolute_path;
    (void)request;
    (void)server;
    (void)location;

    // TODO

    return createErrorResponse(Http::NotImplemented, request, server);
  }

  /// Index
  Http::Response    RequestHandler::responseFromLocationIndex(const std::string& absolute_path,
                                                              const Config::ServerLocation* location)
  {
    // Index file searching
    size_t i_file;
    for (i_file = 0; i_file < location->index.size(); ++i_file)
    {
      if (Utils::File::fileExists(location->index[i_file].c_str()))
        break;
    }

    if (i_file == location->index.size()) //index file doesnt exist
    {
      if (location->autoindex == "on")
        return responseFromAutoIndex(absolute_path);
      throw Utils::Exceptions::FileDoesNotExist();
    }

    // Response generating
    Http::Response response;

    response.version = "HTTP/1.1";
    response.status_code = Http::Ok;

    response.body = Utils::File::readFile((absolute_path + location->index[i_file]).c_str());

    response.headers.insert(std::make_pair(
      "Content-Type",
      RequestHandler::getContentType(location->index[i_file])
    ));

    return response;
  }

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
                                              const Config::ServerLocation* location)
  {
    // case 1. location with root (delete location path part)
    if (location && !location->root.empty())
    {
      std::string subpath = request.uri;

      subpath.erase(0, location->path.size());

      return location->root + subpath;
    }
    
    // case 2. default (work dir + uri)
    return (Utils::File::getCurrentDir() + request.uri);
  }

  bool        RequestHandler::methodIsAllowed(const Http::Request& request, const Config::ServerLocation& location)
  {
    return (std::find(location.method.begin(), location.method.end(),
      Http::Parser::methodToString(request.method)) != location.method.end());
  }

  std::string RequestHandler::getContentType(const std::string& filename)
  {
    size_t found_index = filename.rfind('.');
    if (found_index == std::string::npos) {
      return "text/plain";
    }

    std::string ext = Utils::String::toLower(filename.substr(found_index));

    if (ext == ".html") return "text/html";
    if (ext == ".css")  return "text/css";
    if (ext == ".js")   return "text/javascript";
    if (ext == ".gif")  return "image/gif";
    if (ext == ".jpg")  return "image/jpeg";
    if (ext == ".jpeg") return "image/jpeg";
    if (ext == ".png")  return "image/png";

    return "text/plain";
  }

}} //!namespace WS::Core