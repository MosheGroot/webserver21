#pragma once

namespace WS { namespace Config
{
  /* @brief Server's location 
            with specified URI and acceptable HTTP method
  */
  struct ServerLocation
  {

    std::string path;
    std::string root;
    std::vector<std::string> method;

  }; //!class ServerLocation
}} //!namespace WS::Config
