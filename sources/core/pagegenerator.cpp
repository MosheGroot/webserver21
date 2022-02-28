#include "../../headers/core/pagegenerator.hpp"
#include "../../headers/utils/file.hpp"
#include "../../headers/utils/string.hpp"

#include <vector>

namespace WS { namespace Core {

  // Traits
  const char * const PageGenerator::error_page_path_    = "resourses/default_pages/error.html";
  const char * const PageGenerator::default_page_path_  = "resourses/default_pages/default.html";

  const char * const PageGenerator::error_page_message_var_   = "{error_message}";
  const char * const PageGenerator::default_page_message_var_ = "{message}";

  // Generators
  std::string   PageGenerator::generateErrorPage(const std::string& error_message)
  {
    std::string page_template = Utils::File::readFile(PageGenerator::error_page_path_);

    std::vector<std::string> splitted = Utils::String::splitOnce(
      page_template, PageGenerator::default_page_message_var_);

    return splitted[0] + error_message + splitted[1];
  }
}};