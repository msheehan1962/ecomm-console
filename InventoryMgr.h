/*
 * Name: InventoryMgr.h
 *
 * Description: The InventoryMgr object is responsible for initializing the
 *              inventory data structure. In addition, it will run as a single
 *              thread monitoring the "low inventory" message queue from the 
 *              customer threads. when inventory is low the InventoryMgr will 
 *              replenish the appriopriate stock/list.  
 *
 *
 */
#ifndef INVENTORYMGR_H
#define INVENTORYMGR_H

#include <vector>
#include "Queue.h"
#include "Message.h"
#include "ProductT.h"
#include "ListVector.h" 

class InventoryMgr
{
public:

  struct InventoryFile {
    std::string prodId = " ";
    std::string invFile = " ";
  };
 
  InventoryMgr(); 
  ~InventoryMgr();                 
  InventoryMgr(const InventoryMgr& copySource);  // copy constructor

  void operator()();               // the thread that monitors the inventory
  
  std::vector<InventoryFile>* GetInventoryFilePtr() const;
  ListVector<ProductT>* GetInventoryDataPtr() const;
  Queue<Message>* GetInvMsgQueuePtr() const;
  
  void InitInventory();
  void InventoryMonitor();

private:
  // files from which we create our inventory 			
  std::vector<InventoryFile>* inventoryFilePtr{nullptr};
  // pointer to the inventory data base
  ListVector<ProductT>* inventoryDataPtr{nullptr};	
  // pointer to Customer->InventoryMgr message queue
  Queue<Message>* invMsgQueuePtr{nullptr};
    
};

#endif
