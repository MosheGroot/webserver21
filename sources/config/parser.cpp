#include "../../headers/config/config.hpp"

namespace WS { namespace Config
{
  void              Parser::parseConfig(std::ifstream& conffile, Config &out)
  {
    std::string     data;
    ServerConfig    new_server;
    std::vector<std::string>  result;

    while(!conffile.eof())
    {
      getline(conffile, data);
      if (data.size() == 0)
        continue;
      else if (data == "server")
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
      int len = (int)result.size();
      if (result[0] == "listen" && len == 2)
        new_server.ip_addr = result[1];
      else if (result[0] == "server_name" && len > 1)
      {
        for (int i = 1; i < len; i++) 
          new_server.server_name.push_back(result[i]);
      }
      else if (result[0] == "port" && len == 2)
        new_server.port = result[1];
      else if (result[0] == "root" && len == 2)
        new_server.root = result[1];
      else if (result[0] == "index" && len == 2)
        new_server.index = result[1];
      else if (result[0] == "autoindex" && len == 2)
        new_server.autoindex = result[1];
      else if (result[0] == "buff_size_body" && len == 2)
        new_server.buff_size_body = result[1];
      else if (result[0] == "error_page" && len == 2)
        new_server.error_page = result[1];
      else if (result[0] == "location" && len == 2)
      {
        parseServerLocation(conffile, new_server, result[1]);
        break;
      }
      else
        throw WrongConfigException();
    }
    if (new_server.port == "")
      new_server.port = "8080";
    if (new_server.ip_addr == "" || new_server.ip_addr == "localhost")
      new_server.ip_addr = "127.0.0.1";
    out.server_list.push_back(new_server);
  }

  void              Parser::parseServerLocation(std::ifstream& conffile, ServerConfig &out, std::string path)
  {
    ServerLocation  new_location;
    std::string     data;
    std::vector<std::string>  result;

    new_location.path = path;
    while(!conffile.eof())
    {
      getline(conffile, data);
      if (data.size() == 0)
        break;
      result = splitStr(data);
      int len = (int)result.size();
      if (result[0] == "method" && len > 1)
      {
        for (int i = 1; i < len; i++)
          new_location.method.push_back(result[i]);
      }
      else if (result[0] == "root" && len == 2)
        new_location.root = result[1];
      else if (result[0] == "index" && len == 2)
        new_location.index = result[1];
      else if (result[0] == "location" && len == 2)
      {
        out.location_list.push_back(new_location);
        new_location.path = result[1];
        new_location.root = "";
        new_location.method.clear();
      }
      else if (len == 0)
        break;
      else
        throw WrongConfigException();
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
      if ((line[i] == ' ' && buffer.size() > 0) || i == len -1)
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
