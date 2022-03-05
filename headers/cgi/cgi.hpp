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

namespace WS { namespace CGI
{
  /* @brief Singleton class for CGI (throught bash)
  */
  class Handler
  {

    /// Signleton part
    private:
      Handler() {} // delete

      Handler(const Handler& other) {(void)other;} // delete

    public:
      static Handler  instance_;



    /// Logic part
    // General
    public:

      /* @brief Exec cgi file as bash script
      */
      std::string exec(const std::string& script_path,
                        const Http::Request& request,
                        const Config::ServerConfig& server);

    private:
      /* @brief Init enviroment variables for next execve
      */
      void          initEnv(const std::string& script_path,
                            const Http::Request& request,
                            const Config::ServerConfig& server);

      /* @brief Check header and add in ENV if it exist

        @return True if header exists and added to ENV, false otherwise
      */
      bool          addHeaderToEnv(const Http::Request &request,
                                    const std::string& header, 
                                    const std::string& env_param);

      /* @brief Execute CGI script
      */
      std::string   executeCgi(const std::string& script, 
                                const std::string& body);

      /* @brief ENVS for CGI.
        */
      std::map<std::string, std::string> env_;


    // Utils
    private:


      /* @brief Convert ENVS to usable char** array
      */
      char          **getCharEnv(void);
    

    // Exceptions
    public:
      /* @brief Exception if config data is wrong
      */
      class	ErrorMemoryException: public std::exception {
        virtual const char  *what() const throw();
      };
  }; //!class CGI
}} //!namespace WS::CGI
