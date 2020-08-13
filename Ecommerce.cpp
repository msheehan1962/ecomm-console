//*******************************************************************
//  Name: Ecommerce.cpp
//
//  Description:  This file is responsible for initializing the program
//                inventory file structure and instantiating the threads
//                for managing the inventory (InventoryMgr), customers 
//                (CustomerMgr) and sales (SalesMgr) threads.
//
//  Interface:  main()
//
//********************************************************************

#include <iostream>
#include <thread>
#include <mutex>
#include "ProductT.h"
#include "InventoryMgr.h"
#include "CustomerMgr.h"
#include "SalesMgr.h"
#include "ListVector.h"
#include "EcommerceConsole.h"

using namespace std;

std::mutex m_Mutex[PRODUCT_CT];
std::mutex m_Qutex;

void InitInventoryFiles(InventoryMgr&);  // function prototype

EcommerceConsole console;

int main()
{
  std::thread::id master_thread;

  // instantiate the InventoryMgr object
  InventoryMgr invMgr;

  // initialize the inventory file structure
  InitInventoryFiles(invMgr);

  // start the InventoryMgr thread
  thread invThread(invMgr);
  invThread.detach();

  // prepare to start the SalesMgr thread
  SalesMgr salesMgr;
  thread salesThread(salesMgr);
  salesThread.detach();

  // prepare to start the CustomerMgr thread
  CustomerMgr  customerMgr(invMgr.GetInventoryDataPtr(),
			   invMgr.GetInvMsgQueuePtr(),
			   salesMgr.GetMsgQueuePtr());

  thread customerThread(customerMgr);
  customerThread.detach();

  // wait for and dispatch commands 
  console.menu();
  
  return 0;
}

//**************************************************************************
//
//  Name: InitInventoryFiles()
//
//  Purpose: Set up the InventoryFile data structure from which the
//           InventoryManager initializes the inventory data base.
//
//  Parameters:  InventoryMgr reference. Need this to get the inventory
//               file pointer.
//
//  Return:  void
//
//**************************************************************************
void InitInventoryFiles(InventoryMgr& invMgr) {

  vector<InventoryMgr::InventoryFile>* invFilePtr = invMgr.GetInventoryFilePtr();

  InventoryMgr::InventoryFile fileStruct;
    
  fileStruct.prodId = "jacket";
  fileStruct.invFile = "JacketFile.txt";
  invFilePtr->push_back(fileStruct);
  fileStruct.prodId = "pants";
  fileStruct.invFile = "PantsFile.txt";
  invFilePtr->push_back(fileStruct);
  fileStruct.prodId = "shorts";
  fileStruct.invFile = "ShortsFile.txt";
  invFilePtr->push_back(fileStruct);
  fileStruct.prodId = "shirt";
  fileStruct.invFile = "ShirtFile.txt";
  invFilePtr->push_back(fileStruct);
  fileStruct.prodId = "socks";
  fileStruct.invFile = "SocksFile.txt";
  invFilePtr->push_back(fileStruct);
}
