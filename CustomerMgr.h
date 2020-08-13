//
//  CustomerMgr.h
//
//  Description: 
//
//
#ifndef CUSTOMERMGR_H
#define CUSTOMERMGR_H
#include "ListVector.h"
#include "Queue.h"
#include "Message.h"
#include "ProductT.h"

class CustomerMgr
{
public:
  CustomerMgr(ListVector<ProductT>* prodList,
	      Queue<Message>* salesMsgQ,
	      Queue<Message>* inventoryMsgQ);
  ~CustomerMgr();
  // copy constructor
  //CustomerMgr(const CustomerMgr& copySource);  
  //CustomerMgr(CustomerMgr&& moveSource);       // move constructor
  //void operator= (CustomerMgr&& moveSource);   // move assignment operator
  void operator () ();
  void CustomerManager();
  void CustomerTask(int delayTime);
  ListVector<ProductT>* GetInventoryListPtr();
private:
  Queue<Message>* SalesMsgQuePtr;
  Queue<Message>* InventoryMsgQuePtr;
  ListVector<ProductT>* InventoryListPtr;
  ProductT getRandomProduct(int&);
};

#endif
