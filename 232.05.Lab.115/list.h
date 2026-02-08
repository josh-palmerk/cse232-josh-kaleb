/***********************************************************************
 * Header:
 *    LIST
 * Summary:
 *    Our custom implementation of std::list
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        List         : A class that represents a List
 *        ListIterator : An iterator through List
 * Author
 *    JOsh, Kaleb, Spencer
 ************************************************************************/

#pragma once
#include <cassert>     // for ASSERT
#include <iostream>    // for nullptr
#include <new>         // std::bad_alloc
#include <memory>      // for std::allocator

class TestList; // forward declaration for unit tests
class TestHash; // forward declaration for hash used later

namespace custom
{

/**************************************************
 * LIST
 * Just like std::list
 **************************************************/
template <typename T, typename A = std::allocator<T>>
class list
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   friend void swap(list& lhs, list& rhs);
public:
   
   //
   // Construct
   //
   
   list(const A& a = A())  
   {
       numElements = 0;
       pHead = nullptr;
       pTail = nullptr;

   }
   list(list <T, A> & rhs, const A& a = A())
   {
       pHead = pTail = nullptr;
       numElements = 0;
       for (Node* cur = rhs.pHead; cur; cur = cur->pNext)
           push_back(cur->data);

   }
   list(list <T, A>&& rhs, const A& a = A());
   list(size_t num, const T & t, const A& a = A());
   list(size_t num, const A& a = A());
   list(const std::initializer_list<T>& il, const A& a = A())
   {
       pHead = pTail = nullptr;
       numElements = 0;
       for (const T& x : il)
           push_back(x);
   }

   template <class Iterator>
   list(Iterator first, Iterator last, const A& a = A())
   {
       pHead = pTail = nullptr;
       numElements = 0;
       for (; first != last; ++first)
           push_back(*first);
   }

   ~list()
   { 
       clear();
   }
   
   //
   // Assign
   //
   
   list <T, A> & operator = (list <T, A> &  rhs);
   list <T, A> & operator = (list <T, A> && rhs);
   list <T, A> & operator = (const std::initializer_list<T>& il);
   void swap(list <T, A>& rhs)
   {
       std::swap(pHead, rhs.pHead);
       std::swap(pTail, rhs.pTail);
       std::swap(numElements, rhs.numElements);
   }


   //
   // Iterator
   //
   
   class iterator;
   iterator begin()  { return iterator (pHead);   }
   iterator rbegin() { return iterator (pTail);   }
   iterator end()    { return iterator (nullptr); }
   
   //
   // Access
   //
   
   T & front();
   T & back();

   //
   // Insert
   //
   
   void push_front(const T &  data);
   void push_front(      T && data);
   void push_back (const T &  data);
   void push_back (      T && data);
   iterator insert(iterator it, const T &  data);
   iterator insert(iterator it,       T && data);

   //
   // Remove
   //
   
   void pop_back();
   void pop_front();
   void clear();
   iterator erase(const iterator & it);

   //
   // Status
   //
   
   bool empty()  const { return (pHead == nullptr); }
   size_t size() const { return numElements;   }

private:
   // nested linked list class
   class Node;

   // member variables
   A    alloc;         // use alloacator for memory allocation
   size_t numElements; // though we could count, it is faster to keep a variable
   Node * pHead;       // pointer to the beginning of the list
   Node * pTail;       // pointer to the ending of the list
};

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <typename T, typename A>
class list <T, A> :: Node
{
public:
   //
   // Construct
   //
   Node()
   {
      pNext = pPrev = nullptr;
   }
   Node(const T& data) : data(data)
   {
      pNext = pPrev = nullptr;
   }
   Node(T&& data) : data(std::move(data))
   {
      pNext = pPrev = nullptr;
   }

   //
   // Member Variables
   //

   T data;             // user data
   Node * pNext;       // pointer to next node
   Node * pPrev;       // pointer to previous node
};

/*************************************************
 * LIST ITERATOR
 * Iterate through a List, non-constant version
 ************************************************/
template <typename T, typename A>
class list <T, A> :: iterator
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   template <typename TT, typename AA>
   friend class custom::list;
   
