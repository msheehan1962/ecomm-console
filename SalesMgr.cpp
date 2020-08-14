//***************************************************************************
//  Name:  SalesMgr.cpp
//
//
//
//***************************************************************************

#include "SalesMgr.h"
#include "StringSplit.h"
//#include "DollarAmount.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
//#include <thread>
#include <chrono>



extern std::vector<std::string> stringSplit(const std::string& s, char delimiter);
extern Queue<Message> ConsoleInputQueue;

// defualt constructor
SalesMgr::SalesMgr() {
  //std::cout << "SalesMgr constructor\n";
  salesMsgQueuePtr = new Queue<Message>;
  for (int i{0}; i < PRODUCT_CT; i++) {
    menProdCtr[i] = new (ApparelCtr);
    womenProdCtr[i] = new (ApparelCtr);
  }
}

// copy constructor
//*******************************************************************
//  Name: copy constructor()
//
//  Purpose: copy SalesMgr pointers to 
//
//
//*******************************************************************
SalesMgr::SalesMgr( const SalesMgr& copySource) {
  //std::cout << "SalesMgr copy constructor\n";
  salesMsgQueuePtr = copySource.salesMsgQueuePtr;

  for (int i{0}; i < PRODUCT_CT; i++) {
    menProdCtr[i] = copySource.menProdCtr[i];
    womenProdCtr[i] = copySource.womenProdCtr[i];
  }
  
}

//*******************************************************************
//  Name: destructor()
//
//  Purpose: clean up resources
//
//
//*******************************************************************
SalesMgr::~SalesMgr() {
  //std::cout << "~SalesMgr\n";
  //delete salesMsgQueuePtr;
  //for (int i{0}; i < PRODUCT_CT; i++) {
  //  delete menProdCtr[i];
  //  delete womenProdCtr[i];
  //}
}

//*******************************************************************
//  Name: operator() ()
//
//  Purpose: used in main to kick off the sales manager thread
//
//
//*******************************************************************
void SalesMgr::operator () () {
  //std::cout << "SalesMgr functor\n";
  SalesMgrMonitor();
}


//*******************************************************************
//  Name: GetMsgQueuePtr()
//
//  Purpose:  Return the salesMsgQueuePtr
//
//  Parameters: void
//
//  Return:  Queue<Message>* salesMsgQueuePtr
//
//*******************************************************************
Queue<Message>* SalesMgr::GetMsgQueuePtr() {
  return salesMsgQueuePtr;
}

//*******************************************************************
//  Name: SalesMgrMonitor()
//
//  Purpose:  Continuously monitor the sales message queue and
//            update sales counters for each message. Also monitor
//            the console message queue to for print sales info
//            message or quit message.
//
//  Parameters: void
//
//  Return:  void
//
//*******************************************************************
void SalesMgr::SalesMgrMonitor()
{
  Message salesMsg, userMsg;

  while(1) {

    if (!salesMsgQueuePtr->empty()) {

      salesMsg  = salesMsgQueuePtr->dequeue();
      UpdateSalesCounters(salesMsg);
    }

    // check the console queue for any incomming messages
    if (!ConsoleInputQueue.empty()) {
      
      userMsg = ConsoleInputQueue.peek();
      
      if (userMsg.destination == (unsigned)Dest::SALESMGR) {
	userMsg = ConsoleInputQueue.dequeue();
	if (userMsg.type == (unsigned)Type::QUIT) {
	  OutputSalesCounters();
	  break;
	}
	if (userMsg.type == (unsigned)Type::PRINT)
	  OutputSalesCounters();
      }
    }   
  }   // while
  
}

