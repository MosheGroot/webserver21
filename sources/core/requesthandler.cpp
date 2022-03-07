#include "../../headers/core/requesthandler.hpp"
#include "../../headers/core/pagegenerator.hpp"
#include "../../headers/cgi/cgi.hpp"
#include "../../headers/utils/logger.hpp"
#include "../../headers/utils/file.hpp"
#include "../../headers/utils/string.hpp"
#include "../../headers/utils/exceptions.hpp"

#include <algorithm>
#include <exception>
#include <unistd.h>

namespace WS { namespace Core {

  /// General
  std::string  RequestHandler::handle(const std::string& raw_request, 
                                      const std::string& ip, 
                                      const std::string& port, 
                                      const Config::Config& conf)
  {
    Utils::Logger::debug("RequestHandler::handle"); // < DEBUG

    Http::Request                 request;
    const Config::ServerConfig*   server = NULL;
    const Config::ServerLocation* location = NULL;

    std::string  response;

    try
    {
      request = Http::Parser::deserializeRequest(raw_request);

      if (conf.server_list.size() == 0)
      {
        // imposible
        response = RequestHandler::createDefaultPageResponse();
      }
      else
      {
        server = RequestHandler::selectServer(request, ip, port, conf);
        location = RequestHandler::selectLocation(request, *server);
        if (!location)
          response = RequestHandler::createErrorResponse(Http::NotFound, request, server);
        else
          response = RequestHandler::createResponse(request, server, location);
      }
    }
    catch(const std::exception& e)
    {
      Utils::Logger::error(e.what());
      response = RequestHandler::createErrorResponse(Http::InternalServerError, request, server);
    }

    return response;
  }


