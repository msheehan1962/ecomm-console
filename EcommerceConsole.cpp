//****************************************************************
//  Name:  EcommerceConsole.cpp
//
//  Purpose:
//
//****************************************************************

#include "EcommerceConsole.h"
#include "Queue.h"
#include <thread>

extern EcommerceConsole console;

Queue<Message> ConsoleInputQueue;


EcommerceConsole::EcommerceConsole() : Console() {}

EcommerceConsole::EcommerceConsole(const std::string& infile) :
  Console(infile){}

EcommerceConsole::EcommerceConsole(const std::string& infile,
				   const std::string& outfile) :
  Console(infile, outfile) {}

EcommerceConsole::~EcommerceConsole()
{
  //std::cout << "~EcommerceConsole\n";
}

//********************************************************************
//  Name: menu()
//
//  Purpose: Monitor user input and take appropriate actions.
//
//  Parameters: none
//
//  Returns:  void
//
//********************************************************************
void EcommerceConsole::menu() {

  std::string userInput;
  Message userMessage;
  do {

    std::cout << "\nMenu: enter 'p' to print stats or \n";
    std::cout << "            'q' to quit.\n";
    std::cout << "Input: ";
      
    inStream >> userInput;

    //
    // Print the statistics by sending a message to the sales manager
    //

    if (userInput[0] == 'p' || userInput[0] == 'P') {
      userMessage.destination = (unsigned)Dest::SALESMGR;
      userMessage.source = (unsigned)Dest::USER;
      userMessage.type = (unsigned)Type::PRINT;
      userMessage.msgData = "";
      ConsoleInputQueue.enqueue(userMessage);
      // wait for SalesMgr to print before looping back and
      // printing out the menu.
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    //
    //  Quit command begins the Ecommerce Simulator shutdown by sending
    //  messages to the customer, inventory and sales managers.
    //
    //   1) shutdown customer manager
    //   2) shutdown sales manager
    //   3) shutdown inventory manager
    //
    if (userInput[0] == 'q' || userInput[0] == 'Q') {
      userMessage.destination = (unsigned)Dest::CUSTOMERMGR;
      userMessage.source = (unsigned)Dest::USER;
      userMessage.type = (unsigned)Type::QUIT;
      userMessage.msgData = "";
      ConsoleInputQueue.enqueue(userMessage);
      std::this_thread::sleep_for(std::chrono::milliseconds(100));

      userMessage.destination = (unsigned)Dest::SALESMGR;
      ConsoleInputQueue.enqueue(userMessage);
      std::this_thread::sleep_for(std::chrono::milliseconds(100));

      userMessage.destination = (unsigned)Dest::INVENTORYMGR;
      ConsoleInputQueue.enqueue(userMessage);
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      break;
    }      
  
  } while (1);  
}

