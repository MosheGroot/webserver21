#include "../../headers/core/server.hpp"
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
#include <errno.h>

#include <stdlib.h> // remove // atoi

namespace WS { namespace Core
{
  
  Server  Server::instance_;

  void  Server::init(const char* config_path)
  {
    // Config
    WS::Config::Parser::parseFile(config_path, conf_);
    WS::Utils::Debug::printConf(conf_); // < DEBUG

    // Sockets
    FD_ZERO(&master_set_);

    listening_socket_.reserve(conf_.server_list.size());
    for (size_t i = 0; i < conf_.server_list.size(); i++)
    {
      // create socket
      listening_socket_.push_back(socket(AF_INET, SOCK_STREAM, 0));
      if ((listening_socket_[i]) == -1)
        throw std::runtime_error("Can't create a listening socket(): " + std::string(strerror(errno)));

      // set options
      int enable = 1;
      if (setsockopt(listening_socket_[i], SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1)
        throw std::runtime_error("Can't setsockopt() at listening socket: " + std::string(strerror(errno)));

      // bind socket to ip:port and listen
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

      // set up FD
      FD_SET(listening_socket_[i], &master_set_);

      // fill socket info to the map
      socket_info_.insert(std::make_pair(listening_socket_[i], &conf_.server_list[i]));

      // socket_info_[listening_socket_[i]] = conf_.server_list[i];
      // SocketInfo(
      //   conf_.server_list[i].ip_addr,
      //   conf_.server_list[i].port,
      //   std::atoi(conf_.server_list[i].buff_size_body.c_str()));
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
            socket_info_[new_client_socket] = socket_info_[socket];

            FD_SET(new_client_socket, &master_set_);
          }
          else
          {
            handleConnection(socket, writefds);
          }
        }
      }

      //usleep(100); // because too hot... // remove!
    
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
    if (recvMsg(client_socket) != CLIENT_DISCONNECTED && FD_ISSET(client_socket, &writefds)) //?z
    {
      // sendMsg(client_socket, "Message has been recieved!\n", sizeof("Message has been recieved!\n"));
    }
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

  void  Server::handleMsg(std::string msg, int socket_recv_from) const
  {
    Utils::Logger::debug("#" + Utils::String::to_string(socket_recv_from) + "Recieved: " + msg);

    std::string response = RequestHandler::handle(
      msg,
      socket_info_.at(socket_recv_from)->ip_addr,
      socket_info_.at(socket_recv_from)->port,
      this->conf_);

    Utils::Logger::instance_.debug("SENDING RESPONSE");
    Utils::Logger::instance_.debug(response);

    sendMsg(socket_recv_from, response.c_str(), response.size());

    Utils::Logger::instance_.debug("RESPONSE HAS BEEN SENDED");
  }

  int  Server::recvMsg(int socket_recv_from)
  {
    std::string       request;
    char              buffer[50]; // max_body_size + header
    int               ret_bytes;
    size_t            max_body_size = atoi(socket_info_[socket_recv_from]->buff_size_body.c_str());
    size_t            headers_end = std::string::npos;

    max_body_size = 10;

    std::memset(buffer, 0, sizeof(buffer));
    while ((ret_bytes = recv(socket_recv_from, buffer, sizeof(buffer) - 1, MSG_DONTWAIT)) > 0)
    {
      Utils::Logger::info("GOTCHA " + Utils::String::to_string(ret_bytes));
      // recieve
      request += buffer;
      std::memset(buffer, 0, sizeof(buffer));

      // check max body size
      if (headers_end == std::string::npos)
      {
        //Utils::Logger::error("1. Request size: " + Utils::String::to_string(request.size()) + "\n\theaders_end: " + Utils::String::to_string(headers_end));
        //Utils::Logger::error("[" + request + "]");
        headers_end = request.find("\r\n\r\n", request.size() - sizeof(buffer) + 1);
      }
      if (headers_end != std::string::npos)
      {
        //Utils::Logger::error("2. Request size: " + Utils::String::to_string(request.size()) + "\n\theaders_end: " + Utils::String::to_string(headers_end));
        if (request.size() - headers_end - 4 > max_body_size)
          throw std::runtime_error("Reached max body size"); // not exception, but send error to client
      }
    }

    switch (ret_bytes)
    {
      case 0:
      {
        handleDisconnection(socket_recv_from);
        return CLIENT_DISCONNECTED;
      }
      default:
      {
        handleMsg(request, socket_recv_from); // if any errors -> disconnetct
        return 0;
      }
    }
  }

  void  Server::sendMsg(int socket_to_send, const char* msg, int msg_size) const
  {
    int ret;
    if ((ret = send(socket_to_send, msg, msg_size, 0)) == -1)
      throw std::runtime_error("Can't send() message to the client: " + std::string(strerror(errno))); // It's a regular error, not an exception
    std::cout << "{{{{{{" << ret << " | " << msg_size << "}}}}}}}}}" << std::endl;
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