#include "../../headers/utils/string.hpp"

namespace Utils
{
  std::vector<std::string>  String::split(const std::string& source, const std::string& delim)
  {
    std::vector<std::string> result;

    const size_t delim_size = delim.length();
    size_t s_start = 0;
    size_t s_end = source.find(delim);
    result.push_back(source.substr(s_start, s_end));

    while (s_end != std::string::npos)
    {
      s_start = s_end + delim_size;
      s_end = source.find(delim, s_start);
      result.push_back(source.substr(s_start, s_end - s_start));
    }

    return result;
  }

  std::vector<std::string>  String::split(const std::string& source, char delim)
  {
    std::vector<std::string> result;

    size_t s_start = 0;
    size_t s_end = source.find(delim);
    result.push_back(source.substr(s_start, s_end));

    while (s_end != std::string::npos)
    {
      s_start = s_end + 1;
      s_end = source.find(delim, s_start);
      result.push_back(source.substr(s_start, s_end - s_start));
    }

    return result;
  }

  std::vector<std::string>  String::splitOnce(const std::string& source, const std::string& delim)
  {
    std::vector<std::string> result;

    const size_t delim_size = delim.length();
    size_t s_start = 0;
    size_t s_end = source.find(delim);

    result.push_back(source.substr(s_start, s_end));
    if (s_end != std::string::npos)
      result.push_back(source.substr(s_end + delim_size));

    return result;
  }

  std::vector<std::string>  String::splitOnce(const std::string& source, char delim)
  {
    std::vector<std::string> result;

    size_t s_start = 0;
    size_t s_end = source.find(delim);

    result.push_back(source.substr(s_start, s_end));
    if (s_end != std::string::npos)
      result.push_back(source.substr(s_end + 1));

    return result;
  }

  std::vector<std::string>   String::splitStr(std::string line) 
  {
    std::vector<std::string> result;
    std::string buffer = "";
    int len = (int)line.length();

    for (int i = 0; i < len; i++)
    {
      if (line[i] != ' ')
        buffer += line[i];
      if ((line[i] == ' ' && buffer.size() > 0) || i == len -1)
      {
        result.push_back(buffer);
        buffer = "";
      }
    }
    return result;
  }

  // template <typename T>
  // std::string                 to_string(T data)
  // {
  //   std::ostringstream result;

  //   result << data;
  //   return result.str();
  // }

} //!namespace Utils
