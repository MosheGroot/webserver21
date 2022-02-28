#include "../../headers/cgi/cgi.hpp"
#include "../../headers/http/http.hpp"
#include "../../headers/utils/string.hpp"


namespace WS { namespace Cgi
{
  // Cgi::Cgi()
  // {
  //    this->initEnv_(serv, serv_location, request);
  // }

  void            Cgi::initEnv_(Config::ServerConfig &serv, Config::ServerLocation &serv_location,
                                Http::Request &request)
  {
    std::map<std::string, std::string> headers = request.headers;
    env_["AUTH_TYPE"] = "Basic";
    env_["CONTENT_LENGTH"] = Utils::String::to_string(request.body.length());
    env_["CONTENT_TYPE"] = headers["Content-Type"];
    env_["GATEWAY_INTERFACE"] = "CGI/1.1";
    env_["PATH_INFO"] = "../../resources/info.php";
    env_["PATH_TRANSLATED"] = "";
    env_["QUERY_STRING"] = "";
    env_["REMOTE_ADDR"] = ""; //
    env_["REMOTE_IDENT"] = ""; //
    env_["REQUEST_URI"] = request.uri;
    env_["REQUEST_METHOD"] = request.method; // to string
    env_["SCRIPT_FILENAME"] = "";
    env_["SCRIPT_NAME"] = "";
    env_["SERVER_NAME"] = ""; // serv.server_name[0]; // ??
    env_["SERVER_PORT"] = Utils::String::to_string(serv.port);
    env_["SERVER_PROTOCOL"] = "HTTP/1.1"; // request.version; 
    env_["SERVER_SOFTWARE"] = "webserv/1.0";
    env_["REQUEST_STATUS"] = "200";
  }

  char          **Cgi::envChar()
  {
    char  **env = new char*[env_.size() + 1];
    int j = 0;

    for (std::map<std::string, std::string>::iterator it = env_.begin(); it != env_.end(); it++)
    {
      env[j] = strdup((it->first + "=" + it->second).c_str()); // catch (std::bad_alloc& error)
      int k = 0;
      while (env[j][k])
      {
        if (env[j][k] == '-')
          env[j][k] = '_';
        k++;
      }
    }
    env[j] = NULL;
    return env;
  }

  std::string     Cgi::executeCgi(const std::string& scriptFile, Config::ServerConfig &serv, 
                                  Config::ServerLocation &serv_location, Http::Request &request)
  {
    pid_t         pid;
    std::string   result_body;
    int           input[2];
    int           output[2];
    char          **env;

    initEnv_(serv, serv_location, request);
    env = envChar();
    pid = fork();

    if (pid == -1)
      std::cout << "FORK ERROR" << std::endl;
    else if (!pid)
    {
      execve(serv_location.path.c_str(), NULL, env);
    }

    return result_body;
  }

  // const char        *Cgi::ErrorMemoryException::what() const throw()
  // {
  //   return "Exception thrown: wrong config file";
  // }

}} //!namespace WS::Cgi
