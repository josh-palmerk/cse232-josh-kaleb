/***********************************************************************
 * Header:
 *    LIST
 * Summary:
 *    Our custom implementation of std::list
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        List         : A class that represents a List
 *        ListIterator : An iterator through List
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once
#include <cassert>     // for ASSERT
#include <iostream>    // for nullptr
#include <new>         // std::bad_alloc
#include <memory>      // for std::allocator
#include <utility>     // for std::move, std::swap
 
class TestList;        // forward declaration for unit tests
class TestHash;

namespace custom
{

/**************************************************
 * LIST
 * Just like std::list
 **************************************************/
template <typename T>
class list
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   friend void swap(list& lhs, list& rhs);
public:  
   // 
   // Construct
   //

   list();
   list(list <T> & rhs);
   list(list <T>&& rhs);
   list(size_t num, const T & t);
   list(size_t num);
   list(const std::initializer_list<T>& il);
   template <class Iterator>
   list(Iterator first, Iterator last);
  ~list() 
   {
      clear();
   }

   // 
   // Assign
   //

   list <T> & operator = (list &  rhs);
   list <T> & operator = (list && rhs);
   list <T> & operator = (const std::initializer_list<T>& il);
   void swap(list <T> & rhs);

   //
   // Iterator
   //

   class  iterator;
   iterator begin()  { return iterator(pHead); }
   iterator rbegin() { return iterator(pTail); }
   iterator end()    { return iterator(nullptr); }

   //
   // Access
   //

   T& front();
   T& back();

   //
   // Insert
   //

   void push_front(const T&  data);
   void push_front(      T&& data);
   void push_back (const T&  data);
   void push_back (      T&& data);
   iterator insert(iterator it, const T& data);
   iterator insert(iterator it, T&& data);

   //
   // Remove
   //

   void pop_back();
   void pop_front();
   void clear();
   iterator erase(const iterator& it);

   // 
   // Status
   //

   bool empty()  const { return numElements == 0; }
   size_t size() const { return numElements;      }


private:
   // nested linked list class
   class Node;

   // member variables
   size_t numElements; // though we could count, it is faster to keep a variable
   Node * pHead;    // pointer to the beginning of the list
   Node * pTail;    // pointer to the ending of the list
};

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <typename T>
class list <T> :: Node
{
public:
   //
   // Construct
   //
   Node()  
   {
      pNext = pPrev = nullptr;
   }
   Node(const T &  data)  
   {
      this->data = data;
      pNext = pPrev = nullptr;
   }
   Node(      T && data)  
   {
      this->data = std::move(data);
      pNext = pPrev = nullptr;
   }

   //
   // Data
   //

   T data;                 // user data
   Node * pNext;       // pointer to next node
   Node * pPrev;       // pointer to previous node
};

/*************************************************
 * LIST ITERATOR
 * Iterate through a List, non-constant version
 ************************************************/
template <typename T>
class list <T> :: iterator
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   template <typename TT>
   friend class custom::list;
public:
   // constructors, destructors, and assignment operator
   iterator() 
   {
      p = nullptr;
   }
   iterator(Node * pNode) 
   {
      p = pNode;
   }
   iterator(const iterator  & rhs) 
   {
      p = rhs.p;
   }
   iterator & operator = (const iterator & rhs)
   {
      p = rhs.p;
      return *this;
   }
   
   // equals, not equals operator
   bool operator == (const iterator & rhs) const { return p == rhs.p; }
   bool operator != (const iterator & rhs) const { return p != rhs.p; }

   // dereference operator, fetch a node
   T & operator * ()
   {
      return p->data;
   }

   // postfix increment
   iterator operator ++ (int postfix)
   {
      iterator tmp(*this);
      ++(*this);
      return tmp;
   }

   // prefix increment
   iterator & operator ++ ()
   {
      if (p)
         p = p->pNext;
      return *this;
   }
   
   // postfix decrement
   iterator operator -- (int postfix)
   {
      iterator tmp(*this);
      --(*this);
      return tmp;
   }

   // prefix decrement
   iterator & operator -- ()
   {
      if (p)
         p = p->pPrev;
      return *this;
   } 

   // two friends who need to access p directly
   friend iterator list <T> :: insert(iterator it, const T &  data);
   friend iterator list <T> :: insert(iterator it,       T && data);
   friend iterator list <T> :: erase(const iterator & it);

