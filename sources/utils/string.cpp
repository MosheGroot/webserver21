#include <cctype>

#include "../../headers/utils/string.hpp"

namespace WS { namespace Utils {
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

  std::string  String::toLower(std::string text)
  {
    for (size_t i = 0; i < text.size(); ++i)
      text[i] = ::tolower(text[i]);
    
    return text;
  }

}} //!namespace WS::Utils
