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
  void operator () ();
  void CustomerManager();
  void CustomerTask(const int delayTime);
  ListVector<ProductT>* GetInventoryListPtr();
private:
  Queue<Message>* SalesMsgQuePtr;
  Queue<Message>* InventoryMsgQuePtr;
  ListVector<ProductT>* InventoryListPtr;
  ProductT getRandomProduct(int&);
};

#endif
