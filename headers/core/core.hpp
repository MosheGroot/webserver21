#pragma once

#include <iostream>
#include <sys/select.h> 

#define CLIENT_DISCONNECTED -1

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
    Server(Server&) { }
    Server& operator=(const Server&) { return *this; }

  public:

    /* @brief Server initialization.
      *  @exception std::exception  Throws when function fails 
      *                             (check error message)
      */
    void  init(void);

    /* @brief Runs the server.
      *  @exception std::exception  Throws when function fails 
      *                             (check error message)
      */
    int   run(void);
    
    /* @brief Preparing Server before work
      *         (set up logger, read config, add thread poll and etc.)
      *  @exception std::exception  Throws when configuration fails 
      *                             (check error message)
      */    
    void  configure(void); // ?

  private:

    /* @brief Tells if the socket is a listening socket
      */
    bool  isListening(int socket) const;

    /* @brief Function for accepting client connection
      *  @exception std::exception  Throws when accept() fails 
      *                             (check error message)
      */
    int   acceptConnection(int listening_socket) const;

    /* @brief Handler for client connections
      */
    void  handleConnection(int client_socket, fd_set& writefds);

    /* @brief Handler for client disconnections
      */
    void  handleDisconnection(int client_socket);

    /* @brief Recieve a message from a client
      */
    int   recvMsg(int socket_recv_from);

    /* @brief Handle a message recieved from a client
      */
    void  handleMsg(std::string msg, int msg_owner) const;

    /* @brief Send a message to a client
      */
    void  sendMsg(int socket_to_send, const char* msg, int msg_size) const;

  private:
    
    static Server*    instance_;
    const std::string ip_addr_;
    int               port_;
    int               socket_;
    fd_set            master_set_;

  }; //!class Server
}} //!namespace WS::Core