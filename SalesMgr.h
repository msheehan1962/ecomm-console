/*
 *  Name: SalesMgr.h
 *
 *  Description: The SalesMgr object is responsible for collecting and reporting
 *               sales information for the Ecommerce simulation. 
 *
 */
#ifndef SALESMGR_H
#define SALESMGR_H
#include <iostream>
#include <string>
#include <sstream>
#include "ProductT.h"
#include "Queue.h"
#include "Message.h"



//std::string productType[] { "jacket", "pants", "shorts", "shirt", "socks" };  

class SalesMgr
{
public:

  SalesMgr();                                      // constructor
  explicit SalesMgr(Queue<Message>* smq);          // constructor
  ~SalesMgr();                                     // destructor
  SalesMgr(const SalesMgr& copySource);            // copy constructor
  void SalesMgrMonitor();                          // sales monitor
  void operator () ();
  void UpdateSalesCounters(const Message);
  void OutputSalesCounters();
  Queue<Message>* GetMsgQueuePtr();

  //  friend std::ostream& operator<< (std::ostream& os, const Message& sm);

 private:
  struct ApparelCtr {

    int itemType{0};
    int itemSizeS{0};           // Small counter
    int itemSizeM{0};           // Medium counter
    int itemSizeL{0};           // Large counter
    int itemColorBR{0};         // Brown counter
    int itemColorRD{0};         // Red counter
    int itemColorWH{0};         // White counter
    int itemColorGR{0};         // Green counter
    int itemColorBK{0};         // Black counter
    int itemColorBL{0};         // Blue counter
    double itemCost{0.0};
  };

  double totalSales{0.0};
  ApparelCtr* menProdCtr[5]{nullptr, nullptr, nullptr, nullptr, nullptr};
  ApparelCtr* womenProdCtr[5]{nullptr, nullptr, nullptr, nullptr, nullptr};

  Queue<Message>* salesMsgQueuePtr{nullptr};;  // apparel, M or F, size, color
	
};





#endif

