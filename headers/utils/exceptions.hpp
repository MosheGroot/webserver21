#pragma once

#include <exception>

namespace WS { namespace Utils {

    /* @brief Class of various usefull exceptions
    */
    class Exceptions 
    {
    public:

        struct	FileDoesNotExist: public std::exception {
            virtual const char  *what() const throw();
        };

    };
}}