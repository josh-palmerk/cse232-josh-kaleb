/***********************************************************************
 * Header:
 *    NODE
 * Summary:
 *    One node in a linked list (and the functions to support them).
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        Node         : A class representing a Node
 *    Additionally, it will contain a few functions working on Node
 * Author
 *    JOsh, Kaleb, Spencer
 ************************************************************************/

#pragma once

#include <cassert>     // for ASSERT
#include <iostream>    // for NULL

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <class T>
class Node
{
public:
   //
   // Construct
   //
   Node(): data(), pNext(nullptr), pPrev(nullptr) {}
   Node(const T& data): data(data), pNext(nullptr), pPrev(nullptr) {}
   Node(T&& data): data(std::move(data)), pNext(nullptr), pPrev(nullptr) {}

   //
   // Member variables
   //

   T data;                 // user data
   Node <T> * pNext;       // pointer to next node
   Node <T> * pPrev;       // pointer to previous node
};

/***********************************************
 * COPY
 * Copy the list from the pSource and return
 * the new list
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline Node<T>* copy(const Node<T>* pSource)
{
   if (!pSource)
      return nullptr;

   Node<T>* pNewHead = new Node<T>(pSource->data);
   Node<T>* pCurrentNew = pNewHead;
   const Node<T>* pCurrentSource = pSource->pNext;

   while (pCurrentSource)
   {
      Node<T>* pNewNode = new Node<T>(pCurrentSource->data);
      pCurrentNew->pNext = pNewNode;
      pNewNode->pPrev = pCurrentNew;
      pCurrentNew = pNewNode;
      pCurrentSource = pCurrentSource->pNext;
   }
   return pNewHead;
}

/***********************************************
 * Assign
 * Copy the values from pSource into pDestination
 * reusing the nodes already created in pDestination if possible.
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline void assign(Node<T>*& pDestination, const Node<T>* pSource)
{
   
}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(Node <T>* &pLHS, Node <T>* &pRHS)
{
	std::swap(pLHS, pRHS);
}

/***********************************************
 * REMOVE
 * Remove the node pSource in the linked list
 *   INPUT  : the node to be removed
 *   OUTPUT : the pointer to the parent node
 *   COST   : O(1)
 **********************************************/
template <class T>
inline Node <T> * remove(const Node <T> * pRemove) 
{
	if (pRemove == nullptr) //null check
		return nullptr;

	Node<T>* parent = (pRemove->pPrev) ? pRemove->pPrev : pRemove->pNext;

	if (pRemove->pPrev != nullptr) // if there is a previous node
		pRemove->pPrev->pNext = pRemove->pNext;


	if (pRemove->pNext != nullptr)
		pRemove->pNext->pPrev = pRemove->pPrev;

	delete pRemove; //free memory
	return parent;
}


/**********************************************
 * INSERT 
 * Insert a new node the the value in "t" into a linked
 * list immediately before the current position.
 *   INPUT   : t - the value to be used for the new node
 *             pCurrent - a pointer to the node before which
 *                we will be inserting the new node
 *             after - whether we will be inserting after
 *   OUTPUT  : return the newly inserted item
 *   COST    : O(1)
 **********************************************/
template <class T>
inline Node<T>* insert(Node<T>* pCurrent, const T& t, bool after = false)
{
   Node<T>* pNew = new Node<T>(t);

   if (!pCurrent)
      return pNew;

   if (after)
   {
      pNew->pPrev = pCurrent;
      pNew->pNext = pCurrent->pNext;
      if (pCurrent->pNext)
         pCurrent->pNext->pPrev = pNew;
      pCurrent->pNext = pNew;
   }
   else
   {
      pNew->pNext = pCurrent;
      pNew->pPrev = pCurrent->pPrev;
      if (pCurrent->pPrev)
         pCurrent->pPrev->pNext = pNew;
      pCurrent->pPrev = pNew;
   }

   return pNew;
}

/******************************************************
 * SIZE
 * Find the size an unsorted linked list.  
 *  INPUT   : a pointer to the head of the linked list
 *            the value to be found
 *  OUTPUT  : number of nodes
 *  COST    : O(n)
 ********************************************************/
template <class T>
inline size_t size(const Node <T> * pHead)
{
    if (pHead == nullptr)
		return 0;
	return 1 + size(pHead->pNext);
}

/***********************************************
 * DISPLAY
 * Display all the items in the linked list from here on back
 *    INPUT  : the output stream
 *             pointer to the linked list
 *    OUTPUT : the data from the linked list on the screen
 *    COST   : O(n)
 **********************************************/
template <class T>
inline std::ostream & operator << (std::ostream & out, const Node <T> * pHead)
{
	//out << "[";
	//const Node<T>* pCurrent = pHead;
	//while (pCurrent != nullptr)
	//{
	//	out << pCurrent->data;
	//	pCurrent = pCurrent->pNext;
	//	if (pCurrent != nullptr)
	//		out << ", "; // AI autocompleted section
	//}
    return out;
}

/*****************************************************
 * FREE DATA
 * Free all the data currently in the linked list
 *   INPUT   : pointer to the head of the linked list
 *   OUTPUT  : pHead set to NULL
 *   COST    : O(n)
 ****************************************************/
template <class T>
inline void clear(Node <T> * & pHead)
{
   while (pHead != nullptr)
   {
      Node<T> * pDelete = pHead;
      pHead = pHead->pNext;
      delete pDelete;
   }
}