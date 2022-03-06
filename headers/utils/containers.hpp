#pragma once

#include <algorithm>

namespace WS { namespace Utils {

  /* @brief Static class of utils for 
            various containers with std::algorithm usage
  */
  class Containers
  {
  public:
    /* @brief Function for check if Container contains specified value.
              Requires ::value_type trait and begin()/end() iterators.
        
      @return True if container contains such value, flase otherwise.
    */
    template<class Container>
    static bool   contains(Container container, const typename Container::value_type& value);

  };

  template<class Container>
  bool    Containers::contains(Container container, const typename Container::value_type& value)
  {
    return std::find(container.begin(), container.end(), value) != container.end();
  }
  
}} //!namespace WS::Utils