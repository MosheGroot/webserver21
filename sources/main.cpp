#include "../headers/core/core.hpp"
#include "../headers/config/config.hpp"
#include "../headers/utils/debug.hpp"
#include "../headers/utils/logger.hpp"
#include "../headers/http/http.hpp"
#include "../headers/cgi/cgi.hpp"

int main(int argc, const char *argv[])
{
  const char *config_name = "resources/configs/default.conf";
  if (argc >= 2)
    config_name = argv[1];

  WS::Config::Config conf;
  WS::Config::Parser::parsFile(config_name, conf);
  // WS::Utils::Debug::printConf(conf); // < DEBUG

  WS::Core::Server& server = WS::Core::Server::getInstance("IP_ADDR", 54001, conf);

  try
  {
    WS::Utils::Logger::init(WS::Utils::Logger::LOGLEV_DEBUG, true);

    server.init();
    

    WS::Http::Request test;
    test.uri = "localhost";
    test.body = "ky-ky";
    test.method = WS::Http::POST;
    test.version = "HTTP.1/1";
    WS::Cgi::Cgi test_env;
    test_env.initEnv(conf.server_list[0], conf.server_list[0].location_list[0], test); // FOR CGI TEST
    std::string test_cgi = test_env.executeCgi("/home/uliana/labs/school_21/webserver21/resources/cgi-script/first.py"); // FOR CGI TEST
    std::cout << "------>" << test_cgi << std::endl;
    // server.run();
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}
