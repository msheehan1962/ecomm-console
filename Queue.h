//*********************************************************************************
//  Name: Queue.h
//
//  Description: This queue implements the basic functionality of any
//               queue class. The container is a linked list for optimum
//               resource management and is threadsafe.
//  
//  Interface:
//         void          enqueue(const Elem&);   // push
//         Elem          dequeue(void);          // pop
//         Elem          peek(void);             // look at queue->front
//         bool          empty(void);            // is the queue empty?
//         void          qShow();                // display queue contents
//         inline size_t qSize();                // return # of queue elements
//
//  Author:     Mark Sheehan   Jan 27, 2020
//
//***********************************************************************************

#ifndef QUEUE_H
#define QUEUE_H
#include <iostream>
#include <mutex>
#include "Message.h"

#define QUEUE_EMPTY -1

extern std::mutex m_Qutex;

//
//  Queue class definition
//
template <typename Elem>
class Queue {
 public:

  struct Node {
    Elem data;
    Node* next;
  };

  Queue() {front = rear = nullptr; }  // default constructor
  explicit Queue(const Elem&);        // construct with first element
  Queue(const Queue&);                // copy constructor 
  ~Queue();                           // destructor
  
  void       enqueue(const Elem&);    // push
  Elem       dequeue(void);           // pop
  Elem       peek(void) const;        // look at the front of the queue
  bool       empty(void) const;       // is the queue empty?
  void       qShow() const;           // display the content of the queue
  inline size_t qSize() const { return qsize; } // return # of elements on the queue
  
 protected:
  Node    *front{nullptr};            // pointer to the front of the queue
  Node    *rear{nullptr};             // pointer to the rear of the queue
  size_t  qsize{0};                   // element counter
};


//***************************************************************************
//  Name:  constructor
//
//  Purpose: Create the Queue object
//
//  Parameters:  T& item  - initial element on the queue
//
//  Return:   void
//
//**************************************************************************
template <typename Elem>
Queue<Elem>::Queue(const Elem& Item) {
  front = rear = new Node;
  rear->data = Item;
  qsize++;
}

//***************************************************************************
//  Name:  destructor
//
//  Purpose: Relinquish any elements on the Queue
//
//  Parameters:  none
//
//  Return:   void
//
//**************************************************************************
template <typename Elem>
Queue<Elem>::~Queue() {
  //std::cout << "~Queue()\n";
  for (auto count = qSize(); count > 0; count--) {
    Node* temp = front;
    front = front->next;
    delete temp;
  }
}

//***************************************************************************
//  Name:  enqueue()
//
//  Purpose: insert an element onto the rear of the queue (Push)
//
//  Parameters:  Elem - element to insert onto the queue
//
//  Return:   void
//
//**************************************************************************
template <typename Elem>
void Queue<Elem>::enqueue(const Elem &Item) {
  
  std::lock_guard<std::mutex> lock(m_Qutex);

  if (qsize == 0)
    front = rear = new Node;
  else
    rear = rear->next = new Node;
  rear->data = Item;
  qsize++;
}

//***************************************************************************
//  Name:  dequeue()
//
//  Purpose: Remove the element at the front of the queue (Pop)
//
//  Parameters:  void
//
//  Return:   Elem - the element at the front of the queue
//
//**************************************************************************
template <typename Elem>
Elem Queue<Elem>::dequeue(void) {
  Elem rdData;                     // storage for the element at the front 
                                   // of the queue (to return)
  
  std::lock_guard<std::mutex> lock(m_Qutex);
  
  if (empty()) {
    std::cout << "UnderFlow Error\n";
  }
    
  else {
    Node* temp = front;
    rdData = front->data;
    front = front->next;
    qsize--;
    delete temp;
  }
  return rdData;
}

//***************************************************************************
//  Name:  peek()
//
//  Purpose:  Return the element at the front of the queue don't remove it
//
//  Parameters:  void
//
//  Return:   element on the front of the queue
//
//**************************************************************************
template <typename Elem>
Elem Queue<Elem>::peek(void) const {

  std::lock_guard<std::mutex> lock(m_Qutex);
  if (qsize == 0) {
    Message nullMsg;
    nullMsg.destination = (unsigned)Dest::NULLMSG;
    return nullMsg;
  } else
    return (front->data);
}

//***************************************************************************
//  Name:  empty()
//
//  Purpose: Check if the Queue is has element on it or not
//
//  Parameters:  void
//
//  Return:   bool true is empty, false if not empty
//
//**************************************************************************
template <typename Elem>
bool Queue<Elem>::empty(void) const {
  if (qsize == 0)
    return true;
  else
    return false;
}

// 
//***************************************************************************
//  Name:  qShow()
//
//  Purpose: display contents of the queue to stdout
//
//  Parameters: void
//
//  Return:   void
//
//**************************************************************************
template <typename Elem>
void Queue<Elem>::qShow(void) const {
  Node* temp = front;
    for (auto count = qSize(); count > 0; count--) {
      //std::cout << temp->data << " - ";
      temp = temp->next;
    }
  std::cout << std::endl;

}
#endif
