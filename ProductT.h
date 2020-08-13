//**********************************************************************
//  Name: ProductT.h
//
//  Purpose:  
//
//
//**********************************************************************


#ifndef PRODUCTDB_PRODUCTT_H
#define PRODUCTDB_PRODUCTT_H
#include <string>
#include <iostream>

#define PRODUCT_CT 5

enum class PSIZE { szS = 0, szM, szL };
enum class PGENDER { sxM = 0, sxF, sxU };
enum class PCOLOR { BR = 0, RD, WH, GR, BK, BL };
enum class PLIST { JACKET = 0, PANTS, SHORTS, SHIRT, SOCKS };

class ProductT
{
 public:
  ProductT();
  ProductT(std::string& pCategory);
  ProductT(std::string& cat, PGENDER gender, PSIZE size, PCOLOR color);
  ProductT(std::string& cat, PGENDER gender, PSIZE size, PCOLOR color,
	   double cost);
  ~ProductT() {}
  operator const char* ();
  bool operator== (const ProductT& compareTo);
  std::string GetProductCategory() { return prodCategory; }
  int  GetProductGender() { return (int)prodGender; }
  int  GetProductSize() { return (int)prodSize; }
  int  GetProductColor() { return (int)prodColor; }
  void SetCategory(std::string& input);
  void SetProductGender(PGENDER input);
  void SetProductSize(PSIZE input);
  void SetProductColor(PCOLOR input);
  double GetProductCost();
  
  friend std::ostream& operator<< (std::ostream& os, const ProductT& pt);

private:                           //  0       1      2       3      4
  std::string prodCategory = "";   // jacket, pants, shorts, shirt, socks, 
  PGENDER prodGender = PGENDER::sxM;
  PSIZE  prodSize = PSIZE::szS;
  PCOLOR prodColor = PCOLOR::BR;
  double  prodCost = 0.0;
};
#endif   //PRODUCTDB_PRODUCTT_H


