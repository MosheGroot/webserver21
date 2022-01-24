#include "../../../headers/http/parser/parser.hpp"
#include "../../../headers/utils/string.hpp"

namespace Http
{
  Request      Parser::deserializeRequest(const std::string& data)
  {
    /// Prepare
    // check raw data
    if (data.empty())
      throw std::invalid_argument("empty request");

    // split headers and body
    std::vector<std::string>  splitted_request = Utils::String::splitOnce(data, "\n\n");
    if (splitted_request.size() != 2)
      throw std::invalid_argument("missing empty string after headers");

    // split start-line and headers
    std::vector<std::string>  splitted_raw_headers = Utils::String::split(
      splitted_request[0], '\n');


    /// Parsing
    Request request;

    // start-line
    {
      std::vector<std::string> splitted_startline = Utils::String::split(
        splitted_raw_headers[0], ' ');

      if (splitted_startline.size() != 3)
        throw std::invalid_argument("invalid start-line of request");

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
        splitted_header = Utils::String::split(splitted_raw_headers[headers_index], ": ");
        request.headers.insert(std::make_pair(splitted_header[0], splitted_header[1]));
        ++headers_index;
      }
    }

    // body
    {
      if (request.method == POST)
        request.body = splitted_request[1];
    }

    /// Return
    return request;
  }


  std::string  Parser::serializeResponse(const Response& data)
  {
    std::stringstream ss;

    /// Serialize
    // start-line
    ss << data.version << ' '
      << data.status_code << ' '
      << data.status_text << '\n';

    // headers
    {
      std::map<std::string, std::string>::const_iterator  iter;
      for (iter = data.headers.begin(); iter != data.headers.end(); ++iter)
        ss << iter->first << ": " << iter->second << '\n';
      ss << '\n'; // empty line between headers and body
    }

    // body
    ss << data.body;

    /// Return
    return ss.str();
  }


  Method    Parser::stringToMethod(const std::string& source)
  {
    if (source == "GET") return GET;
    if (source == "POST") return POST;
    if (source == "DELETE") return DELETE;

    throw std::invalid_argument("unsupported HTTP method");
  }

} //!namespace Http