  /// Select
  const Config::ServerConfig*   RequestHandler::selectServer(const Http::Request& request,
                                                              const std::string& ip,
                                                              const std::string& port,
                                                              const Config::Config& conf)
  {
    Utils::Logger::debug("RequestHandler::selectServer"); // < DEBUG
    
    const Config::ServerConfig* default_server = NULL;
    
    for (size_t i = 0; i < conf.server_list.size(); ++i)
    {
      const Config::ServerConfig& curr = conf.server_list[i];

      if (curr.ip_addr == ip && curr.port == port)
      {
        const std::string& host = request.headers.at("Host");
        for (size_t i = 0; i < curr.server_name.size(); ++i)
          if (curr.server_name[i] + ":" + curr.port == host)
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
    Utils::Logger::debug("RequestHandler::selectLocation"); // < DEBUG

    const Config::ServerLocation* best_prefix_location = NULL;
    size_t                  best_prefix_len = 0;

    for (size_t i = 0; i < server.location_list.size(); ++i)
    {
      const Config::ServerLocation& curr = server.location_list[i];

      if (request.uri == curr.path)
        return &curr;

      if (request.uri.find(curr.path) == 0 && curr.path.size() > best_prefix_len) // check prefix 
      {
        best_prefix_location = &curr;
        best_prefix_len = curr.path.size();
      }
    }

    return best_prefix_location;
  }


  /// Response
  std::string             RequestHandler::createResponse(const Http::Request& request, 
                                                                const Config::ServerConfig* server,
                                                                const Config::ServerLocation* location)
  {
    Utils::Logger::debug("RequestHandler::createResponse"); // < DEBUG

    if (location)
    {
      if (!methodIsAllowed(request, *location))
        return RequestHandler::createErrorResponse(Http::MethodNotAllowed, request, server);

      if (!location->redirect.empty()) // redirect
        return RequestHandler::createRedirectResponse(location->redirect);

      if (location && !location->cgi_path.empty())
        return RequestHandler::createCGIResponse(request, server, location);
    }

    std::string path = RequestHandler::getAbsolutePath(request, location);

    std::string response;
    if (request.method == Http::GET)
      response = responseFromGet(path, request, server, location);
    else if (request.method == Http::POST)
      response = responseFromPost(path, request, server);
    else if (request.method == Http::DELETE)
      response = responseFromDelete(path, request, server);

    return response;
  }


  std::string             RequestHandler::createErrorResponse(Http::StatusCode code,
                                                                const Http::Request& request, 
                                                                const Config::ServerConfig* server)
  {
    Utils::Logger::debug("RequestHandler::createErrorResponse"); // < DEBUG
    try
    {
      Utils::Logger::error(request.headers.at("Host") + request.uri + ": " + Http::Parser::statusToString(code));
    }
    catch(...)
    {
    }
    

    Http::Response  response(code);

    if (server && server->error_page.find(code) != server->error_page.end())
    {
      // redirect to error page
      // TODO
      response.body = PageGenerator::generateErrorPage();
    }
    else
    {
      // default error page
      response.body = PageGenerator::generateErrorPage();
    }

    return Http::Parser::serializeResponse(response);
  }


  std::string             RequestHandler::createDefaultPageResponse()
  {
    Utils::Logger::debug("RequestHandler::createDefaultPageResponse"); // < DEBUG
    Http::Response  response(Http::Ok);
    
    response.body = PageGenerator::generateDefaultPage();

    response.headers.insert(std::make_pair("Content-Type", "text/html"));

    return Http::Parser::serializeResponse(response);
  }

  
  std::string      RequestHandler::createRedirectResponse(const std::string& redirect_url)
  {
    Http::Response  response(Http::MovedPermanently);

    response.headers.insert(std::make_pair("Location", redirect_url));

    return Http::Parser::serializeResponse(response);
  }


  std::string      RequestHandler::createCGIResponse(const Http::Request& request,
                                                      const Config::ServerConfig* server,
                                                      const Config::ServerLocation* location)
  {
    /// Check max body size
    if (request.body.size() > static_cast<size_t>(::atoi(server->max_body_size.c_str())))
      return RequestHandler::createErrorResponse(Http::PayloadTooLarge, request, server);


    /// Get script path
    std::string script_path;

    if (location->cgi_path[0] != '/')
      script_path = Utils::File::getCurrentDir() + '/' + location->cgi_path;
    else
      script_path = location->cgi_path;

    Utils::Logger::debug("CGI SCRIPT PATH : " + script_path); // < DEBUG

    if (!Utils::File::fileExists(script_path.c_str())) // config error
      return createErrorResponse(Http::InternalServerError, request, server);


    /// Exec CGI
    std::string cgi_response = CGI::Handler::instance_.exec(script_path, request, *server);    
    Utils::Logger::info("{" + cgi_response + "}");

    /// Get response
    Http::Response  response(Http::Ok);

    return Http::Parser::serializeResponse(response, false) + cgi_response;
  }



  /// Methods

  std::string    RequestHandler::responseFromGet(const std::string& absolute_path,
                                                      const Http::Request& request,
                                                      const Config::ServerConfig* server,
                                                      const Config::ServerLocation* location)
  {
    Utils::Logger::debug("RequestHandler::responseFromGet"); // < DEBUG
    
    /// case 1. Location processing
    Utils::Logger::debug("Absolute path : " + absolute_path); // < DEBUG
    Utils::Logger::debug("Location path : " + location->path); // < DEBUG

    if (location && (
      Utils::File::isDir(absolute_path.c_str())
      || !location->index.empty()))
    {
      Utils::Logger::debug("RequestHandler::responseFromGet CASE 1"); // < DEBUG
      try
      {
        return responseFromLocationIndex(absolute_path, request, *location);
      }
      catch(const Utils::Exceptions::FileDoesNotExist& e)
      {
        if (location->autoindex == "on")
          return responseFromAutoIndex(absolute_path, request);

        return createErrorResponse(Http::NotFound, request, server);
      }
      catch(...)
      {
        throw;
      }
    }

    /// case 2. Open file
    Utils::Logger::debug("RequestHandler::responseFromGet CASE 2"); // < DEBUG
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

    // generate response
    Http::Response response(Http::Ok);

    response.body = data;

    response.headers.insert(std::make_pair(
      "Content-Type",
      RequestHandler::getContentType(absolute_path)
    ));

    return Http::Parser::serializeResponse(response);
  }


  std::string    RequestHandler::responseFromPost(const std::string& absolute_path,
                                                      const Http::Request& request, 
                                                      const Config::ServerConfig* server)
  {
    Utils::Logger::debug("RequestHandler::responseFromPost"); // < DEBUG

    // check max body size
    if (request.body.size() > static_cast<size_t>(::atoi(server->max_body_size.c_str())))
      return RequestHandler::createErrorResponse(Http::PayloadTooLarge, request, server);

    // check if path is a dir
    if (Utils::File::isDir(absolute_path.c_str()))
      return RequestHandler::createErrorResponse(Http::BadRequest, request, server);

    // create response
    Http::Response  response;

    if (Utils::File::fileExists(absolute_path.c_str()))
    {
      Utils::File::writeFile(request.body, absolute_path.c_str(), true);
      response.status_code = Http::NoContent;
    }
    else
    {
      Utils::File::writeFile(request.body, absolute_path.c_str());
      response.status_code = Http::Created;
    }

    return Http::Parser::serializeResponse(response);
  }


  std::string    RequestHandler::responseFromDelete(const std::string& absolute_path,
                                                        const Http::Request& request, 
                                                        const Config::ServerConfig* server)
  {
    Utils::Logger::debug("RequestHandler::responseFromDelete"); // < DEBUG

    if (!Utils::File::fileExists(absolute_path.c_str()))
      return RequestHandler::createErrorResponse(Http::NotFound, request, server);
    if (Utils::File::isDir(absolute_path.c_str()))
      return RequestHandler::createErrorResponse(Http::Conflict, request, server);

    Http::Response response;

    std::string data = Utils::File::readFile(absolute_path.c_str());
    ::remove(absolute_path.c_str());

    response.status_code = Http::Ok;
    response.body = data;

    response.headers.insert(std::make_pair(
      "Content-Type",
      RequestHandler::getContentType(absolute_path)
    ));

    return Http::Parser::serializeResponse(response);
  }



  /// Index

  std::string    RequestHandler::responseFromLocationIndex(const std::string& absolute_path,
                                                            const Http::Request& request,
                                                            const Config::ServerLocation& location)
  {
    Utils::Logger::debug("RequestHandler::responseFromLocationIndex"); // < DEBUG

    // Index file searching
    size_t i_file;
    for (i_file = 0; i_file < location.index.size(); ++i_file)
    {
      if (Utils::File::fileExists((absolute_path + location.index[i_file]).c_str()))
        break;
    }

    if (i_file == location.index.size()) //index file doesnt exist
    {
      if (location.autoindex == "on")
        return responseFromAutoIndex(absolute_path, request);
      throw Utils::Exceptions::FileDoesNotExist();
    }

    // Response generating
    Http::Response response(Http::Ok);

    response.body = Utils::File::readFile((absolute_path + location.index[i_file]).c_str());

    response.headers.insert(std::make_pair(
      "Content-Type",
      RequestHandler::getContentType(location.index[i_file])
    ));

    return Http::Parser::serializeResponse(response);
  }


  std::string    RequestHandler::responseFromAutoIndex(const std::string& absolute_path, 
                                                        const Http::Request& request)
  {
    Utils::Logger::debug("RequestHandler::responseFromAutoIndex"); // < DEBUG
    Http::Response  response(Http::Ok);

    response.body = PageGenerator::generateIndexPage(absolute_path, request.uri);
    response.headers.insert(std::make_pair("Content-Type", "text/html"));

    return Http::Parser::serializeResponse(response);
  }



  /// Utils

  std::string RequestHandler::getAbsolutePath(const Http::Request& request,
                                              const Config::ServerLocation* location)
  {
    Utils::Logger::debug("RequestHandler::getAbsolutePath"); // < DEBUG
    
    // case 1. location with root (delete location path part)
    if (location && !location->root.empty())
    {
      Utils::Logger::debug("RequestHandler::getAbsolutePath CASE 1"); // < DEBUG
      std::string subpath = request.uri;

      subpath.erase(0, location->path.size());

      if (location->root[0] != '/')
        return Utils::File::getCurrentDir() + "/" + location->root + subpath;
      return location->root + subpath;
    }

    // case 2. default (work dir + uri)
    Utils::Logger::debug("RequestHandler::getAbsolutePath CASE 2"); // < DEBUG
    return (Utils::File::getCurrentDir() + request.uri);
  }


  bool        RequestHandler::methodIsAllowed(const Http::Request& request, const Config::ServerLocation& location)
  {
    Utils::Logger::debug("RequestHandler::methodIsAllowed"); // < DEBUG

    if (!location.method.size()) // empty
      return true;

    return (std::find(location.method.begin(), location.method.end(),
      Http::Parser::methodToString(request.method)) != location.method.end());
  }


  std::string RequestHandler::getContentType(const std::string& filename)
  {
    Utils::Logger::debug("RequestHandler::getContentType"); // < DEBUG
    
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