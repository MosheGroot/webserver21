#pragma once

#include <iostream>
#include <fstream>
#include <exception>

#include "../models/config.hpp"
#include "../../utils/string.hpp"

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

    /* @brief Check ip.
      */
    static bool   checkIp(std::string &ip_addr);

    /* @brief Convert to int.
      */
    static int    to_int(std::string &data);

    /* @brief Exception if file not found
      */
    class	FileNotFoundException: public std::exception {
      virtual const char  *what() const throw();
    };

    /* @brief Exception if config data is wrong
      */
    class	WrongConfigException: public std::exception {
      virtual const char  *what() const throw();
    };

    /* @brief Exception if wrong ip address
      */
    class	WrongIpAddress: public std::exception {
      virtual const char  *what() const throw();
    };

  }; //!class Parser
}} //!namespace WS::Config
