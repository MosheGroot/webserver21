#include "../../headers/core/pagegenerator.hpp"
#include "../../headers/utils/file.hpp"
#include "../../headers/utils/string.hpp"
#include "../../headers/utils/logger.hpp"

#include <vector>
#include <dirent.h>
#include <sstream>
#include <stdexcept>

namespace WS { namespace Core {

  // Traits
  const std::string PageGenerator::error_pages_dir_    = "resources/default_pages/errors/";
  const std::string PageGenerator::default_page_path_  = "resources/default_pages/default_index.html";

  // Generators
  std::string   PageGenerator::generateErrorPage(Http::StatusCode status_code)
  {
    Utils::Logger::debug("PageGenerator::generateErrorPage"); // < DEBUG

    return Utils::File::readFile((
      PageGenerator::error_pages_dir_
      + Utils::String::to_string(static_cast<int>(status_code))
      + ".html").c_str());
  }

  std::string   PageGenerator::generateErrorPage(const char *error_page_path)
  {
    Utils::Logger::debug("PageGenerator::generateErrorPage"); // < DEBUG

    return Utils::File::readFile(error_page_path);
  }

  
  std::string  PageGenerator::generateDefaultPage()
  {
    Utils::Logger::debug("PageGenerator::generateDefaultPage"); // < DEBUG
    
    return Utils::File::readFile(PageGenerator::default_page_path_.c_str());
  }


  std::string  PageGenerator::generateIndexPage(const std::string& absolute_path,
                                                std::string request_uri)
  {
    Utils::Logger::debug("PageGenerator::generateIndexPage");       // < DEBUG
    Utils::Logger::debug("Index absolute_path : " + absolute_path); // < DEBUG
    Utils::Logger::debug("Index request uri   : " + request_uri);  // < DEBUG

    // Create update request uri dir
    if (request_uri[request_uri.size() - 1] != '/')
      request_uri.push_back('/');

    // Open dir
    DIR *dir;
    dir = opendir(absolute_path.c_str());
    if (!dir)
      throw std::invalid_argument("generateIndexPage exception: directory does not exists");

    /// Generate page
    // output stream
    std::stringstream ss;

    // headers and styles
    ss << "<!DOCTYPE html>\n"
            "<html>\n"
            "<style>\n"
              "html { color-scheme: light dark; }\n"
              "body { width: 35em; margin: 0 auto;\n"
              "font-family: Tahoma, Verdana, Arial, sans-serif; }\n"
            "</style>\n"
           "<title> Index of " << request_uri << "</title>\n"
           "<body>\n"
           "<div>\n";

    // header text
    ss << "<h1>Index of " << request_uri << "</h1>\n";

    // List block
    ss << "<hr>\n"
          "<pre>\n";

    for (struct dirent *dirEntry = readdir(dir); dirEntry; dirEntry = readdir(dir)) {
      ss << "<p><a href=\"" << request_uri + dirEntry->d_name << "\">";

      ss << dirEntry->d_name;
      if (dirEntry->d_type & DT_DIR)
        ss << "/";

      ss << "</a></p>" << '\n';
    }
    closedir(dir);

    ss << "</pre>\n"
          "</hr>\n";

    // ending
    ss << "</div>\n"
           "</body>\n"
           "</html>";

    return ss.str();
  }

}};