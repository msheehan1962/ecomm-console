#
# Makefile for the Ecommerce simulation
# -
#
all: Ecommerce

Ecommerce: Ecommerce.o InventoryMgr.o CustomerMgr.o SalesMgr.o ProductT.o StringSplit.o Console.o EcommerceConsole.o DateTime.o
	g++ -Wall -std=c++11 -lrt -pthread -g Ecommerce.o InventoryMgr.o CustomerMgr.o SalesMgr.o ProductT.o Console.o EcommerceConsole.o StringSplit.o DateTime.o -o Ecommerce

Ecommerce.o: Ecommerce.cpp
	g++ -Wall -std=c++11 -lrt -pthread -g -c Ecommerce.cpp

InventoryMgr.o: InventoryMgr.cpp InventoryMgr.h
	g++ -std=c++11 -lrt -pthread -g -c InventoryMgr.cpp

CustomerMgr.o: CustomerMgr.cpp CustomerMgr.h
	g++ -std=c++11 -lrt -pthread -g -c CustomerMgr.cpp

SalesMgr.o: SalesMgr.cpp SalesMgr.h DollarAmount.h
	g++ -Wall -std=c++11 -lrt -pthread -g -c SalesMgr.cpp

ProductT.o: ProductT.cpp ProductT.h
	g++ -Wall -std=c++11 -lrt -pthread -g -c ProductT.cpp

StringSplit.o: StringSplit.cpp
	g++ -Wall -std=c++11 -lrt -pthread -g -c StringSplit.cpp

Console.o: Console.cpp Console.h
	g++ -std=c++11 -lrt -pthread -g -c Console.cpp

EcommerceConsole.o: EcommerceConsole.cpp EcommerceConsole.h
	g++ -std=c++11 -lrt -pthread -g -c EcommerceConsole.cpp

DateTime.o: DateTime.cpp DateTime.h
	g++ -Wall -std=c++11 -lrt -pthread -g -c DateTime.cpp

clean:
	rm -rf *.o Ecommerce
