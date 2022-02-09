#include "../headers/core/core.hpp"
#include "../headers/utils/logger.hpp"

int main()
{
  WS::Core::Server& server = WS::Core::Server::getInstance("IP_ADDR", 54001);

  try
  {
    WS::Utils::Logger::init();

    server.init();
    server.run();
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}
