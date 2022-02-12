#pragma once

#include <iostream>
#include <fstream>
#include <exception>

#include "../models/config.hpp"

namespace WS { namespace Config
{
  /* @brief Static class Parser.
     */
  class Parser
  {

  public:

    /* @brief Parsing and read config file.
      */
    static void   parsFile(const char *filename, Config &out);

    /* @brief Parsing config.
      */
    static void   parseConfig(std::ifstream& data, Config &out);

    /* @brief Parsing server.
      */
    static void   parseServerConfig(std::ifstream& data, Config &out);

    /* @brief Parsing location.
      */
    static void   parseServerLocation(std::ifstream& data, ServerConfig &out, std::string path);

    /* @brief Split string. Separate by spaсe.
      */
    static std::vector<std::string>   splitStr(std::string line);

    /* @brief Exception if file not found
      */
    class	FileNotFoundException: public std::exception{
      virtual const char  *what() const throw();
    };

    /* @brief Exception if config data is wrong
      */
    class	WrongConfigException: public std::exception{
      virtual const char  *what() const throw();
    };

  }; //!class Parser
}} //!namespace WS::Config
