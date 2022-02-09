#include "../../headers/config/parser/parser.hpp"

namespace WS { namespace Config
{
  std::vector<ServerConfig> Config::server_list;

  void              Parser::parseConfig(std::ifstream& conffile, Config &out)
  {
    std::string     data;
    ServerConfig    new_server;
    std::vector<std::string>  result;

    while(!conffile.eof())
    {
      getline(conffile, data);
      std::cout << data << std::endl;
      if (data == "server")
        parseServerConfig(conffile, out);
      else
        throw WrongConfigException();
    }
  }

  void              Parser::parseServerConfig(std::ifstream& conffile, Config &out)
  {
    ServerConfig    new_server;
    std::string     data;
    std::vector<std::string>  result;

    while(!conffile.eof())
    {
      getline(conffile, data);
      result = splitStr(data);
      if (result[0] == "listen")
        new_server.ip_addr = result[1];
      else if (result[0] == "server_name")
        new_server.server_name = result[1];
      else if (result[0] == "port")
        new_server.port = result[1];
      else if (result[0] == "root")
        new_server.root = result[1];
      else if (result[0] == "autoindex")
        new_server.autoindex = result[1];
      else if (result[0] == "buff_size_body")
        new_server.buff_size_body = result[1];
      else if (result[0] == "error_page")
        new_server.error_page = result[1];
      else if (result[0] == "location")
      {
        parseServerLocation(conffile, new_server, result[1]);
        break;
      }
      else
        throw WrongConfigException();
    }
    out.server_list.push_back(new_server);
  }

  void              Parser::parseServerLocation(std::ifstream& conffile, ServerConfig &out, std::string path)
  {
    ServerLocation  new_location;
    std::string     data;
    
    new_location.path = path;
    while(!conffile.eof())
    {
      getline(conffile, data);
      if (data.size() == 0) {
        // std::cout << data  << std::endl;
        break;
      }
    }
    out.location_list.push_back(new_location);
  }

  void              Parser::parsFile(const char *filename, Config &out) 
  {
    std::ifstream   conffile(filename);
    std::string     data;

    if (!conffile.is_open())
      throw FileNotFoundException();
    parseConfig(conffile, out);
    conffile.close();
  }

  std::vector<std::string>   Parser::splitStr(std::string line) 
  {
    std::vector<std::string> result;
    std::string buffer = "";
    int len = (int)line.length();

    for (int i = 0; i < len; i++)
    {
      if (line[i] != ' ')
        buffer += line[i];
      if (line[i] == ' ' || i == len -1)
      {
        result.push_back(buffer);
        buffer = "";
      }
    }
    return result;
  }

  const char        *Parser::FileNotFoundException::what() const throw()
  {
    return "Exception thrown: could not open configuration file";
  }

  const char        *Parser::WrongConfigException::what() const throw()
  {
    return "Exception thrown: wrong config file";
  }

}} //!namespace WS::Config
