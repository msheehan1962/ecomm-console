//*****************************************************************
//  Name: Console.cpp
//
//  Description: The Console object implements the constructors
//               and destructors as well as the getLine, putLine,
//               and printPrompt member functions
//
//  Purpose: The Console object can be constructed for the I/O
//           configurations below:
//             a) stdin and stdout (default)
//             b) file in and stdout
//             c) file in and file out
//
//
//  Author:  Mark Sheehan (Feb. 27, 2020)
//******************************************************************


#include "Console.h"
#include <iostream>


// default constructor console/terminal IO
Console::Console() : inStream{ std::cin }, outStream{ std::cout } { }

// constructor for file input
Console::Console(const std::string& infile) :
	_inFile{ infile, std::ios_base::in },    // open input file
	inStream{ _inFile }, outStream{ std::cout }
{
  _isInFile = true;
}

// constructor for file input and file output
Console::Console(const std::string& infile, const std::string& outfile) :
  _inFile{ infile, std::ios_base::in },    // open input file
  _outFile{ outfile, std::ios_base::out }, // open output file
  inStream{_inFile}, outStream{_outFile}
{
  _isInFile = true; 
  _isOutFile = true;
}

// destructor: close any open files
Console::~Console()
{
  if (_isInFile)
    _inFile.close();
  if (_isOutFile)
    _outFile.close();
}

//*******************************************************************
//  Name: getLine()
//
//  Purpose: read input from either the console, inStream(cin) or
//           from a file inStream(_inFile) depending upon the object
//           configuration.
//
//  Parameters: none
//
//  Returns: std:string object containing the input line
//
//*******************************************************************
std::string Console::getLine()
{
  std::string temp;    
	
  // if reading from a file, set end-of-file as "done" input.
  if (_isInFile) {
    getline(_inFile, temp);
    if (inStream.eof())
      temp = "done";
  }
  else
    getline(std::cin, temp);
  
  return temp;
}

//*******************************************************************
//  Name: putLine()
//
//  Purpose: redirects the output string (ostr) to the output stream.
//           The outStream is configured either to stdout or a file.
//
//  Parameters: std::string output string
//
//  Returns: void
//
//*******************************************************************
void Console::putLine(const std::string& ostr) const
{
  outStream << ostr;
}

//*******************************************************************
//  Name: printPrompt()
//
//  Purpose: sends the prompt to sdtout or a file
//
//  Parameters: none
//
//  Returns: void
//
//*******************************************************************

// printPrompt: print the default prompt
void Console::printPrompt() const
{
	outStream << _prompt;
}
