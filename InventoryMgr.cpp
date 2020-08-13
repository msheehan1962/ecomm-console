/*
 *  Name: InventroyMgr.cpp
 *
 *  Description:	The InventoryMgr object is designed to initialize the
 *			inventory data, monitor inventory, and replenish the 
 *  			available inventory when the Customer sends a "Low
 *  			Inventory" message.
 *
 *  Author:		Mark Sheehan
 *
 */

#include <iostream>
#include <fstream>
#include <thread>
#include "ListVector.h"
#include "ProductT.h"
#include "InventoryMgr.h"

// parser for reading product info from a string (from a file)
extern std::vector<std::string> stringSplit(const std::string& s,
					    char delimiter);

// Message queue from the user input console
extern Queue<Message> ConsoleInputQueue;

//********************************************************************
//  Name constructor()
//
//  Purpose: Initialize the inventory file pointer, the inventory
//           data pointer and the inventory message queue pointer.
//
//  Parameters: none
//
//  Return: void
//
//*******************************************************************
InventoryMgr::InventoryMgr() 
{
  // initialize the inventory file pointer (initial data is read from text files)
  if (inventoryFilePtr == NULL)
    inventoryFilePtr = new std::vector<InventoryFile>;

  // ListVector is a singleton object. Instantiate the ListVector or data base
  ListVector<ProductT>* inventoryDataPtr = ListVector<ProductT>::get_instance(PRODUCT_CT);

  // Initialize the inventory message queue pointer (for Inventory low messages)
  if (invMsgQueuePtr == NULL)
    invMsgQueuePtr = new Queue<Message>;
}


//********************************************************************
//  Name copy constructor()
//
//  Purpose: Copy the inventory file pointer, the inventory
//           data pointer and the inventory message queue pointer
//           from the original object to the new one..
//
//  Parameters: InventoryMgr& copySource 
//
//  Return: void
//
//*******************************************************************
InventoryMgr::InventoryMgr(const InventoryMgr& copySource)
{
  inventoryFilePtr = copySource.inventoryFilePtr;
  inventoryDataPtr = copySource.inventoryDataPtr;
  invMsgQueuePtr = copySource.invMsgQueuePtr;
}


//********************************************************************
//  Name destructor()
//
//  Purpose:  clean up resources
//
//  Parameters: none 
//
//  Return: void
//
//*******************************************************************
InventoryMgr::~InventoryMgr()
{
  //std::cout << "~InventoryMgr\n";
  //delete inventoryFilePtr;
  //delete inventoryDataPtr;
  //delete invMsgQueuePtr;
}

//********************************************************************
//  Name operator() ()
//
//  Purpose:  Used by main to start the InventoryMgr thread.
//
//  Parameters: none 
//
//  Return: void
//
//*******************************************************************
// the inventory manager is implemented below
void InventoryMgr::operator () () //std::vector<InventoryFile> inFiles)
{
  //std::cout << "InvMgr Mgt functor" << std::endl;
  InitInventory();
  InventoryMonitor();
}

//********************************************************************
//  Name Getter Functions
//
//  Purpose:  return the inventoryFilePtr
//                       inventoryDataPtr
//                       invMsgQueuePtr
//
//  Parameters: none
//
//  Return: respective data types
//
//*******************************************************************
std::vector<InventoryMgr::InventoryFile>* InventoryMgr::GetInventoryFilePtr() const
{
  return inventoryFilePtr;
}

ListVector<ProductT>* InventoryMgr::GetInventoryDataPtr() const
{
  return inventoryDataPtr;
}

Queue<Message>* InventoryMgr::GetInvMsgQueuePtr() const
{
  return invMsgQueuePtr;
}