public:
   // constructors, destructors, and assignment operator
	iterator() : p(nullptr)
   {
       p = nullptr;
   }
   iterator(Node * pRHS)
   {
       p = pRHS;
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
       assert(p != nullptr);
       return p->data;
   }

   // postfix increment
   iterator operator ++ (int postfix)
   {
	   iterator temp(*this);
       if (p != nullptr)
		   p = p->pNext;
	   return temp;
   }

   // prefix increment
   iterator & operator ++ ()
   {
      if (p != nullptr)
		  p = p->pNext;
	  return *this;
   }
   
   // postfix decrement
   iterator operator -- (int postfix)
   {
	   iterator temp(*this);
       if (p != nullptr)
		   p = p->pPrev;
	   return temp;
   }

   // prefix decrement
   iterator & operator -- ()
   {
	   if (p != nullptr) 
		   p = p->pPrev;
       return *this;
   } 

   // two friends who need to access p directly
   friend iterator list <T, A> :: insert(iterator it, const T &  data);
   friend iterator list <T, A> :: insert(iterator it,       T && data);
   friend iterator list <T, A> :: erase(const iterator & it);

private:

   typename list <T, A> :: Node * p;
};

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T, typename A>
list <T, A> ::list(size_t num, const T & t, const A& a) 
{
    pHead = pTail = nullptr;
    numElements = 0;
    for (size_t i = 0; i < num; i++)
        push_back(t); // copy construct
}

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T, typename A>
list <T, A> ::list(size_t num, const A& a) 
{
    pHead = pTail = nullptr;
    numElements = 0;
    for (size_t i = 0; i < num; i++)
        push_back(T());  // default construct
}

/*****************************************
 * LIST :: MOVE constructors
 * Steal the values from the RHS
 ****************************************/
template <typename T, typename A>
list <T, A> ::list(list <T, A>&& rhs, const A& a) :
   numElements(rhs.numElements), pHead(rhs.pHead), pTail(rhs.pTail), alloc(a)
{
   rhs.pHead = rhs.pTail = nullptr;
   rhs.numElements = 0;
}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS 
 *********************************************/
