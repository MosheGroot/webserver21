#include "../../headers/cgi/cgi.hpp"
#include "../../headers/http/http.hpp"
#include "../../headers/utils/string.hpp"


namespace WS { namespace Cgi
{

  void           Cgi::initEnv(Config::ServerConfig &serv, Http::Request &request)
  {
    std::map<std::string, std::string> headers = request.headers;

    env_["AUTH_TYPE"] = "Basic";
    env_["CONTENT_LENGTH"] = Utils::String::to_string(request.body.length());
    env_["CONTENT_TYPE"] = headers["Content-Type"];
    env_["GATEWAY_INTERFACE"] = "CGI/1.1";
    env_["PATH_INFO"] = "./resources/cgi-script/info.php";
    env_["PATH_TRANSLATED"] = "";
    env_["QUERY_STRING"] = "";
    env_["REMOTE_ADDR"] = serv.ip_addr;
    env_["REMOTE_HOST"] = headers["Host"];
    env_["REMOTE_PORT"] = Utils::String::to_string(serv.port);
    env_["REQUEST_URI"] = request.uri;
    env_["REQUEST_METHOD"] = request.method; // to string
    env_["SCRIPT_NAME"] = request.uri;
    env_["SCRIPT_FILENAME"] = serv.root + "/" + request.uri;
    env_["SERVER_NAME"] = headers["Host"];
    env_["SERVER_PORT"] = Utils::String::to_string(serv.port);
    env_["SERVER_PROTOCOL"] = "HTTP/1.1";
    env_["SERVER_SOFTWARE"] = "webserv/1.0";
  }

  char           **Cgi::envChar()
  {
    char  **env = new char*[env_.size() + 1];
    int j = 0;

    for (std::map<std::string, std::string>::iterator it = env_.begin(); it != env_.end(); it++)
    {
      std::string data = it->first + "=" + it->second;
      env[j] = new char[data.size() + 1];
      env[j] = strcpy(env[j], data.c_str());
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
