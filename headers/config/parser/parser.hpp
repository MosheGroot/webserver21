#pragma once

#include <iostream>
#include <fstream>
#include <exception>

#include "../models/config.hpp"

namespace WS { namespace Config
{
  /* @brief Static class Parser, provides method for parsing an entire config
            or one of servers from config.
  */
  class Parser
  {
  public:

    /* @brief Function for parsing an entire config from string.
    *  @param[in] data  string with config to parse
    *  @return          Filled Config structure.
    */
    static void         parseConfig(std::ifstream& data, Config &out);

    /* @brief Function for parsing one server from string.
              Data string should begin from `server <name>` and 
              it will be readed until end of one server (the rest of data in string will be ignored).
       @param[in] data  string with config to parse
       @return          Filled ServerConfig structure.
    */
    static void   parseServerConfig(std::ifstream& data, Config &out);

    static void parseServerLocation(std::ifstream& data, ServerConfig &out, std::string path);
    static void           parsFile(const char *filename, Config &out);
    static std::vector<std::string>   splitStr(std::string line);

    class	FileNotFoundException: public std::exception{
      virtual const char  *what() const throw();
    };

    class	WrongConfigException: public std::exception{
      virtual const char  *what() const throw();
    };

  }; //!class Parser
}} //!namespace WS::Config
