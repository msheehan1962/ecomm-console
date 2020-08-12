//
//  Name:  Console.h
//
//  Description:  This class accepts user input and output 
//                and seamlessly handles interactive or file
//                input and output.
//
#ifndef _CONSOLE_H
#define _CONSOLE_H
#include <iostream>
#include <string>
#include <fstream>

class Console {
public:
	// default constructor: use cin and cout
	Console();    
	// constructor: use file input and cout
	Console(const std::string&);
	// constructor: use file input and file output
	Console(const std::string&, const std::string&);
	// destructor
	~Console();  
	
	virtual std::string getLine();          // read input
	void putLine(const std::string&) const; // print output
	virtual void printPrompt() const;       // print user prompt

	std::istream& inStream;       // input stream
	std::ostream& outStream;      // output stream

protected:
	bool _isInFile{ false };      // flag indicating file input
	bool _isOutFile{ false };     // flag indicating file output
	std::fstream _inFile{ "" };   // input file name
	std::fstream _outFile{ "" };  // output file name
	std::string _prompt{ "Enter input: " };  // default prompt
};


#endif
