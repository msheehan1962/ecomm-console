//*************************************************************************
//  Name: Message.h
//
//  Description:  The Message is a data structure providing a
//                communication channel between objects. 
//
//*************************************************************************

#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>
#include <sstream>

// Destination options for the message ueue
enum class Dest { USER, CUSTOMER, CUSTOMERMGR, SALESMGR,
		  INVENTORYMGR, BROADCAST, NULLMSG };

// Messages sent on the message queue
enum class Type { LOWINVENTORY, SALE, PRINT, QUIT, UNAVAILABLE };

class Message {
public:
  Message() {}              // constructor
  // Message(enum class Dest) : destination=Dest {}              // constructor
  ~Message() {}             // destructor
  
    
  unsigned destination;     // message to
  unsigned source;          // message from
  unsigned type;            // purpose of message
  std::string msgData;      // contents of the message
  
  //friend std::ostream& operator<< (std::ostream& os, const Message& pt);    
};

/*
std::ostream& operator<< (std::ostream& os, const Message& msg)
{
  os << "dest: " << msg.destination << "  source: " << msg.source 
     << "  type: " << msg.type << "data: " << msg.msgData;
  return os;
}
*/
#endif
