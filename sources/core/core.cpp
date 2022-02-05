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

  void  Server::init()
  {
    // create listening socket
    if ((socket_ = socket(AF_INET, SOCK_STREAM, 0)) == -1)
      throw std::runtime_error("Can't create listening socket");

    // bind socket
    sockaddr_in hint;
    std::memset(&hint, 0, sizeof(hint));
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port_);
    hint.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(socket_, (sockaddr *)&hint, sizeof(hint)) == -1)
      throw std::runtime_error("Can't bind listening socket");

    // listen socket
    if (listen(socket_, SOMAXCONN) == -1)
      throw std::runtime_error("Can't listen socket");
  }

  int Server::run() //need refactoring
  {
    sockaddr_in client;
    socklen_t   client_size = sizeof(client);
    int         client_socket;
    char        buf[4096]; // think about how to handle buffer size
    char        answer[32] = "Message has been recieved!\n"; //tmp message-buffer for debugging
    int         bytes;

    while (true)
    {
      {
        std::stringstream ss;
        ss << "IP: " << ip_addr_ << " port: " << port_;
        Utils::Logger::instance_.info(ss.str());
        Utils::Logger::instance_.debug("Wait for client...");
      }

      // has to be done in separate thread, cause accept blocks
      if ((client_socket = accept(socket_, (sockaddr *)&client, &client_size)) == -1)
        throw std::runtime_error("Can't connect to client");
      Utils::Logger::instance_.debug("Client accepted");

      while (true)
      {
        std::memset(buf, 0, sizeof(buf));
        Utils::Logger::instance_.info("Waiting for message...");
        switch (bytes = recv(client_socket, buf, 4096, 0)) //?
        {
          case -1:
            throw std::runtime_error("Can't recieve message from client");
          case 0: // normal error; needs to be handled later with threads and sessions
          {
            Utils::Logger::instance_.info("recv() has returned 0");
            break;
          }
          default:
          {
            Utils::Logger::instance_.info("Recieved: " + std::string(buf, 0, bytes));
            break;
          }
        }

        if (send(client_socket, answer, sizeof(answer), 0) == -1) //tmp send() for debugging
          throw std::runtime_error("Can't send() message to client [debugging func call]");
      }

      if (close(client_socket) == -1)
        throw std::runtime_error("Can't close the client_socket");
    
    } // !while (true)
    
    if (close(socket_) == -1)
      throw std::runtime_error("Can't close the socket_"); // need to be moved somewhere else
  }
}} //!namespace WS::Core

WS::Core::Server* WS::Core::Server::instance_;