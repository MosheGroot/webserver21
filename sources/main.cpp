#include "../headers/utils/logger.hpp"
#include "../headers/http/http.hpp"

int main()
{
    Utils::Logger::instance_.info("Hello, World!");
    return 0;
}
