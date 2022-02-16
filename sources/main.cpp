#include "../headers/core/core.hpp"
#include "../headers/config/config.hpp"
#include "../headers/utils/debug.hpp"
#include "../headers/utils/logger.hpp"

int main(int argc, const char *argv[])
{
  const char *config_name = "resources/configs/default.conf";
  if (argc >= 2)
    config_name = argv[1];

  try
  {
    WS::Utils::Logger::init(WS::Utils::Logger::LOGLEV_DEBUG, true);
    WS::Core::Server& server = WS::Core::Server::getInstance();

    server.init(config_name);
    server.run();
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}
