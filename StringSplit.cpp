//********************************************************************
//  Name: stringSplit.cpp ( arg1, arg2)
//
//  Usage: arg1 - a string object 
//         arg2 - character to delimit the string example: ','
//
//********************************************************************

#include <vector>
#include <string>
#include <sstream>

std::vector<std::string> stringSplit(const std::string& s, char delimiter)
{
  std::vector<std::string> tokens;

  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter))
    {
      tokens.push_back(token);
    }
  return tokens;
}

