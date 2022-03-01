#include "../../headers/core/pagegenerator.hpp"
#include "../../headers/utils/file.hpp"
#include "../../headers/utils/string.hpp"

#include <vector>
#include <dirent.h>
#include <sstream>
#include <stdexcept>

namespace WS { namespace Core {

  // Traits
  const char * const PageGenerator::error_page_path_    = "resourses/default_pages/error.html";
  const char * const PageGenerator::default_page_path_  = "resourses/default_pages/default.html";

  const char * const PageGenerator::error_page_message_var_   = "{error_message}";
  const char * const PageGenerator::default_page_message_var_ = "{message}";

  // Generators
  std::string   PageGenerator::generateErrorPage(const std::string& error_message, const char *error_page_path)
  {
    std::string page_template = Utils::File::readFile(
      (error_page_path) ? error_page_path : PageGenerator::error_page_path_);

    std::vector<std::string> splitted = Utils::String::splitOnce(
      page_template, PageGenerator::default_page_message_var_);

    return splitted[0] + error_message + splitted[1];
  }

  
  std::string  PageGenerator::generateDefaultPage()
  {
    return "";
  }


  std::string  PageGenerator::generateIndexPage(std::string path)
  {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(path.c_str());
    if (!dir) {
      throw std::invalid_argument("Directory does not");
    }

    if (path[path.size() - 1] != '/')
      path.push_back('/');

    std::stringstream out("autoindex.html");
    out << "<!DOCTYPE html>"                         << '\n';
    out << "<html>"                                  << '\n';
    out << "<title> Index of " << path << "</title>" << '\n';
    out << "<body>"                                  << '\n';
    out << "<div>"                                   << '\n';
    out << "<h1>Index of ";

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
    out << "</div>"                                   << '\n';
    out << "</body>"                                  << '\n';
    out << "</html>"                                  << '\n';

    closedir(dir);
    return out.str();
  }

}};