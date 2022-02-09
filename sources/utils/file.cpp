#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>
#include <errno.h>
#include <string.h>
#include <stdexcept>

#include "../../headers/utils/file.hpp"

namespace WS { namespace Utils
{

  std::string File::readFile(const char *filename)
  {
    // TODO
    (void)filename; // delete after TODO (unused parameter warning)

    return "";
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

}} //!namespace WS::Utils
