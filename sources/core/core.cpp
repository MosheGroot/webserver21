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
  
  Server& Server::getInstance(std::string ip_addr, int port)
  {
    if (instance_ == NULL)
      instance_ = new Server(ip_addr, port);
    return *instance_;
  }

  /// Logic part

  Server::Server(std::string ip_addr, int port)
  :  ip_addr_(ip_addr), port_(port) { }

  void  Server::init() // need to be remade considering more than 1 listening socket
  {
    
    // create listening socket
    if ((socket_ = socket(AF_INET, SOCK_STREAM, 0)) == -1)
      throw std::runtime_error("Can't create a listening socket()");

    // bind socket
    sockaddr_in hint;
    std::memset(&hint, 0, sizeof(hint));
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port_);
    hint.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(socket_, (sockaddr *)&hint, sizeof(hint)) == -1)
      throw std::runtime_error("Can't bind() the socket");

    // listen socket
    if (listen(socket_, SOMAXCONN) == -1)
      throw std::runtime_error("Can't listen() the socket");

    FD_ZERO(&master_set_);
    FD_SET(socket_, &master_set_);
  }

  int Server::run() //needs refactoring
  {
    std::stringstream ss;
    fd_set            readfds;
    fd_set            writefds;
    int               new_client_socket;

    ss << "ip: " << ip_addr_ << " port: " << port_ << std::endl;
    Utils::Logger::instance_.info(ss.str());
    // Utils::Logger::instance_.info("Waiting for a message / connection...");
    while (true)
    {
      readfds = master_set_;
      writefds = master_set_;
      if (select(FD_SETSIZE, &readfds, &writefds, NULL, NULL) == -1)
        throw std::runtime_error("Can't select() fdsets");

      // check if any socket has changed
      for (size_t socket = 0; socket < FD_SETSIZE; socket++) // add tracking for max socket so far
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
            handleConnection(socket);
          }
        }
      }

      usleep(100); // tmp

      // if (close(client_socket) == -1)
      //   throw std::runtime_error("Can't close() the client's connection");
    
    } // !while (true)
    
    if (close(socket_) == -1)
      throw std::runtime_error("Can't close the socket_"); // need to be moved somewhere else
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
    ss << "Client #" << new_client_socket << " has been accepted";
    Utils::Logger::instance_.info(ss.str());

    // ss1 << "Client #" << new_client_socket << ": s_addr = " << client.sin_addr.s_addr << "; sin_family = " << client.sin_family << "; sin_port = " << client.sin_port;
    // Utils::Logger::instance_.debug(ss1.str());

    return new_client_socket;
  }

  void  Server::handleConnection(int client_socket) const
  {
    recvMsg(client_socket);
    sendMsg(client_socket, "Message has been recieved!\n", sizeof("Message has been recieved!\n"));
  }

  void  Server::recvMsg(int socket_recv_from) const
  {
    std::stringstream ss;
    char              buf[4096]; // max_body_size + header
    int               bytes;

    std::memset(buf, 0, sizeof(buf));
    // Utils::Logger::instance_.info("Waiting for a message...");
    
    switch (bytes = recv(socket_recv_from, buf, 4096, 0)) //?
    {
      case -1:
        throw std::runtime_error("Can't recieve message from client");
      case 0: // normal error
      {
        Utils::Logger::instance_.info("recv() has returned 0");
        break;
      }
      default:
      {
        ss << "#" << socket_recv_from << " Recieved: " << std::string(buf, 0, bytes);
        Utils::Logger::instance_.info(ss.str());
        break;
      }
    }

  }

  void  Server::sendMsg(int socket_to_send, std::string msg, int length) const
  {
    if (send(socket_to_send, msg.c_str(), length, 0) == -1)
      throw std::runtime_error("Can't send() message to the client");
  }

}} //!namespace WS::Core


WS::Core::Server* WS::Core::Server::instance_;