#include "../../headers/core/core.hpp"

namespace WS::Core
{
    /// Singleton part
    WebServer&   WebServer::getInstance(void)
    {
      if (instance_ == nullptr)
        instance_ = new WebServer();
      return *instance_;
    }

    /// Logic part

} //!namespace WS::Core