//*******************************************************************
//  Name: UpdateSalesCounters()
//
//  Purpose:  Identify the product sold and update the appropriate 
//            counter (for tracking sales).
//
//  Parameters: Message input (sent from the customer thread)
//
//  Return:  void
//
//*******************************************************************
void SalesMgr::UpdateSalesCounters(Message input)
{
  // parse the input message "," delimeter, into the Tokens vector
  std::vector<std::string> Tokens = stringSplit(input.msgData, ',');

  //std::cout << "UpdatSalesCounters: msgData = " << input.msgData << std::endl;
  
  // determine the index into the counter array. 
  std::string ptype = Tokens[0];   // product type from input message

  int index{0};
  if (ptype == "jacket")
    index = (int)PLIST::JACKET;
  if (ptype == "pants")
    index = (int)PLIST::PANTS;
  if (ptype == "shorts")
    index = (int)PLIST::SHORTS;
  if (ptype == "shirt")
    index = (int)PLIST::SHIRT;
  if (ptype == "socks")
    index = (int)PLIST::SOCKS;

  // which counters, men or women? Set the target product pointer to
  // the appropriate counters
  
  ApparelCtr* tgtCtPtr;
  if ((int)PGENDER::sxM == stoi(Tokens[1]))       // if gender is male
    tgtCtPtr = menProdCtr[index];
  else if ((int)PGENDER::sxF == stoi(Tokens[1]))   // if gender is male 
    tgtCtPtr = womenProdCtr[index];
  else
    std::cout << "UpdateSalesCounters: unknown gender\n";

  tgtCtPtr->itemType++;
  
  // check the size and increment the corresponding counter
  if ((int)PSIZE::szS == stoi(Tokens[2]))      // if size is small
    tgtCtPtr->itemSizeS++;
  else if ((int)PSIZE::szM == stoi(Tokens[2])) // if size is medium
    tgtCtPtr->itemSizeM++;
  else if ((int)PSIZE::szL == stoi(Tokens[2])) // size is large
    tgtCtPtr->itemSizeL++;
  else
    std::cout << "UpdateSalesCounters: unknown size\n";
    
  // check the color and increment the corresponding counter
  if ((int)PCOLOR::BR == stoi(Tokens[3]))      // if color is brown
    tgtCtPtr->itemColorBR++;
  else if ((int)PCOLOR::RD == stoi(Tokens[3])) // if color is red
    tgtCtPtr->itemColorRD++;
  else if ((int)PCOLOR::WH == stoi(Tokens[3])) // if color is white
    tgtCtPtr->itemColorWH++;
  else if ((int)PCOLOR::GR == stoi(Tokens[3])) // if color is green
    tgtCtPtr->itemColorGR++;
  else if ((int)PCOLOR::BK == stoi(Tokens[3])) // if color is black
    tgtCtPtr->itemColorBK++;
  else if ((int)PCOLOR::BL == stoi(Tokens[3])) // if color is blue
    tgtCtPtr->itemColorBL++;
  else
    std::cout << "UpdateSalesCounters: unknown color\n";

  // Add the product cost to the total sales amount
  totalSales += stod(Tokens[4]);
}

//*******************************************************************
//  Name: OutputSalesCounters()
//
//  Purpose:  Send formatted output "sales counters" to stdout
//            
//
//  Parameters: void
//
//  Return:  void
//
//*******************************************************************
using std::setw;
void SalesMgr::OutputSalesCounters()
{
  std::string prodName[PRODUCT_CT] {"Jacket Counters:", "Pants Counters: ",
      "Shorts Counters:", "Shirt Counters: ", "Socks Counters: " };
  ApparelCtr* ctPtr;  // target count pointer
  
  int totalProducts{0}; 

  std::cout << std::endl << std::endl;
    for (int prod{0}; prod < PRODUCT_CT; prod++)
      {
	ctPtr = menProdCtr[prod];
	std::cout << prodName[prod] << setw(8) << "Large" << setw(12)
		  << "Medium" << setw(12) << "Small" << setw(13) << "Total\n";
	std::cout << setw(10) << "  Mens:" << setw(14) << ctPtr->itemSizeL
		  << setw(12) << ctPtr->itemSizeM << setw(12)
		  << ctPtr->itemSizeS << setw(12)
		  << ctPtr->itemSizeL + ctPtr->itemSizeM + ctPtr->itemSizeS
		  << std::endl;
	totalProducts += ctPtr->itemSizeL + ctPtr->itemSizeM + ctPtr->itemSizeS;
	ctPtr = womenProdCtr[prod];
	std::cout << setw(10) << "Womans:" << setw(14) << ctPtr->itemSizeL
		  << setw(12) << ctPtr->itemSizeM << setw(12)
		  << ctPtr->itemSizeS << setw(12)
		  << ctPtr->itemSizeL + ctPtr->itemSizeM + ctPtr->itemSizeS
		  << std::endl;
	totalProducts += ctPtr->itemSizeL + ctPtr->itemSizeM + ctPtr->itemSizeS;
      }

    std::cout << "Total Sales: $" << std::fixed << std::setprecision(2)
	      << setw(12) << totalSales << setw(34) << totalProducts
	      << std::endl;
}
