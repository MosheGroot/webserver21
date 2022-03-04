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
      if (s_end - s_start) // not empty
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
      if (s_end - s_start) // not empty
        result.push_back(source.substr(s_start, s_end - s_start));
    }

    return result;
  }

  std::vector<std::string>  String::splitOnce(const std::string& source, const std::string& delim)
  {
    std::vector<std::string> result;
    const size_t delim_size = delim.length();

    // first
    size_t s_start = 0;
    size_t s_end = source.find(delim);
    result.push_back(source.substr(s_start, s_end));

    // second
    if (s_end != std::string::npos)
    {
      s_start = s_end;
      s_end = source.find(delim, s_start + delim_size);
      while (s_end != std::string::npos && (s_end - s_start == delim_size))
        s_start += delim_size;
      result.push_back(source.substr(s_start + delim_size, std::string::npos));
    }

    return result;
  }

  std::vector<std::string>  String::splitOnce(const std::string& source, char delim)
  {
    std::vector<std::string> result;

    // first
    size_t s_start = 0;
    size_t s_end = source.find(delim);
    result.push_back(source.substr(s_start, s_end));

    // second
    if (s_end != std::string::npos)
    {
      s_start = s_end;
      while (source[s_start] == delim)
        ++s_start;
      result.push_back(source.substr(s_start, std::string::npos));
    }

    return result;
  }

  std::string   String::toLower(std::string text)
  {
    for (size_t i = 0; i < text.size(); ++i)
      text[i] = ::tolower(text[i]);
    
    return text;
  }

}} //!namespace WS::Utils
