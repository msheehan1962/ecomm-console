//****************************************************************************
//  File name: listVector.h
//
//  Description: listVector is a singleton threadsafe template class made
//               up of a std::vector of std::list objects.
//               The vector contains shared_ptr's to lists. The lists 
//               conatin whatever object <typename T> as defined by the
//               client. 
//
//****************************************************************************


#ifndef LISTVECTOR_H
#define LISTVECTOR_H

#include <iostream>
#include <vector>
#include <list>
#include <mutex>
#include <algorithm>
#include "ProductT.h"

extern std::mutex m_Mutex[PRODUCT_CT];

//
// ListVector<T> class definition 
//

template<class  T>
class ListVector
{
public:                                     // return instance ptr to the vector
  static ListVector<T>* get_instance(int nl); // of nl (number of listss) lists.
  static ListVector<T>* get_instance();     // return instance pointer
  void add_element(T& elem, int nlist);     // add element to end of input list
  bool remove_if(T& elem, int nlist);       // remove element if match input elem

private:
  std::vector<std::shared_ptr<std::list<T>>> Lists;
  static ListVector* instancePtr;              // pointer to this object

  ListVector();                                // constructor
  ~ListVector();                               // destructor 
  ListVector(const ListVector&) = delete;      // No copy constructor 
  const ListVector* operator=(const ListVector&) = delete; // No assignment operator

};


template<typename T>
ListVector<T>* ListVector<T>::instancePtr = nullptr;

//****************************************************************************
//  Name:  constructor()
//
//  Purpose:  private used by get_instance() once to create the vector
//            of lists.
//
//  Parameters:  none
//
//  Returns: none
//
//****************************************************************************
template<typename T>
ListVector<T>::ListVector() {}



//****************************************************************************
//  Name:  destructor()
//
//  Purpose:  private - release the instance pointer
//
//  Parameters:  none
//
//  Returns: none
//
//****************************************************************************
template<typename T>
ListVector<T>::~ListVector() {
  //std::cout << "~ListVector\n";
  delete instancePtr;
}

//****************************************************************************
//  Name:  get_instance()
//
//  Purpose:  Function to instantiate teh ListVector object and to initialize
//            a spared_pointer for each list in the Vector.
//
//  Parameters: int nlist - The number of lists to initialize
//
//  Return: ListVector<T>* - pointer to the static list vector (data base)
//
//****************************************************************************
template<typename T>
ListVector<T>* ListVector<T>::get_instance(int nl)
{
  if (!instancePtr) {
    instancePtr = new ListVector<T>;

    // initialize each list and then push the shared_ptr to each
    // of the "nl" number of lists onto the vector.
    for (int j = 0; j < nl; j++)
      {
	std::list<T>* mlistP = new std::list<T>;
	std::shared_ptr<std::list<T>> sp1(mlistP);
	instancePtr->Lists.push_back(sp1);
      }
  }
  return instancePtr;
}

//****************************************************************************
//  Name:  get_instance()
//
//  Purpose:  access function for the ListVector Singleton class.  
//
//  Parameters: none
//
//  Return: ListVector<T>* - pointer to the static list vector (data base)
//                           or nullptr if not instantiated yet.
//
//****************************************************************************
template<typename T>
ListVector<T>* ListVector<T>::get_instance()
{
  if (!instancePtr) 
    instancePtr = nullptr;
  return instancePtr;
}

//****************************************************************************
//  Name:  add_element()
//
//  Purpose:  push the input T& elem onto the input list.
//
//  Parameters: T& elem   - element to add to the list
//              int nlist - index to identify which list to add to
//
//  Return: void
//
//****************************************************************************
template<typename T>
void ListVector<T>::add_element(T& elem, int nlist)
{
  // lock access to the list
  std::lock_guard<std::mutex> lock(m_Mutex[nlist]);

  // get the shared pointer to the appropriate list
  auto ml = instancePtr->Lists.at(nlist);

  // add the element to the end of the list
  ml->push_back(elem);
}

//****************************************************************************
//  Name:  remove_if()
//
//  Purpose:  Search the list (indexed by nlist) for an element matching the
//            input T& elem.  If found, remove it else return false.
//
//  Parameters: T& elem   - copy of the element to search for
//              int nlist - index to identify which list to search
//
//  Return: bool true if element found, else return false
//
//****************************************************************************
template<typename T>
bool ListVector<T>::remove_if(T& elem, int nlist)
{
  // lock access to the list
  std::lock_guard<std::mutex> lock(m_Mutex[nlist]);
  
  // instantiate the appropriate list shared_ptr
  auto shrd_lst = instancePtr->Lists.at(nlist);
  
  auto lst = shrd_lst.get();
  
  // find the element if it's on the list
  auto elfound = find(lst->begin(), lst->end(), elem);
  
  if (elfound == lst->end()) {
    return false;
  }
  
  lst->erase(elfound);
  return true;
}

#endif

