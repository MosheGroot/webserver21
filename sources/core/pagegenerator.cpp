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
  const char * const PageGenerator::error_page_path_    = "resources/default_pages/error.html";
  const char * const PageGenerator::default_page_path_  = "resources/default_pages/default_index.html";

  // Generators
  std::string   PageGenerator::generateErrorPage(const char *error_page_path)
  {
    Utils::Logger::debug("PageGenerator::generateErrorPage"); // < DEBUG
    std::string page = Utils::File::readFile(
      (error_page_path) ? error_page_path : PageGenerator::error_page_path_);

    return page;
  }

  
  std::string  PageGenerator::generateDefaultPage()
  {
    Utils::Logger::debug("PageGenerator::generateDefaultPage"); // < DEBUG
    
    return Utils::File::readFile(PageGenerator::default_page_path_);
  }


  std::string  PageGenerator::generateIndexPage(std::string path)
  {
    Utils::Logger::debug("PageGenerator::generateIndexPage"); // < DEBUG
    
    DIR *dir;
    struct dirent *entry;

    Utils::Logger::debug(path);
    dir = opendir(path.c_str());
    if (!dir)
      throw std::invalid_argument("generateIndexPage exception: directory does not exists");

    if (path[path.size() - 1] != '/')
      path.push_back('/');

    std::stringstream out("autoindex.html");
    out << "<!DOCTYPE html>\n"
            "<html>\n"
            "<style>\n"
              "html { color-scheme: light dark; }\n"
              "body { width: 35em; margin: 0 auto;\n"
              "font-family: Tahoma, Verdana, Arial, sans-serif; }\n"
            "</style>\n"
           "<title> Index of " << path << "</title>\n"
           "<body>\n"
           "<div>\n"
           "<h1>Index of ";

    std::string host_(path);
    out << path;
    out << "</h1>" << '\n';
    out << "<hr>"                                    << '\n';
    while ((entry = readdir(dir)) != NULL) {
      out << "<p><a href=\"" << host_ + entry->d_name << "\">";
      out << entry->d_name;
      if (entry->d_type & DT_DIR)
        out << "/";
      out << "</a></p>" << '\n';
    }
    out << "</div>\n" 
           "</body>\n"
           "</html>";

    closedir(dir);
    return out.str();
  }

}};