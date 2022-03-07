#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sys/select.h> 


#define CLIENT_DISCONNECTED -1


#include "./requesthandler.hpp"
#include "../../headers/config/config.hpp"

namespace WS { namespace Core
{
  /* @brief General Server class.
  */
  class Server
  {
  
  /// Signleton
  public:

    static Server instance_;

  private:

    Server() { }
    Server(Server&) { }
    Server& operator=(const Server&) { return *this; }

  /// General
  public:
    /* @brief Server initialization.
      *  @exception std::exception  Throws when function fails 
      *                             (check error message)
      */
    void    init(const char* config_path);

    /* @brief Runs the server.
      *  @exception std::exception  Throws when function fails 
      *                             (check error message)
      */
    int     run(void);
    

  /// Init
  private:
    /* @brief Init configuration file
    */
    void    initConfig(const char* config_path);

    /* @brief Init connections (not servers) list with IP:Port pairs
    */
    void    initConnectionsSet(void);

    /* @brief Init listening sockets
    */
    void    initSockets(void);


  /// Runtime
  //  Connections
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


  //  Recieve and send
    /* @brief Recieve a message from a client
      */
    int   recvMsg(int socket_recv_from);

    /* @brief Handle a message recieved from a client
      */
    int   handleMsg(std::string msg, int msg_owner);

    /* @brief Send a message to a client
      */
    void  sendMsg(int socket_to_send, const char* msg, int msg_size) const;


  //  Closing
    /* @brief Close all listening sockets
      */
    void  closeListeningSockets() const;

  private:
  /// General
  // config
    const Config::Config  conf_;

  // sockets
    /* @brief Data struct of info about one connection,
              that can be used by different servers
    */
    struct ConnectionInfo
    {
      std::string ip_addr;
      std::string port;

      ConnectionInfo() {}

      ConnectionInfo(const std::string& ip_addr, const std::string& port)
        : ip_addr(ip_addr), port(port)
      {}

      bool operator<(const struct ConnectionInfo& second) const // for std::set
      {
        if (this->ip_addr == second.ip_addr)
          return (this->port < second.port);
        return (this->ip_addr < second.ip_addr);
      }
    };
    
    fd_set                                      master_set_;        // master set of all sockets
    std::set<struct ConnectionInfo>             connections_set_;   // list of connection infos
    std::vector<int>                            listening_sockets_; // list of listening sockets
    std::map<int, const struct ConnectionInfo*> socket_infos_;      // < socket to ConnectionInfo map

  }; //!class Server
}} //!namespace WS::Core