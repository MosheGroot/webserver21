#include "../../headers/core/core.hpp"
#include "../../headers/utils/logger.hpp"

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <unistd.h> // only for close() [very fckng inconvenient]

namespace WS { namespace Core
{
  /// Singleton part
  
  Server& Server::getInstance(std::string ip_addr, int port, const Config::Config& conf)
  {
    if (instance_ == NULL)
      instance_ = new Server(ip_addr, port, conf);
    return *instance_;
  }

  /// Logic part

  Server::Server(std::string ip_addr, int port, const Config::Config& conf)
  :  ip_addr_(ip_addr), port_(port), conf_(conf) { }

  void  Server::init() // need to be remade considering more than 1 listening socket
  {
    if ((socket_ = socket(AF_INET, SOCK_STREAM, 0)) == -1)
      throw std::runtime_error("Can't create a listening socket()");

    int enable = 1;
    if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1)
      throw std::runtime_error("Can't setsockopt() a listening socket()");

    sockaddr_in hint;
    std::memset(&hint, 0, sizeof(hint));
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port_);
    hint.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(socket_, (sockaddr *)&hint, sizeof(hint)) == -1)
      throw std::runtime_error("Can't bind() the socket");

    if (listen(socket_, SOMAXCONN) == -1)
      throw std::runtime_error("Can't listen() the socket");

    FD_ZERO(&master_set_);
    FD_SET(socket_, &master_set_);
  }

  int Server::run()
  {
    std::stringstream ss;
    fd_set            readfds;
    fd_set            writefds;
    int               new_client_socket;

    ss << "ip: " << ip_addr_ << " port: " << port_ << std::endl;
    Utils::Logger::instance_.info(ss.str());
    
    while (true)
    {
      readfds = master_set_;
      writefds = master_set_;
      if (select(FD_SETSIZE, &readfds, &writefds, NULL, NULL) == -1)
        throw std::runtime_error("Can't select() fdsets");

      // check if any socket has changed
      for (int socket = 0; socket < FD_SETSIZE; socket++) // add tracking for max socket for replacing FD_SETSIZE
      {
        if (FD_ISSET(socket, &readfds))
        {
          if (isListening(socket))
          {
            new_client_socket = acceptConnection(socket);
            FD_SET(new_client_socket, &master_set_);
          }
          else
          {
            handleConnection(socket, writefds);
          }
        }
      }

      usleep(100); // because too hot...
    
    } // !while (true)
    
    if (close(socket_) == -1)
      throw std::runtime_error("Can't close the socket_"); // kinda useless unless we handle ctr+c; move in separate function
  }

  bool  Server::isListening(int socket) const
  {
    if (socket == socket_)
      return true;
    return false;
  }

  int  Server::acceptConnection(int listening_socket) const
  {
    std::stringstream ss;
    // std::stringstream ss1;
    sockaddr_in       client;
    socklen_t         client_size = sizeof(client);
    int               new_client_socket;
    
    if ((new_client_socket = accept(listening_socket, (sockaddr *)&client, &client_size)) == -1)
      throw std::runtime_error("Can't accept() the client");
    ss << "Client #" << new_client_socket << " has been accepted" << std::endl;
    Utils::Logger::instance_.info(ss.str());

    // ss1 << "Client #" << new_client_socket << ": s_addr = " << client.sin_addr.s_addr << "; sin_family = " << client.sin_family << "; sin_port = " << client.sin_port;
    // Utils::Logger::instance_.debug(ss1.str());

    return new_client_socket;
  }

  void  Server::handleConnection(int client_socket, fd_set& writefds)
  {
    if (recvMsg(client_socket) != CLIENT_DISCONNECTED && FD_ISSET(client_socket, &writefds)) //?
      sendMsg(client_socket, "Message has been recieved!\n", sizeof("Message has been recieved!\n"));
  }

  void  Server::handleDisconnection(int client_socket)
  {
    std::stringstream ss;

    if (close(client_socket) == -1)
          throw std::runtime_error("Can't close() the client's connection");
    FD_CLR(client_socket, &master_set_);
    ss << "Client #" << client_socket << " has disconnected";
    Utils::Logger::instance_.info(ss.str());
  }

  void  Server::handleMsg(std::string msg, int msg_owner) const
  {
    std::stringstream ss;

    ss << "#" << msg_owner << " Recieved: " << msg;
      Utils::Logger::instance_.info(ss.str());
  }

  int  Server::recvMsg(int socket_recv_from)
  {
    char              buf[4096]; // max_body_size + header
    int               bytes;

    std::memset(buf, 0, sizeof(buf));
    
    switch (bytes = recv(socket_recv_from, buf, 4096, 0))
    {
      case -1:
        throw std::runtime_error("Can't recieve() message from the client");
      case 0:
      {
        handleDisconnection(socket_recv_from);
        return CLIENT_DISCONNECTED;
      }
      default:
      {
        handleMsg(std::string(buf, 0, bytes), socket_recv_from);
        return 0;
      }
    }
  }

  void  Server::sendMsg(int socket_to_send, const char* msg, int msg_size) const
  {
    if (send(socket_to_send, msg, msg_size, 0) == -1)
      throw std::runtime_error("Can't send() message to the client"); // It's a regular error, not an exception
  }

}} //!namespace WS::Core

WS::Core::Server* WS::Core::Server::instance_;