//********************************************************************
//  Name:  InitInventory()
//
//  Purpose:  Iterate through each inventory data file, reading each
//            line. Instantiate a product from the input and push
//            the product onto the inventory data base.
//
//  Parameters: none 
//
//  Return: void
//
//*******************************************************************
void InventoryMgr::InitInventory() 
{
  std::ifstream inventoryFile;      // file containing the inventory file names.

                                    // inventory file pointer (now empty)
  std::vector<InventoryFile>* invFilePtr = GetInventoryFilePtr();
  
  InventoryFile invFileData;        // contains the prod-type and file-name

  // iterate through each file pointed to by the inventoryFilePtr
  for (int index = 0; index < (int)invFilePtr->size(); index++)
    {
      invFileData  = invFilePtr->at(index);

      // Open the file containing the initial inventory
      if (invFileData.invFile != " ")
	inventoryFile.open(invFileData.invFile, std::ios_base::in);

      // read each line into "fileContents"
      std::string fileContents;
      if (inventoryFile.is_open())
	{
	  while (inventoryFile.good())
	    {
	      getline(inventoryFile, fileContents);

	      if ((fileContents == "END") || (fileContents == "END\r"))
		break;

	      // break each line into tokens delimited by ',' 
	      std::vector<std::string> myTokens;		
	      myTokens = stringSplit(fileContents, ',');

	      std::string pCat = myTokens[0];
	      // instantiate local product (ProductT) 
	      ProductT lclProd(pCat, (PGENDER)stoi(myTokens[1]), 
			       (PSIZE)stoi(myTokens[2]),
			       (PCOLOR)stoi(myTokens[3]),
			       (double)stod(myTokens[4]));
	      
	      // push the product onto the list 
	      inventoryDataPtr->add_element(lclProd, index);
	    } // while
	  
	  inventoryFile.close(); // Finished reading, now close
	}
      else {
	std::cout << "Failed to open file in InitInventory()" << std::endl;
	return;
      }
    }
}

//********************************************************************
//  Name:  InventoryMonitor()
//
//  Purpose:  if there is an inventory-low message, create a
//            ProductT element and then add 10 of those products
//            onto the product data base. Also monitor the console
//            input queue for quit message.
//
//  Parameters: none 
//
//  Return: void
//
//*******************************************************************
void InventoryMgr::InventoryMonitor()
{
  long int invLowCt{0};      // count the inventory low messages
  
  while (1) {

    // check if we receive a message from a customer indicating
    // the product they searched for was not found.
    
    if (!invMsgQueuePtr->empty()) {

      invLowCt++;
      Message invMsg;
      // pop the message from the queue
      invMsg = invMsgQueuePtr->dequeue();   
      
      // break the message string into tokens[] delimited by ','
      
      std::vector<std::string> myTokens;		
      myTokens = stringSplit(invMsg.msgData, ',');
      std::string pCat = myTokens[0];     // get the product category
      
      int whichList{0};
      if (pCat == "jacket") whichList = (int)PLIST::JACKET;
      if (pCat == "pants") whichList = (int)PLIST::PANTS;
      if (pCat == "shorts") whichList = (int)PLIST::SHORTS;
      if (pCat == "shirt") whichList = (int)PLIST::SHIRT;
      if (pCat == "socks") whichList = (int)PLIST::SOCKS;
      
      // instantiate local product and push onto list as a way
      // to replenish the stock. push data 10 times.
      ProductT lclProd(pCat, (PGENDER)stoi(myTokens[1]), 
		       (PSIZE)stoi(myTokens[2]),
		       (PCOLOR)stoi(myTokens[3]),
		       (double)stod(myTokens[4]));

      // add 10 elements to the inventory data list
      // invDataPtr gives access to the product data base
      ListVector<ProductT>* invDataPtr = GetInventoryDataPtr();
      
      for (int index{0}; index < 10; index++)
	  invDataPtr->add_element(lclProd, whichList);
      
    } // if (!invMsgQueuePtr->empty())

    Message userMsg;

    // check the console queue for any incomming messages
    
    if (!ConsoleInputQueue.empty()) {
      userMsg = ConsoleInputQueue.peek();
      if (userMsg.destination == (unsigned)Dest::INVENTORYMGR) {
	userMsg = ConsoleInputQueue.dequeue();
	if (userMsg.type == (unsigned)Type::QUIT) {
	  std::cout << "inventory-low count: " << invLowCt << std::endl; 
	  break;
	}
      }
    }

  } // while
}
