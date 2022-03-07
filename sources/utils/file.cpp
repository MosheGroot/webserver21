#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>
#include <errno.h>
#include <string.h>
#include <stdexcept>

#include <fstream>
#include <sstream>

#include "../../headers/utils/file.hpp"
#include "../../headers/utils/exceptions.hpp"
#include "../../headers/utils/logger.hpp"

namespace WS { namespace Utils
{

  std::string File::readFile(const char *filename)
  {
    std::string absolute_path;
    if (filename[0] != '/')
      absolute_path = File::getCurrentDir() + "/" + std::string(filename);
    else
      absolute_path = std::string(filename);


    Utils::Logger::debug(absolute_path);
    std::ifstream file(absolute_path.c_str());

    if (!file.is_open())
      throw Utils::Exceptions::FileDoesNotExist();

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string text = buffer.str();
    Utils::Logger::debug(text);
    return text;
  }

  void  File::writeFile(const std::string& data, const char *filepath, bool override)
  {
    std::ofstream fout(filepath, (override ? std::ios_base::trunc : std::ios_base::app));

    if (!fout.is_open())
      throw std::runtime_error("writeFile exception: can't open file " + std::string(filepath));

    fout << data << std::flush;

    fout.close();
  }


  void  File::createPath(const char* path, mode_t mode)
  {
    // make duplicate
    char *copypath = strdup(path);

    // create parent directories
    char *sbegin_ptr = copypath;
    char *send_ptr = NULL;

    while ((send_ptr = strchr(sbegin_ptr, '/')) != 0)
    {
      if (sbegin_ptr != send_ptr)
      {
        *send_ptr = '\0';
        File::createDir(copypath, mode);
        *send_ptr = '/';
      }
      sbegin_ptr = send_ptr + 1;
    }

    // create last dir
    File::createDir(copypath, mode);

    // clean up
    free(copypath);
  }


  void  File::createDir(const char* dir, mode_t mode)
  {
    struct stat   st;

    if (stat(dir, &st) != 0)
    {
      if (mkdir(dir, mode) != 0)
        throw std::runtime_error(strerror(errno));
    }
    else if (!S_ISDIR(st.st_mode))
    {
      throw std::runtime_error(strerror(ENOTDIR));
    }
  }


  bool   File::isDir(const char *path)
  {
    struct stat st;

    if (stat(path, &st) != 0 || S_ISREG(st.st_mode)) // doesnt exists or file
      return false;
    return S_ISDIR(st.st_mode) != 0;
  }

  bool   File::isFile(const char *path)
  {
    struct stat st;

    if (stat(path, &st) != 0) // doesnt exists
      return false;
    return S_ISREG(st.st_mode) != 0;
  }

  bool  File::pathExists(const char *path)
  {
    struct stat st;

    return (stat(path, &st) == 0);
  }

  std::string   File::getCurrentDir(void)
  {
    char pwd[1024];

    getcwd(pwd, 1024);
    return pwd;
  }
  
}} //!namespace WS::Utils
