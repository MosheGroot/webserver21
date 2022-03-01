#include "../../headers/utils/exceptions.hpp"

namespace WS { namespace Utils {

    const char*     Exceptions::FileDoesNotExist::what() const throw()
    {
        return "Exception thrown: file doesn't exist";
    }

}} //!namespace WS::Utils