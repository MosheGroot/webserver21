#pragma once

#include <iostream>

namespace WS { namespace Core
{
  /* @brief General Server class.
  */
  class Server
  {
  
  public:
  
    Server(std::string ip_addr, int port);
    static Server&  getInstance(std::string ip_addr, int port);

  private:

    Server() { }
    Server(Server& other) { }
    Server& operator=(const Server& other) { }

  public:

    /* @brief Server initialization.
      */
    void	init(void);

    /* @brief Runs the server.
      */
    int		run(void);
    
    /* @brief Preparing Server before work
      *         (set up logger, read config, add thread poll and etc.)
      *  @exception std::exception  Throws when configuration fails 
      *                             (check error message)
      */    
    void	configure(void); // ?

    /* @brief Handler for client connections
      */
    void	handleConnection(int client_socket);

    /* @brief Handler for client disconnections
      */
    void	handleDisconnection(int client_socket);

    /* @brief Send a message to a client
      */
    void	sendMsg(int	socket_to_send, std::string msg, int length);

    /* @brief Recieve a message from a client
      */
    void	recvMsg(int	socket_recv_from, std::string msg, int length);

    /* @brief Handle a message recieved from a client
      */
    void	handleMsg();

  private:
    
    static Server*    instance_;
    const std::string ip_addr_;	// ip address from config
    int               port_;		// port from config
    int               socket_;	// listening socket

  }; //!class Server
}} //!namespace WS::Core