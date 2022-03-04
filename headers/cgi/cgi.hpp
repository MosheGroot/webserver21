#pragma once

#include <iostream>
#include <string>
#include <map>
#include <new>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>

#include "../config/models/config.hpp"
#include "../http/http.hpp"

namespace WS { namespace Cgi
{
  class Cgi
  {
    
    private:

      /* @brief ENVS for CGI.
        */
      std::map<std::string, std::string> env_;

    public:

      std::string   executeCgi(const std::string& script);
      char          **envChar();

      /* @brief init ENVS.
        */
      void          initEnv(Config::ServerConfig &serv, Http::Request &request);

      /* @brief Exception if config data is wrong
        */
      class	ErrorMemoryException: public std::exception {
        virtual const char  *what() const throw();
      };

  }; //!class CGI
}} //!namespace WS::Cgi
