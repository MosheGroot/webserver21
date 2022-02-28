#pragma once

#include <string>

namespace WS { namespace Core {

  class PageGenerator
  {
  private:
    PageGenerator() {} //deleted

    static const char * const error_page_path_;
    static const char * const error_page_message_var_;

    static const char * const default_page_path_;
    static const char * const default_page_message_var_;

  public:
    /* @brief Generate error page with specified error code
    */
    static std::string  generateErrorPage(const std::string& error_message);

    /* @brief Generate default page
    */
    static std::string  generateDefaultPage(void);
  };

}} //!namespace WS::Core