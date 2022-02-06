#include <iostream>
#include "./readConfig.cpp"

int main(int argc, char * argv[])
{
    ReadConfig::readfile(argv[1]);
    return 0;
}