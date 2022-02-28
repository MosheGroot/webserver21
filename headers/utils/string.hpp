#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <exception>

namespace Utils
{
  /* @brief Static class for String processing
  */
  class String
  {
  public:
    /*  @brief Split string by delimeter
        @param source       string to split
        @param delim        delimiter as string
    */
    static std::vector<std::string>  split(const std::string& source, const std::string& delim);

    /*  @brief Split string by delimeter
        @param source       string to split
        @param delim        delimiter as char
    */
    static std::vector<std::string>  split(const std::string& source, char delim);


    /*  @brief Split string to two parts by delimeter (if it exists in string)
        @param source       string to split
        @param delim        delimiter as string
    */
    static std::vector<std::string>  splitOnce(const std::string& source, const std::string& delim);

    /*  @brief Split string to two parts by delimeter (if it exists in string)
        @param source       string to split
        @param delim        delimiter as char
    */
    static std::vector<std::string>  splitOnce(const std::string& source, char delim);

    /* @brief Split string for parsing. Separate by spaсe.
      */
    static std::vector<std::string>   splitStr(std::string line);

    /* @brief Some type to string
      */
    template <typename T>
    static std::string                to_string(T data);

  }; //!class String
} //!namespace Utils
