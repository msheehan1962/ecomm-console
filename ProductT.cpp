//************************************************************************
//  Name: ProductT.cpp
//
//  Purpose: This file contains the support functions for the ProductT 
//           class. The ProductT class describes apparel types and their 
//           different variation characteristics.
//
//
//***********************************************************************

#include "ProductT.h"
#include <iostream>
#include <sstream>

//************************************************************************
//  Name: constructors
//
//  Purpose: Used to instantiate the ProductT objects
//
//************************************************************************
ProductT::ProductT() { std::cout << "ProductT constructor" << std::endl; }

ProductT::ProductT(std::string& pCategory) : prodCategory(pCategory) 
{
  std::cout << "in Product constructor" << std::endl;
}

ProductT::ProductT(std::string& cat, PGENDER gender, PSIZE size, PCOLOR color)
	: prodCategory(cat), prodGender(gender), prodSize(size), prodColor(color) {}

ProductT::ProductT(std::string& cat, PGENDER gender, PSIZE size, PCOLOR color, double cost)
	: prodCategory(cat), prodGender(gender), 
	  prodSize(size), prodColor(color), prodCost(cost){}

//************************************************************************
//  Name: operator const char*
//
//  Purpose: Used to output the contents of the ProductT class
//           to stdout
//
//  Parameters: none
//
//  Returns: formatted string
//
//************************************************************************
ProductT::operator const char*()
{
  std::ostringstream formattedProduct;
  formattedProduct << prodCategory << ',' << (int)prodGender << ','
		   << (int)prodSize << ',' << (int)prodColor;
  std::string prodInString = formattedProduct.str();
  return prodInString.c_str();
}

//************************************************************************
//  Name: operator==
//
//  Purpose: Used to find a matching element in the list of ProductT
//           objects
//
//  Parameters: const ProductT& compareTo - Check this against "compareTo"
//
//  Returns: bool  true if match, false otherwise
//
//************************************************************************
bool ProductT::operator== (const ProductT& compareTo)
{
  return((prodCategory == compareTo.prodCategory) &&
	 (prodGender == compareTo.prodGender) &&
	 (prodSize == compareTo.prodSize) &&
	 (prodColor == compareTo.prodColor));
}

//************************************************************************
//  Name: Setter Functions
//
//  Purpose: Used to set the various field in the ProductT class
//
//  Parameters: various
//
//  Returns: void
//
//************************************************************************
void ProductT::SetCategory(std::string& input)
{
  ProductT::prodCategory = input; // jacket, shirts, pants, socks, shorts
}

void ProductT::SetProductGender(PGENDER input)
{
  prodGender = input;
}

void ProductT::SetProductSize(PSIZE input)
{
  prodSize = input;
}

void ProductT::SetProductColor(PCOLOR input)
{
  prodColor = input;
}

//************************************************************************
//  Name: GetProductCost()
//
//  Purpose: Used to retrieve the product cost of "this" object.
//
//  Parameters: void
//
//  Returns: double product cost
//
//************************************************************************
double ProductT::GetProductCost()
{
  return prodCost;
}

//************************************************************************
//  Name: operator<< "stream insertion operator"
//
//  Purpose: Used stream a formatted contents of a ProductT object
//
//  Parameters: ostream& os - the output stream (cout or a file)
//              ProductT& pt - the product to output
//
//  Returns: ostream for output
//
//************************************************************************
std::ostream& operator<< (std::ostream& os, const ProductT& pt)
{
  os << pt.prodCategory << ',' << (int)pt.prodGender << ','
     << (int)pt.prodSize << ',' << (int)pt.prodColor << ',' << pt.prodCost;
	return os;
}
