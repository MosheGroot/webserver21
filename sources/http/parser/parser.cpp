#include "../../../headers/http/parser/parser.hpp"

namespace WS::Http
{
  std::string  Parser::serializeRequest(const Request& data, Method method)
  {
    std::stringstream ss;
    ss << Parser::methodToString[method] << ' '
      << data.uri << ' '
      << data.version << '\n';
  }

  Response     Parser::deserializeResponse(const std::string& data, Method method)
  {

  }

  const std::map<Method, std::string>  Parser::methodToString = {
    {GET, "GET"},
    {POST, "POST"},
    {DELETE, "DELETE"}
  };

  const std::map<std::string, Method>  Parser::StringToMethod = {
    {"GET", GET},
    {"POST", POST},
    {"DELETE", DELETE}
  }

} //!namespace WS::Http