private:

   typename list <T> :: Node * p;
};

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T>
list <T> ::list(size_t num, const T & t) 
{
   numElements = 0;
   pHead = pTail = nullptr;
   for (size_t i = 0; i < num; ++i)
      push_back(t);
}

/*****************************************
 * LIST :: ITERATOR constructors
 * Create a list initialized to a set of values
 ****************************************/
template <typename T>
template <class Iterator>
list <T> ::list(Iterator first, Iterator last)
{
   numElements = 0;
   pHead = pTail = nullptr;
   for (auto it = first; it != last; ++it)
      push_back(*it);
}

/*****************************************
 * LIST :: INITIALIZER constructors
 * Create a list initialized to a set of values
 ****************************************/
template <typename T>
list <T> ::list(const std::initializer_list<T>& il)
{
   numElements = 0;
   pHead = pTail = nullptr;
   for (const auto & value : il)
      push_back(value);
}

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T>
list <T> ::list(size_t num)
{
   numElements = 0;
   pHead = pTail = nullptr;
   for (size_t i = 0; i < num; ++i)
      push_back(T());
}

/*****************************************
 * LIST :: DEFAULT constructors
 ****************************************/
template <typename T>
list <T> ::list() 
{
   numElements = 0;
   pHead = pTail = nullptr;
}

/*****************************************
 * LIST :: COPY constructors
 ****************************************/
template <typename T>
list <T> ::list(list& rhs) 
{
   numElements = 0;
   pHead = pTail = nullptr;
   for (auto it = rhs.begin(); it != rhs.end(); ++it)
      push_back(*it);
}

/*****************************************
 * LIST :: MOVE constructors
 * Steal the values from the RHS
 ****************************************/
template <typename T>
list <T> ::list(list <T>&& rhs)
{
   numElements = 0;
   pHead = pTail = nullptr;
   swap(rhs);
}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS 
 *********************************************/
