#include "../../headers/cgi/cgi.hpp"
#include "../../headers/http/http.hpp"
#include "../../headers/utils/string.hpp"


namespace WS { namespace Cgi
{
  Cgi::Cgi()
  {
  }

  Cgi::~Cgi()
  {
  }

  void           Cgi::initEnv(Config::ServerConfig &serv, Config::ServerLocation &serv_location,
                              Http::Request &request)
  {
    serv_location.index = serv_location.index;
    serv.ip_addr = serv.ip_addr;
    // std::cout << request.body.length() << std::endl;
    std::map<std::string, std::string> headers = request.headers;
    env_["AUTH_TYPE"] = "Basic";
    env_["CONTENT_LENGTH"] = Utils::String::to_string(request.body.length());
    env_["CONTENT_TYPE"] = headers["Content-Type"];
    env_["GATEWAY_INTERFACE"] = "CGI/1.1";
    env_["PATH_INFO"] = "./resources/cgi-script/info.php";
    env_["PATH_TRANSLATED"] = "";
    env_["QUERY_STRING"] = "";
    env_["REMOTE_ADDR"] = ""; //
    env_["REMOTE_IDENT"] = ""; //
    env_["REQUEST_URI"] = request.uri;
    env_["REQUEST_METHOD"] = request.method; // to string
    env_["SCRIPT_FILENAME"] = "/home/uliana/labs/school_21/webserver21/resources/cgi-script/first.py";
    env_["SCRIPT_NAME"] = "";
    env_["SERVER_NAME"] = ""; // serv.server_name[?]; // ??
    env_["SERVER_PORT"] = serv.port;
    env_["SERVER_PROTOCOL"] = "HTTP/1.1";
    env_["SERVER_SOFTWARE"] = "webserv/1.0";
    env_["REQUEST_STATUS"] = "200"; // ??
  }

  char           **Cgi::envChar()
  {
    char  **env = new char*[env_.size() + 1];
    int j = 0;

    for (std::map<std::string, std::string>::iterator it = env_.begin(); it != env_.end(); it++)
    {
      std::string data = it->first + "=" + it->second;
      env[j] = new char[data.size() + 1];
      env[j] = strcpy(env[j], data.c_str()); // catch (std::bad_alloc& error)
      int k = 0;

      std::cout << env[j] << std::endl;
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

  std::string     Cgi::executeCgi(const std::string& scriptFile)
  {
    pid_t         pid;
    std::string   result_body;
    char          **env;
    
    FILE  *input = tmpfile();
    FILE  *output = tmpfile();
    int input_fd = fileno(input);
    int output_fd = fileno(output);

    env = envChar();
    pid = fork();
    if (pid == -1)
      throw ErrorMemoryException();
    else if (pid == 0)
    {
      char * const * nll = NULL;
      dup2(input_fd, STDIN_FILENO);
      dup2(output_fd, STDOUT_FILENO);
      execve(scriptFile.c_str(), nll, env);
    }
    else
    {
      char buffer[1024] = {0};
      waitpid(-1, NULL, 0);
      lseek(output_fd, 0, SEEK_SET);
      int ret = 1;
      while (ret > 0)
      {
        memset(buffer, 0 , 1024);
        ret = read(output_fd, buffer, 1023);
        result_body += buffer;
      }
    }
    fclose(input);
    fclose(output);
    close(input_fd);
    close(output_fd);

    for (size_t i = 0; env[i]; i++)
      delete[] env[i];
    delete[] env;

    return result_body;
  }

  const char      *Cgi::ErrorMemoryException::what() const throw()
  {
    return "Exception thrown: fork error";
  }

}} //!namespace WS::Cgi
