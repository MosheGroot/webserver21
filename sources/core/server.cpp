#include "../../headers/core/server.hpp"
#include "../../headers/utils/logger.hpp"
#include "../../headers/utils/debug.hpp"
#include "../../headers/utils/containers.hpp"

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
#include <algorithm>

#include <stdlib.h> // remove // atoi

namespace WS { namespace Core
{
  /// Singleton
  Server  Server::instance_;


  /// Init

  void  Server::init(const char* config_path)
  {
    this->initConfig(config_path);
    this->initConnectionsSet();
    this->initSockets();
  }


  void    Server::initConfig(const char* config_path)
  {
    WS::Config::Parser::parseFile(config_path, const_cast<Config::Config&>(this->conf_));
    WS::Utils::Debug::printConf(this->conf_); // < DEBUG
  }


  void    Server::initConnectionsSet(void)
  {
    for (size_t i = 0; i < this->conf_.server_list.size(); ++i)
    {
      this->connections_set_.insert(Server::ConnectionInfo(
        this->conf_.server_list[i].ip_addr,
        this->conf_.server_list[i].port)
      );
    }
  }


  void    Server::initSockets(void)
  {
    FD_ZERO(&master_set_);

    this->listening_sockets_.reserve(conf_.server_list.size());

    std::set<Server::ConnectionInfo>::const_iterator it;
    for (it = this->connections_set_.begin(); it != this->connections_set_.end(); ++it)
    {
      // create socket
      this->listening_sockets_.push_back(socket(AF_INET, SOCK_STREAM, 0));
      if ((this->listening_sockets_.back()) == -1)
        throw std::runtime_error("Can't create a listening socket(): " + std::string(strerror(errno)));


      // set options
      int enable = 1;
      if (setsockopt(
          this->listening_sockets_.back(),
          SOL_SOCKET,
          SO_REUSEADDR,
          &enable,
          sizeof(int)) == -1
        )
        throw std::runtime_error("Can't setsockopt() at listening socket: " + std::string(strerror(errno)));


      // bind socket to ip:port and listen
      sockaddr_in hint;
      std::memset(&hint, 0, sizeof(hint));

      hint.sin_family = AF_INET;

      uint16_t tmp_port = atoi(it->port.c_str()); //remove
      hint.sin_port = htons(tmp_port);

      if ((hint.sin_addr.s_addr = inet_addr(it->ip_addr.c_str())) == INADDR_NONE)
        throw std::runtime_error("Non valid ip_addr: " + std::string(strerror(errno)));

      if (bind(this->listening_sockets_.back(), (sockaddr *)&hint, sizeof(hint)) == -1)
      {
        throw std::runtime_error("Can't bind() the socket: " + std::string(strerror(errno)));
      }

      if (listen(this->listening_sockets_.back(), SOMAXCONN) == -1)
        throw std::runtime_error("Can't listen() the socket: " + std::string(strerror(errno)));


      // set up FD
      FD_SET(this->listening_sockets_.back(), &master_set_);


      // fill socket info to the map
      socket_infos_.insert(std::make_pair(
        this->listening_sockets_.back(),
        &(*it)
      ));
    }
  }



  /// Runtime

  int Server::run()
  {
    std::stringstream ss;
    fd_set            readfds;
    fd_set            writefds;
    int               new_client_socket;

    while (true)
    {
      /// copy masterset
      readfds = master_set_;
      writefds = master_set_;

      /// select
      if (select(FD_SETSIZE, &readfds, &writefds, NULL, NULL) == -1)
        throw std::runtime_error("Can't select() fdsets: " + std::string(strerror(errno)));

      /// check if any socket has changed
      for (int socket = 0; socket < FD_SETSIZE; socket++) // add tracking for max socket for replacing FD_SETSIZE
      {
        if (FD_ISSET(socket, &readfds))
        {
          if (isListening(socket))
          {
            // accept
            new_client_socket = acceptConnection(socket);

            // duplicate connection info
            socket_infos_[new_client_socket] = socket_infos_[socket];

            // add to set
            FD_SET(new_client_socket, &master_set_);
          }
          else
          {
            // handle clients connection
            handleConnection(socket, writefds);
          }
        }
      }

      //usleep(100); // because too hot... // remove!
    
    }
    
    /// clean up
    closeListeningSockets();

  }


  //  Connections

  bool  Server::isListening(int socket) const
  {
    return Utils::Containers::contains(this->listening_sockets_, socket);
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


  //  Recieve and send

  void  Server::handleMsg(std::string msg, int socket_recv_from) const
  {
    Utils::Logger::debug("#" + Utils::String::to_string(socket_recv_from) + "Recieved: " + msg); // < DEBUG

    // get connection info
    const Server::ConnectionInfo* info = socket_infos_.at(socket_recv_from);

    // make response
    std::string response = RequestHandler::handle(
      msg,
      info->ip_addr,
      info->port,
      this->conf_);

    // send response
    Utils::Logger::instance_.debug("SENDING RESPONSE");         // < DEBUG
    Utils::Logger::instance_.debug("{" + response + "}");       // < DEBUG

    sendMsg(socket_recv_from, response.c_str(), response.size());

    Utils::Logger::instance_.debug("RESPONSE HAS BEEN SENDED"); // < DEBUG
  }

  int  Server::recvMsg(int socket_recv_from)
  {
    std::string       request;
    char              buffer[50]; // max_body_size + header
    int               ret_bytes;

    // recieve
    std::memset(buffer, 0, sizeof(buffer));
    while ((ret_bytes = recv(socket_recv_from, buffer, sizeof(buffer) - 1, MSG_DONTWAIT)) > 0)
    {
      request += buffer;
      std::memset(buffer, 0, sizeof(buffer));
    }

    // handle
    switch (ret_bytes)
    {
      case 0:
      {
        handleDisconnection(socket_recv_from);
        return CLIENT_DISCONNECTED;
      }
      default:
      {
        handleMsg(request, socket_recv_from); // if any errors -> disconnect
        return 0;
      }
    }
  }

  void  Server::sendMsg(int socket_to_send, const char* msg, int msg_size) const
  {
    int ret;
    if ((ret = send(socket_to_send, msg, msg_size, 0)) == -1)
      throw std::runtime_error("Can't send() message to the client: " + std::string(strerror(errno))); // It's a regular error, not an exception
  }


  //  Closing

  void  Server::closeListeningSockets() const
  {
    for (size_t i = 0; i < conf_.server_list.size() ; i++)
    {
      if (close(listening_sockets_[i]) == -1)
        throw std::runtime_error("Can't close a listening socket: " + std::string(strerror(errno)));
    }
  }

}} //!namespace WS::Core