template <typename T>
list <T>& list <T> :: operator = (list <T> && rhs)
{
   if (this != &rhs)
   {
      clear();
      swap(rhs);
   }
   return *this;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
list <T> & list <T> :: operator = (list <T> & rhs)
{
   if (this != &rhs)
   {
      list<T> temp(rhs);
      swap(temp);
   }
   return *this;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
list <T>& list <T> :: operator = (const std::initializer_list<T>& rhs)
{
   clear();
   for (const auto & value : rhs)
      push_back(value);
   return *this;
}

/**********************************************
 * LIST :: CLEAR
 * Remove all the items currently in the linked list
 *     INPUT  :
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
void list <T> :: clear()
{
   Node * pDelete = pHead;
   while (pDelete)
   {
      Node * pNext = pDelete->pNext;
      delete pDelete;
      pDelete = pNext;
   }
   pHead = pTail = nullptr;
   numElements = 0;
}

/*********************************************
 * LIST :: PUSH BACK
 * add an item to the end of the list
 *    INPUT  : data to be added to the list
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> :: push_back(const T & data)
{
   Node * pNew = new Node(data);
   pNew->pNext = nullptr;
   pNew->pPrev = pTail;
   if (pTail)
      pTail->pNext = pNew;
   else
      pHead = pNew;
   pTail = pNew;
   ++numElements;
}

template <typename T>
void list <T> ::push_back(T && data)
{
   Node * pNew = new Node(std::move(data));
   pNew->pNext = nullptr;
   pNew->pPrev = pTail;
   if (pTail)
      pTail->pNext = pNew;
   else
      pHead = pNew;
   pTail = pNew;
   ++numElements;
}

/*********************************************
 * LIST :: PUSH FRONT
 * add an item to the head of the list
 *     INPUT  : data to be added to the list
 *     OUTPUT :
 *     COST   : O(1)
 *********************************************/
template <typename T>
void list <T> :: push_front(const T & data)
{
   Node * pNew = new Node(data);
   pNew->pPrev = nullptr;
   pNew->pNext = pHead;
   if (pHead)
      pHead->pPrev = pNew;
   else
      pTail = pNew;
   pHead = pNew;
   ++numElements;
}

template <typename T>
void list <T> ::push_front(T && data)
{
   Node * pNew = new Node(std::move(data));
   pNew->pPrev = nullptr;
   pNew->pNext = pHead;
   if (pHead)
      pHead->pPrev = pNew;
   else
      pTail = pNew;
   pHead = pNew;
   ++numElements;
}


/*********************************************
 * LIST :: POP BACK
 * remove an item from the end of the list
 *    INPUT  : 
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> ::pop_back()
{
   if (!pTail)
      return;
   Node * pDelete = pTail;
   pTail = pTail->pPrev;
   if (pTail)
      pTail->pNext = nullptr;
   else
      pHead = nullptr;
   delete pDelete;
   --numElements;
}

/*********************************************
 * LIST :: POP FRONT
 * remove an item from the front of the list
 *    INPUT  :
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> ::pop_front()
{
   if (!pHead)
      return;
   Node * pDelete = pHead;
   pHead = pHead->pNext;
   if (pHead)
      pHead->pPrev = nullptr;
   else
      pTail = nullptr;
   delete pDelete;
   --numElements;
}

/*********************************************
 * LIST :: FRONT
 * retrieves the first element in the list
 *     INPUT  : 
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T>
T & list <T> :: front()
{
   static T temp;
   return (pHead ? pHead->data : temp);
}

/*********************************************
 * LIST :: BACK
 * retrieves the last element in the list
 *     INPUT  : 
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T>
T & list <T> :: back()
{
   static T temp;
   return (pTail ? pTail->data : temp);
}

/******************************************
 * LIST :: REMOVE
 * remove an item from the middle of the list
 *     INPUT  : an iterator to the item being removed
 *     OUTPUT : iterator to the new location 
 *     COST   : O(1)
 ******************************************/
template <typename T>
typename list <T> :: iterator  list <T> :: erase(const list <T> :: iterator & it)
{
   Node * pDelete = it.p;
   if (!pDelete)
      return end();

   Node * pNext = pDelete->pNext;
   if (pDelete->pPrev)
      pDelete->pPrev->pNext = pDelete->pNext;
   else
      pHead = pDelete->pNext;

   if (pDelete->pNext)
      pDelete->pNext->pPrev = pDelete->pPrev;
   else
      pTail = pDelete->pPrev;

   delete pDelete;
   --numElements;
   return iterator(pNext);
}

/******************************************
 * LIST :: INSERT
 * add an item to the middle of the list
 *     INPUT  : data to be added to the list
 *              an iterator to the location where it is to be inserted
 *     OUTPUT : iterator to the new item
 *     COST   : O(1)
 ******************************************/
template <typename T>
typename list <T> :: iterator list <T> :: insert(list <T> :: iterator it,
                                                 const T & data) 
{
   Node * pPos = it.p;
   if (pPos == nullptr)
   {
      push_back(data);
      return iterator(pTail);
   }

   Node * pNew = new Node(data);
   pNew->pNext = pPos;
   pNew->pPrev = pPos->pPrev;

   if (pPos->pPrev)
      pPos->pPrev->pNext = pNew;
   else
      pHead = pNew;

   pPos->pPrev = pNew;
   ++numElements;
   return iterator(pNew);
}

template <typename T>
typename list <T> :: iterator list <T> :: insert(list <T> :: iterator it,
   T && data)
{
   Node * pPos = it.p;
   if (pPos == nullptr)
   {
      push_back(std::move(data));
      return iterator(pTail);
   }

   Node * pNew = new Node(std::move(data));
   pNew->pNext = pPos;
   pNew->pPrev = pPos->pPrev;

   if (pPos->pPrev)
      pPos->pPrev->pNext = pNew;
   else
      pHead = pNew;

   pPos->pPrev = pNew;
   ++numElements;
   return iterator(pNew);
}

/**********************************************
 * LIST :: SWAP (MEMBER)
 *********************************************/
template <typename T>
void list <T> :: swap(list <T> & rhs)
{
   std::swap(numElements, rhs.numElements);
   std::swap(pHead, rhs.pHead);
   std::swap(pTail, rhs.pTail);
}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS
 *********************************************/
template <typename T>
void swap(list <T> & lhs, list <T> & rhs)
{
   lhs.swap(rhs);
}


//#endif
}; // namespace custom
