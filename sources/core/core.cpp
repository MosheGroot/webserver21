#include "../../headers/core/core.hpp"

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

namespace WS::Core
{
    /// Singleton part
    Server&	Server::getInstance(void)
    {
      if (instance_ == nullptr)
        instance_ = new Server();
      return *instance_;
    }

    /// Logic part

	Server::Server(std::string ip_addr, int port)
	  :  ip_addr_(ip_addr), port_(port) { }

	int		Server::init()
	{
	  // create listening socket
	  
	  if (socket_ = socket(AF_INET, SOCK_STREAM, 0) == -1)
	  {  
		std::cout << "Can't create listening socket!" << std::endl;
		return -1;
	  }

	  // bind socket
	  
	  sockaddr_in	hint;
	  std::memset(&hint, 0, sizeof(hint));
	  hint.sin_family = AF_INET;
	  hint.sin_port = htons(port_);
	  hint.sin_addr.s_addr = INADDR_ANY;
	  
	  if (bind(socket_, &hint, sizeof(hint)) == -1)
	  {
		std::cout << "Can't bind listening socket!" << std::endl;
		return -2;
	  }

	  // listen socket

	  if (listen(socket_, SOMAXCONN) == -1)
	  {
		std::cout << "Can't listen socket!" << std::endl;
		return -3;
	  }
	}

	int		Server::run()
	{
	  // loop
	}

} //!namespace WS::Core