#pragma once

namespace WebServer::Core
{
  /* @brief General WebServer class.
  */
  class WebServer
  {
  public:

    /* @brief Entry point of WebServer.
    *         Initiates, configurates and runs the WebServer.
    */
    void  run(void);

  }; //!class WebServer
} //!namespace WebServer::Core