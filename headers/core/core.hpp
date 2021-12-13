#pragma once

namespace WS::Core
{
  /* @brief General WebServer class.
  */
  class WebServer
  {
  /// singleton part
    static WebServer *instance_;

    WebServer() {}

  public:
    WebServer(WebServer &other) = delete;

    static WebServer&     getInstance(void);

  /// Logic part
  public:
    /* @brief Entry point of WebServer.
    *         Initiates, configurates and runs the WebServer.
    */
    void  run(void);


  private:
    /* @brief Preparing WebServer before work
    *         (set up logger, read config, add thread poll and etc.)
    *  @exception std::exception  Throws when configuration fails 
    *                             (check error message)
    */    
    void  configure(void);

    /* @brief Main function for handling connections
    */
    void listen(void);
  }; //!class WebServer
} //!namespace WS::Core