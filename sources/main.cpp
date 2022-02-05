#include "../headers/core/core.hpp"

int main()
{
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