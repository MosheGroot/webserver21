#pragma once

#include <iostream>
#include <string>
#include <map>
#include <new>
#include <string.h>
#include <unistd.h>

#include "../config/models/config.hpp"

namespace WS { namespace Cgi
{
  class Cgi
  {
    
    private:

      /* @brief ENVS for CGI.
        */
      std::map<std::string, std::string> env_;

      /* @brief init ENVS.
        */
      void          initEnv_(Config::ServerConfig &serv, Config::ServerLocation &serv_location, Http::Request &request);


    public:

      std::string   executeCgi(const std::string& script, Config::ServerConfig &serv, Config::ServerLocation &serv_location, Http::Request &request);
      char          **envChar();

      /* @brief Exception if config data is wrong
        */
      // class	ErrorMemoryException: public std::exception {
      //   virtual const char  *what() const throw();
      // };

  }; //!class CGI
}} //!namespace WS::Cgi
