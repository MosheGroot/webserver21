#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <exception>

using namespace std;

class ReadConfig
{
    // private:

    public:
      ReadConfig(void) {}
      virtual ~ReadConfig(void) {}

      static void   readfile(const char *filename) {
        ifstream conffile(filename);
        std::string data;

        if (!conffile.is_open())
          throw FileNotFoundException();
      
        while(!conffile.eof())
        {
          getline(conffile, data);
          std::cout << data << std::endl;
        }
        conffile.close();
      }

      // static std::vector<std::string>   split(const char *line) {
        
      // }

      class	FileNotFoundException: public std::exception {
        virtual const char	*what() const throw() {
          return "Exception thrown: could not open configuration file";
        }
      };
};