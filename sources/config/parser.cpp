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
      result = Utils::String::splitStr(data);
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
    if (checkIp(new_server.ip_addr))
      out.server_list.push_back(new_server);
    else
      throw WrongIpAddress();
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
      result = Utils::String::splitStr(data);
      int len = (int)result.size();
      if (result[0] == "method" && len > 1)
      {
        for (int i = 1; i < len; i++)
          new_location.method.push_back(result[i]);
      }
      else if (result[0] == "root" && len == 2)
        new_location.root = result[1];
      else if (result[0] == "cgi_path" && len == 2)
        new_location.cgi_path = result[1];
      else if (result[0] == "cgi_pass" && len == 2)
        new_location.cgi_pass = result[1];
      else if (result[0] == "location" && len == 2)
      {
        out.location_list.push_back(new_location);
        new_location.path = result[1];
        new_location.root = "";
        new_location.cgi_pass = "";
        new_location.cgi_path = "";
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

  bool              Parser::checkIp(std::string &ip_addr)
  {
    std::vector<std::string>  data;

    int flag = 0;
    data = Utils::String::split(ip_addr, '.');
    for (size_t i = 0; i < data.size(); i++)
    {
      if (to_int(data[i]) == 255)
        flag += 1;
    }
    if (flag == 4)
      return false;

    return true;
  }

  int               Parser::to_int(std::string &data)
  {
    std::stringstream degree(data);

    int res = 0;
    degree >> res;

    return res;
  }

  const char        *Parser::FileNotFoundException::what() const throw()
  {
    return "Exception thrown: could not open configuration file";
  }

  const char        *Parser::WrongConfigException::what() const throw()
  {
    return "Exception thrown: wrong config file";
  }

  const char        *Parser::WrongIpAddress::what() const throw()
  {
    return "Exception thrown: wrong ip address";
  }

}} //!namespace WS::Config
