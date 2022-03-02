#include "../../headers/http/parser/parser.hpp"
#include "../../headers/utils/string.hpp"
#include "../../headers/utils/logger.hpp" // < debug

namespace WS { namespace Http
{
  Request      Parser::deserializeRequest(const std::string& data)
  {
    Utils::Logger::debug("Http::Parser::deserializeRequest"); // < DEBUG

    /// Prepare
    // check raw data
    if (data.empty())
      throw std::invalid_argument("deserializeRequest exception: empty request");

    // split headers and body
    std::vector<std::string>  splitted_request = Utils::String::splitOnce(data, "\r\n\r\n");

    // split start-line and headers
    std::vector<std::string>  splitted_raw_headers = Utils::String::split(
      splitted_request[0], "\r\n");


    /// Parsing
    Request request;

    // start-line
    {
      Utils::Logger::debug("Start-line: [" + splitted_raw_headers[0] + "]"); // < DEBUG
      
      std::vector<std::string> splitted_startline = Utils::String::split(
        splitted_raw_headers[0], ' ');

      if (splitted_startline.size() != 3)
        throw std::invalid_argument("deserializeRequest exception: invalid start-line of request");

      request.method = Parser::stringToMethod(splitted_startline[0]);
      request.uri = splitted_startline[1];
      request.version = splitted_startline[2];
    }

    // headers
    {
      std::vector<std::string> splitted_header;
      size_t headers_index = 1;

      while (headers_index < splitted_raw_headers.size())
      {
        // clean up
        size_t newline_index = splitted_raw_headers[headers_index].rfind("\r\n");
        if (newline_index != std::string::npos)
          splitted_raw_headers[headers_index].erase(newline_index, 1);

        // insert
        splitted_header = Utils::String::split(splitted_raw_headers[headers_index], ": ");
        request.headers.insert(std::make_pair(splitted_header[0], splitted_header[1]));
        ++headers_index;
      }
    }

    // body
    {
      if (request.method == POST)
      {
        if (splitted_request.size() <= 1)
          throw std::invalid_argument("deserializeRequest exception: missing empty line after headers");
        request.body = splitted_request[1];
      }
    }

    /// Return
    return request;
  }


  std::string  Parser::serializeResponse(const Response& data)
  {
    Utils::Logger::debug("Http::Parser::serializeResponse"); // < DEBUG

    std::stringstream ss;

    /// Serialize
    // start-line
    ss << data.version << ' '
      << Parser::statusToString(data.status_code) << "\r\n";

    // headers
    {
      std::map<std::string, std::string>::const_iterator  iter;
      for (iter = data.headers.begin(); iter != data.headers.end(); ++iter)
        ss << iter->first << ": " << iter->second << "\r\n";
    }

    // body
    if (!data.body.empty())
    {
      ss << "Content-Length: " << data.body.size();
    }
    
    ss << "\r\n\r\n" // empty line between headers and body
      << data.body;

    /// Return
    return ss.str();
  }


  Method      Parser::stringToMethod(const std::string& source)
  {
    Utils::Logger::debug("Http::Parser::stringToMethod"); // < DEBUG
    
    if (source == "GET") return GET;
    if (source == "POST") return POST;
    if (source == "DELETE") return DELETE;

    throw std::invalid_argument("unsupported HTTP method");
  }

  std::string Parser::methodToString(Method method)
  {
     Utils::Logger::debug("Http::Parser::methodToString"); // < DEBUG
    
    if (method == GET) return "GET";
    if (method == POST) return "POST";
    if (method == DELETE) return "DELETE";

    throw std::invalid_argument("unsupported HTTP method");
  }

  std::string Parser::statusToString(StatusCode status_code)
  {
     Utils::Logger::debug("Http::Parser::statusToString"); // < DEBUG
    
    if (status_code == Continue)            return "100 Continue";
    if (status_code == Continue)            return "100 Continue";
    if (status_code == Processing)          return "102 Processing";
    if (status_code == Ok)                  return "200 OK";
    if (status_code == Created)             return "201 Created";
    if (status_code == Accepted)            return "202 Accepted";
    if (status_code == NoContent)           return "204 No Content";
    if (status_code == MovedPermanently)    return "301 Moved Permanently";
    if (status_code == BadRequest)          return "400 Bad Request";
    if (status_code == Forbidden)           return "403 Forbidden";
    if (status_code == NotFound)            return "404 Not Found";
    if (status_code == MethodNotAllowed)    return "405 Method Not Allowed";
    if (status_code == RequestTimeout)      return "408 Request Timeout";
    if (status_code == LengthRequired)      return "411 Length Required";
    if (status_code == PayloadTooLarge)     return "413 Payload Too Large";
    if (status_code == InternalServerError) return "500 Internal Server Error";
    if (status_code == NotImplemented)      return "501 Not Implemented";
    
    throw std::invalid_argument("unsupported status code");
  }

}} //!namespace WS::Http
