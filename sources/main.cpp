#include "../headers/core/core.hpp"
#include "../headers/config/parser/parser.hpp"

void  printConf(WS::Config::Config &conf)
{
  int n = (int)conf.server_list.size();

  for (int i = 0; i < n; i++)
  {
    std::cout << "\n ------- SERVER -------" << std::endl;
    std::cout << "ip_addr         " << conf.server_list[i].ip_addr << std::endl;

    std::cout << "server_name     ";
    int s = (int)conf.server_list[i].server_name.size();
    if (s > 0)
    {
      for (int v = 0; v < s; v++)
        std::cout << conf.server_list[i].server_name[v] << " ";      
    }
    std::cout << std::endl;

    std::cout << "port            " << conf.server_list[i].port << std::endl;
    std::cout << "root            " << conf.server_list[i].root << std::endl;
    std::cout << "autoindex       " << conf.server_list[i].autoindex << std::endl;
    std::cout << "buff_size_body  " << conf.server_list[i].buff_size_body << std::endl;
    std::cout << "error_page      " << conf.server_list[i].error_page << std::endl;

    int m = (int)conf.server_list[i].location_list.size();
    for (int j = 0; j < m; j++)
    {
      std::cout << "location:" << std::endl;
      std::cout << "    path      " << conf.server_list[i].location_list[j].path << std::endl;
      std::cout << "    root      " << conf.server_list[i].location_list[j].root << std::endl;

      std::cout << "    method    ";
      int l = (int)conf.server_list[i].location_list[j].method.size();
      if (l > 0)
      {
        for (int k = 0; k < l; k++)
          std::cout << conf.server_list[i].location_list[j].method[k] << " ";
      }
      std::cout << std::endl;
    }
  }
}

int main(int argc, char *argv[])
{
  if (argc != 2)
    return 0;

  WS::Config::Config conf;
  WS::Config::Parser::parsFile(argv[1], conf);
  // printConf(conf);

  WS::Core::Server& server = WS::Core::Server::getInstance("IP_ADDR", 54001);

  try
  {
    server.init();
    server.run();
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}
