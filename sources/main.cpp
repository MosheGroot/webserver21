#include "../headers/core/core.hpp"
#include "../headers/utils/logger.hpp"

int main(int argc, const char *argv[])
{
  const char *config_path = "resources/configs/default.conf";
  if (argc >= 2)
    config_path = argv[1];

  try
  {
    WS::Utils::Logger::init(WS::Utils::Logger::LOGLEV_DEBUG, true);
    WS::Core::Server& server = WS::Core::Server::instance_;

    server.init(config_path);
    server.run();
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}
