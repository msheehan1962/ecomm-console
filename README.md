# ecomm-console
multi-threaded ecommerce simulator using C++11 and C++14 features.

The purpose of this project was to revise my C++ skills and learn some of the C++11 and C++14 features.

In a nutshell, the simulator consists of three main threads, Inventory Manager, Customer Manager and 
Sales Manager.

The InventoryMgr initializes the product database from the .txt data files and then responds to 
"inventory-low" messages from the Customer threads by replenishing the "no longer available" product 
onto the database.

The CustomerMgr primarily creates Customer threads. The Customer thread gets a random product and searches
the database for that specific product. If the product is found, remove that product and send a message to
the SalesMgr to update the product statistics. If the product is not found, the Customer sends an 
inventory-low message to the InventoryMgr and the task ends!

The SalesMgr monitors the salesMsgQueue and when it receives a message, updates the appropriate sales
statistics. It also monitors the Console Input Queue looking for either a "Print" or a "Quit" command.  

In more detail, the core elements of the simulator are the ListVector database and the Inter-Process 
Communication (IPC) Queues.

The ListVector is a thread-safe singleton template class. It is made up of a std::vector of 
std::shared_ptr objects that point to std::list objects. Each list represents a product type, ie jackets,
shirts, pants, etc. for a total of 5 lists. I chose a singleton class to ensure the simulator had a 
single copy of the database.

The Queue object is a thread-safe template class designed as a singley linked list. The simulator uses 
three Queue objects. a ConsoleInputQueue, an InventoryMessageQueue and a SalesMessageQueue.

Performance:
The Simulator creates just over 14,000 customer threads per second. 
The ListVector database sustains approximatly 26,000 combined read/write operations per second.
