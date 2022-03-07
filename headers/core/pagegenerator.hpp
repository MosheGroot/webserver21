#pragma once

#include <string>
#include "../http/http.hpp"

namespace WS { namespace Core {

  class PageGenerator
  {
  private:
    PageGenerator() {} //deleted

    static const std::string  error_pages_dir_;
    static const std::string  default_page_path_;


  public:
    /* @brief Generate default error page with specified error code
    */
    static std::string  generateErrorPage(Http::StatusCode status_code);

    /* @brief Generate error page with custom html file
    */
    static std::string  generateErrorPage(const char *error_page_path);


    /* @brief Generate default page
    */
    static std::string  generateDefaultPage(void);


    /* @brief Generate default page
    */
    static std::string  generateIndexPage(const std::string& path,
                                          std::string request_uri);

  };

}} //!namespace WS::Core