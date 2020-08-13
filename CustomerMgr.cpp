//
//  Name: CustomerMgr.cpp
//

#include <thread>
#include <algorithm>
#include "CustomerMgr.h"
#include "ProductT.h"
#include "ListVector.h"
#include "DateTime.h"

using namespace std;

extern Queue<Message> ConsoleInputQueue;


//********************************************************************
//  Name:  constructor()
//
//  Purpose: - Instantiate the CustomerMgr object. Initialize the
//             Product data base pointer, the Inventory message queue
//              pointer and the Sales message queue pointer.
// 
//  Parameters: ListVector<ProductT>* - Pointer to product data base
//              Queue<Message>*  - Pointer to the inventory msg queue
//              Queue<Message)*  - Pointer to the sales msg queue
//
//  returns: void
//
//********************************************************************
CustomerMgr::CustomerMgr(ListVector<ProductT>* prodList,
			 Queue<Message>* InvMsgPtr,
			 Queue<Message>* SalesMsgPtr) :
  InventoryListPtr(prodList), InventoryMsgQuePtr(InvMsgPtr),
  SalesMsgQuePtr(SalesMsgPtr) {}



CustomerMgr::~CustomerMgr()
{
  //std::cout << "~CustomerMgr\n";
}

//********************************************************************
//  Name:  operator() ()
//
//  Purpose: - Called from main to start the Customer Manager thread.
// 
//  Parameters: void
//
//  returns: void
//
//********************************************************************
void CustomerMgr::operator () () {
  //std::cout << "CustomerMgr functor\n";
  CustomerManager();
}

//********************************************************************
//  Name:  GetInventoryListPtr()
//
//  Purpose: - Retrun the Inventory List pointer
// 
//  Parameters: void
//
//  returns: ListVector<ProductT>*  pointer to the product data base
//
//********************************************************************
ListVector<ProductT>* CustomerMgr::GetInventoryListPtr() {
  return InventoryListPtr;
}

//********************************************************************
//  Name:  CustomerManager()
//
//  Purpose: - Create the customer threads and monitor the
//             ConsoleInputQueue for incomming messages.
// 
//  Parameters: none
//
//  returns: void
//
//********************************************************************

void CustomerMgr::CustomerManager()
{
  
  Message userMsg;
  // seed the random number generator for the CustomerTask() to
  // get a random selection of products.
  srand((unsigned)time(0));

  // use the DateTime object to track our runTime.
  DateTime timer;
  timer.setStartTime();
  
  int loopCt{0};
  while (1) {

    // each thread gets a selay time from 0 to 1 millisecond
    int threadDelay = rand() % 2; // delay between 0 and 1 msec

    // create the CustomerTask thread with delay and detach
    std::thread customerThread(&CustomerMgr::CustomerTask, this, threadDelay);
    customerThread.detach();
    
    loopCt++;

    // check the console queue for any incomming messages. Only message
    // supported for CustomerManager is QUIT.
    if (!ConsoleInputQueue.empty()) {
      userMsg = ConsoleInputQueue.peek();
      if (userMsg.destination == (unsigned)Dest::CUSTOMERMGR) {
	// it's for me, pop the message off the queue
	userMsg = ConsoleInputQueue.dequeue();
	if (userMsg.type == (unsigned)Type::QUIT) {
	  std::cout << timer.getRelativeTime() << " seconds\n";
	  break;
	}
      }
    }
    
    if (loopCt == 20000000000) {
      std::cout << timer.getRelativeTime() << " seconds\n";
      break;
    }
  }
}

//********************************************************************
//  Name:  CustomerTask()
//
//  Purpose: - Retrieve a random product and attempt to buy that
//             product. If it's out-of-stock send a low-inventory
//             message to the Inventory Manager. Otherwise remove it
//             from the the data base and send message to the Sales
//             Manager to update sales statistics.
// 
//  Parameters: int delay time (time for the customer to wait in line)
//
//  returns: void
//
//********************************************************************
void CustomerMgr::CustomerTask(int delayTime)
{
  // delay to simulate a real person
  std::this_thread::sleep_for(chrono::milliseconds(delayTime));

  // get the data base (inventory data pointer)
  ListVector<ProductT>* lclInvDataPtr = GetInventoryListPtr();
  
  // variable item gets the index associated with the product type
  // lclProduct gets the random product.
  int item{0};        
  ProductT lclProduct = getRandomProduct(item);

  // Pack the random product into a string for the message queue.
  // Either the Sales or Inventory Low message queue.
  
  ostringstream product;
  product << lclProduct.GetProductCategory() << ','
	  << lclProduct.GetProductGender() << ','
	  << lclProduct.GetProductSize() << ','
	  << lclProduct.GetProductColor() << ','
	  << lclProduct.GetProductCost();

  // If the product was not found, (remove_if returns false), send an inventory
  // low message to the inventory manager.
  if (!lclInvDataPtr->remove_if(lclProduct, item)) {
    Message lclMsg;
    lclMsg.destination = (unsigned)Dest::INVENTORYMGR;
    lclMsg.source = (unsigned)Dest::CUSTOMER;
    lclMsg.type = (unsigned)Type::LOWINVENTORY;
    lclMsg.msgData = product.str();
    InventoryMsgQuePtr->enqueue(lclMsg);
  }
  
  // if the product was found, (remove_if returns true), send an message to
  // sales manager so it can update the sales statistics.
  else {
    Message lclMsg;
    lclMsg.destination = (unsigned)Dest::SALESMGR;
    lclMsg.source = (unsigned)Dest::CUSTOMER;
    lclMsg.type = (unsigned)Type::SALE;
    lclMsg.msgData = product.str();
    SalesMsgQuePtr->enqueue(lclMsg);
  }    
  
}  // end CustomerTask()

//********************************************************************
//  Name:  getRandomProduct() 
//
//  Purpose: - Return a randomly generated product of type ProductT.
//             Also return the index into the vector for the list
//             that stores this particular product type.  
// 
//  Parameters: int delay time (time for the customer to wait in line)
//
//  returns: void
//
//********************************************************************
// 
// return a randomly generated product of type ProductT. Also return
// the vector index for the appropriate list in the input parameter.
ProductT CustomerMgr::getRandomProduct(int& whichProd)
{
  // Randomly determine which product to "purchase".
    
  int size = rand() % 3;     // S M or L
  int color = rand() % 6;    // BR, RD, WH, GR, BK, BL
  int gender = rand() % 2;   // M or F
  int item = rand()%10;	     // jacket, pants, shorts, shirt, socks
  double cost;

  string itemString;         // indicates the product type
  if (item < 2) {
    itemString = "jacket";
    cost = 34.99;
    whichProd = 0;
  }
  else if (item < 4) {
    itemString = "pants";
    cost = 15.39;
    whichProd = 1;
  }
  else if (item < 6) {
    itemString = "shorts";
    cost = 24.99;
    whichProd = 2;
  }
  else if (item < 8) {
    itemString = "shirt";
    cost = 15.99;
    whichProd = 3;
  }
  else {
    itemString = "socks";
    cost = 7.79;
    whichProd = 4;
  }

  // instantiate the product and send it back to the customer. 
  ProductT lclProduct(itemString, (PGENDER)gender,
		      (PSIZE) size, (PCOLOR) color, cost);

  return (lclProduct);
}


