//
//
//
//
//
//
#ifndef ECOMMERCECONSOLE_H
#define ECOMMERCECONSOLE_H


#include "Console.h"

class EcommerceConsole : public Console {

 public:
  // constructor: input from std::cin and output to std::cout
  EcommerceConsole();
  // constructor: input from file and output to std::cout
  EcommerceConsole(const std::string&);
  // constructor: input from file and output to file
  EcommerceConsole(const std::string&, const std::string&);
  // destrcutor:
  ~EcommerceConsole();
  
  void menu();
};

#endif
