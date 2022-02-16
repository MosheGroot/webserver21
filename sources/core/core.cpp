#include "../../headers/core/core.hpp"
#include "../../headers/utils/logger.hpp"
#include "../../headers/utils/debug.hpp"

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <unistd.h> // only for close() [very fckng inconvenient]

#include <stdlib.h> // remove // atoi

namespace WS { namespace Core
{
  
  Server  Server::instance_;
  Server&  Server::getInstance() { return instance_; }

  void  Server::init(const char* config_path)
  {
    std::stringstream ss;

    WS::Config::Parser::parsFile(config_path, const_cast<Config::Config&>(conf_));
    WS::Utils::Debug::printConf(conf_); // < DEBUG

    FD_ZERO(&master_set_);

    listening_socket_.reserve(conf_.server_list.size());
    for (size_t i = 0; i < conf_.server_list.size(); i++)
    {
      listening_socket_.push_back(socket(AF_INET, SOCK_STREAM, 0));
      if ((listening_socket_[i]) == -1)
        throw std::runtime_error("Can't create a listening socket(): " + std::string(strerror(errno)));

      int enable = 1;
      if (setsockopt(listening_socket_[i], SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1)
        throw std::runtime_error("Can't setsockopt() at listening socket: " + std::string(strerror(errno)));

      sockaddr_in hint;
      std::memset(&hint, 0, sizeof(hint));
      hint.sin_family = AF_INET;
      uint16_t tmp_port = atoi(conf_.server_list[i].port.c_str()); //remove
      hint.sin_port = htons(tmp_port);
      if ((hint.sin_addr.s_addr = inet_addr(conf_.server_list[i].ip_addr.c_str())) == INADDR_NONE)
        throw std::runtime_error("Non valid ip_addr: " + std::string(strerror(errno)));
      if (bind(listening_socket_[i], (sockaddr *)&hint, sizeof(hint)) == -1)
      {
        throw std::runtime_error("Can't bind() the socket: " + std::string(strerror(errno)));
      }
      if (listen(listening_socket_[i], SOMAXCONN) == -1)
        throw std::runtime_error("Can't listen() the socket: " + std::string(strerror(errno)));

      FD_SET(listening_socket_[i], &master_set_);
    }
  }

  int Server::run()
  {
    std::stringstream ss;
    fd_set            readfds;
    fd_set            writefds;
    int               new_client_socket;

    // for (size_t i = 0; i < conf_.server_list.size(); i++) // remade
    //   ss << "ip: " << conf_.server_list[i].ip_addr << " port: " << conf_.server_list[i].port << std::endl;
    // Utils::Logger::instance_.debug(ss.str());
    
    while (true)
    {
      readfds = master_set_;
      writefds = master_set_;
      if (select(FD_SETSIZE, &readfds, &writefds, NULL, NULL) == -1)
        throw std::runtime_error("Can't select() fdsets: " + std::string(strerror(errno)));

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

      usleep(100); // because too hot... // remove!
    
    }
    
    closeListeningSockets();

  }

  bool  Server::isListening(int socket) const
  {
    for (size_t i = 0; i < conf_.server_list.size(); i++)
    {
      if (socket == listening_socket_[i])
        return true;
    }
    return false;
  }

  int  Server::acceptConnection(int listening_socket) const
  {
    std::stringstream ss;
    std::stringstream ss1;
    sockaddr_in       client;
    socklen_t         client_size = sizeof(client);
    int               new_client_socket;

    if ((new_client_socket = accept(listening_socket, (sockaddr *)&client, &client_size)) == -1)
      throw std::runtime_error("Can't accept() the client: " + std::string(strerror(errno)));
    ss << "Client #" << new_client_socket << " has been accepted" << std::endl;
    Utils::Logger::instance_.info(ss.str());

    ss1 << "Client #" << new_client_socket << ": " << inet_ntoa(client.sin_addr) << ":" << htons(client.sin_port); // remove!
    Utils::Logger::instance_.debug(ss1.str());

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
          throw std::runtime_error("Can't close() the client's connection: " + std::string(strerror(errno)));
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
        throw std::runtime_error("Can't recieve() message from the client: " + std::string(strerror(errno))); // Mb it's a regular error, not an exception
      case 0:
      {
        handleDisconnection(socket_recv_from);
        return CLIENT_DISCONNECTED;
      }
      default:
      {
        handleMsg(std::string(buf, 0, bytes), socket_recv_from); // if any errors -> disconnetct
        return 0;
      }
    }
  }

  void  Server::sendMsg(int socket_to_send, const char* msg, int msg_size) const
  {
    if (send(socket_to_send, msg, msg_size, 0) == -1)
      throw std::runtime_error("Can't send() message to the client: " + std::string(strerror(errno))); // It's a regular error, not an exception
  }

  void  Server::closeListeningSockets() const
  {
    for (size_t i = 0; i < conf_.server_list.size() ; i++)
    {
      if (close(listening_socket_[i]) == -1)
        throw std::runtime_error("Can't close a listening socket: " + std::string(strerror(errno)));
    }
    
  }

}} //!namespace WS::Core