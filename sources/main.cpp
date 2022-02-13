#include "../headers/core/core.hpp"
#include "../headers/config/config.hpp"
#include "../headers/utils/debug.hpp"
#include "../headers/utils/logger.hpp"

int main(int argc, char *argv[])
{
  if (argc != 2)
    return 0;

  WS::Config::Config conf;
  WS::Config::Parser::parsFile(argv[1], conf);
  // WS::Utils::Debug::printConf(conf); // < DEBUG

  WS::Core::Server& server = WS::Core::Server::getInstance("IP_ADDR", 54001, conf);

  try
  {
    WS::Utils::Logger::init(WS::Utils::Logger::LOGLEV_DEBUG, true);

    server.init();
    server.run();
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}
