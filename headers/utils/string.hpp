#pragma once

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

  }; //!class String
} //!namespace Utils
