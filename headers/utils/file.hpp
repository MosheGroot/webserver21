#pragma once

#include <string>

namespace WS { namespace Utils
{
  /* @brief Utils for file processing
  */
  class File
  {
  private:
    File(void) {}

  public:
    /* @brief Read entire file to std::string container

      @param filepath   path to target file
    
      @return All bytes of file in std::string
    */
    static std::string readFile(const char *filepath);

  }; //!class File
}} //!namespace WS::Utils