template <typename T, typename A>
list <T, A>& list <T, A> :: operator = (list <T, A> && rhs)
{
    if (this != &rhs)
    {
        clear(); // Clear existing elements
        pHead = rhs.pHead;
        pTail = rhs.pTail;
        numElements = rhs.numElements;
        rhs.pHead = nullptr;
        rhs.pTail = nullptr;
        rhs.numElements = 0;
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
template <typename T, typename A>
list <T, A>& list <T, A> :: operator = (list <T, A>& rhs)
{
    if (this == &rhs)
        return *this;

    Node* d = pHead;
    Node* s = rhs.pHead;

    // 1) assign into existing nodes
    while (d && s)
    {
        d->data = s->data;   // assignment, not construction
        d = d->pNext;
        s = s->pNext;
    }

    // 2) source has extra nodes , copy construct
    while (s)
    {
        push_back(s->data);
        s = s->pNext;
    }

    // 3) destination has extra nodes , delete them
    while (d)
    {
        Node* kill = d;
        d = d->pNext;
        erase(iterator(kill));
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
template <typename T, typename A>
list <T, A>& list <T, A> :: operator = (const std::initializer_list<T>& rhs)
{
    Node* d = pHead;
    auto s = rhs.begin();

    // assign into existing nodes
    while (d && s != rhs.end())
    {
        d->data = *s;
        d = d->pNext;
        ++s;
    }

    // source bigger, allocate
    while (s != rhs.end())
    {
        push_back(*s);
        ++s;
    }

    // destination bigger, delete
    while (d)
    {
        Node* kill = d;
        d = d->pNext;
        erase(iterator(kill));
    }

    return *this;
}


/**********************************************
 * LIST :: CLEAR
 * Remove all the items currently in the linked list
 *     INPUT  :
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T, typename A>
void list <T, A> :: clear()
{
    while (pHead != nullptr)
    {
        Node* temp = pHead;
        pHead = pHead->pNext;
        delete temp;
	}
	pTail = nullptr;
	numElements = 0;
}

/*********************************************
 * LIST :: PUSH BACK
 * add an item to the end of the list
 *    INPUT  : data to be added to the list
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list <T, A> :: push_back(const T & data)
{
    if (pTail == nullptr)
    {
        pTail = new Node(data);
        pHead = pTail;
    }
    else
    {
        Node* newNode = new Node(data);
        newNode->pPrev = pTail;
        pTail->pNext = newNode;
        pTail = newNode;
    }
	numElements++;
}

template <typename T, typename A>
void list <T, A> ::push_back(T && data)
{
    if (pTail == nullptr)
    {
        pTail = new Node(std::move(data));
        pHead = pTail;
    }
    else
    {
        Node* newNode = new Node(std::move(data));
        newNode->pPrev = pTail;
        pTail->pNext = newNode;
		pTail = newNode;
    }

	numElements++;
}

/*********************************************
 * LIST :: PUSH FRONT
 * add an item to the head of the list
 *     INPUT  : data to be added to the list
 *     OUTPUT :
 *     COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list <T, A> :: push_front(const T & data)
{
    Node* newNode = new Node(data);
   
    if (!pHead)
    {
       pHead = pTail = newNode;
    }
    else
    {
        newNode->pNext = pHead;
        pHead->pPrev = newNode;
        pHead = newNode;
	}
	numElements++;
}

template <typename T, typename A>
void list <T, A> ::push_front(T && data)
{
    Node* newNode = new Node(std::move(data));
   
    if (!pHead)
    {
        pHead = pTail = newNode;
    }
    else
    {
        newNode->pNext = pHead;
        pHead->pPrev = newNode;
        pHead = newNode;
	 }
	 numElements++;
}


/*********************************************
 * LIST :: POP BACK
 * remove an item from the end of the list
 *    INPUT  : 
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list <T, A> ::pop_back()
{
    if (pTail != nullptr)
    {
        Node* temp = pTail;
        pTail = pTail->pPrev;
        if (pTail != nullptr)
            pTail->pNext = nullptr;
        else
            pHead = nullptr; // List is now empty
        delete temp;
		numElements--;
    }
}

/*********************************************
 * LIST :: POP FRONT
 * remove an item from the front of the list
 *    INPUT  :
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list <T, A> ::pop_front()
{
    if (pHead != nullptr)
    {
        Node* temp = pHead;
        pHead = pHead->pNext;
        if (pHead != nullptr)
            pHead->pPrev = nullptr;
        else
            pTail = nullptr; // List is now empty
        delete temp;
	    numElements--;
	}

}

/*********************************************
 * LIST :: FRONT
 * retrieves the first element in the list
 *     INPUT  : 
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T, typename A>
T & list <T, A> :: front()
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
template <typename T, typename A>
T & list <T, A> :: back()
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
template <typename T, typename A>
typename list <T, A> :: iterator  list <T, A> :: erase(const list <T, A> :: iterator & it)
{
	Node* nodeToRemove = it.p;

	Node* nextNode = (nodeToRemove != nullptr) ? nodeToRemove->pNext : nullptr;

    if (nodeToRemove != nullptr)
    {
        if (nodeToRemove->pPrev != nullptr)
            nodeToRemove->pPrev->pNext = nodeToRemove->pNext;
        else
            pHead = nodeToRemove->pNext; // Removing the head

        if (nodeToRemove->pNext != nullptr)
            nodeToRemove->pNext->pPrev = nodeToRemove->pPrev;
        else
            pTail = nodeToRemove->pPrev; // Removing the tail
        delete nodeToRemove;
        numElements--;
	}
	return iterator(nextNode);
}

/******************************************
 * LIST :: INSERT
 * add an item to the middle of the list
 *     INPUT  : data to be added to the list
 *              an iterator to the location where it is to be inserted
 *     OUTPUT : iterator to the new item
 *     COST   : O(1)
 ******************************************/
template <typename T, typename A>
typename list <T, A> :: iterator list <T, A> :: insert(list <T, A> :: iterator it,
                                                 const T & data) 
{
    if (it.p == nullptr)
    {
        push_back(data);
        return iterator(pTail);
    }

    Node* cur = it.p;
    Node* n = new Node(data);

    n->pNext = cur;
    n->pPrev = cur->pPrev;

    if (cur->pPrev)
        cur->pPrev->pNext = n;
    else
        pHead = n;

    cur->pPrev = n;

    numElements++;
    return iterator(n);
}


/******************************************
 * LIST :: INSERT
 * add several items into the middle of the list
 *     INPUT  : data to be added to the list
 *              an iterator to the location where it is to be inserted
 *     OUTPUT : iterator to the new item
 *     COST   : O(1)
 ******************************************/
template <typename T, typename A>
typename list <T, A> ::iterator list <T, A> ::insert(list <T, A> ::iterator it,
   T && data)
{
    if (it.p == nullptr)
    {
        push_back(std::move(data));
        return iterator(pTail);
    }

    Node* cur = it.p;
    Node* n = new Node(std::move(data));

    n->pNext = cur;
    n->pPrev = cur->pPrev;

    if (cur->pPrev)
        cur->pPrev->pNext = n;
    else
        pHead = n;

    cur->pPrev = n;

    numElements++;
    return iterator(n);
}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS
 *********************************************/
template <typename T, typename A>
void swap(list <T, A> & lhs, list <T, A> & rhs)
{
    std::swap(lhs.pHead, rhs.pHead);
    std::swap(lhs.pTail, rhs.pTail);
	std::swap(lhs.numElements, rhs.numElements);

}

}; // namespace custom


