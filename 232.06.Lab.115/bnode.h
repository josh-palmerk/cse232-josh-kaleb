/***********************************************************************
 * Header:
 *    BINARY NODE
 * Summary:
 *    One node in a binary tree (and the functions to support them).
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        BNode         : A class representing a BNode
 *    Additionally, it will contain a few functions working on Node
 * Author
 *    Josh, Kaleb, Spencer
 ************************************************************************/

#pragma once

#include <iostream>  // for OFSTREAM
#include <cassert>

/*****************************************************************
 * BNODE
 * A single node in a binary tree.  Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <class T>
class BNode
{
public:
   // 
   // Construct
   //
   BNode()
   {
       pLeft = pRight = pParent = nullptr;
   }
   BNode(const T &  t) 
   {
	   this->data = t;
       pLeft = pRight = pParent = nullptr; //something still wrong here
       
   }
   BNode(T && t) 
   {
	   this->data = std::move(t);
       pLeft = pRight = pParent = nullptr;
   }

   //
   // Data
   //
   BNode <T> * pLeft;
   BNode <T> * pRight;
   BNode <T> * pParent;
   T data;
};

/*******************************************************************
 * SIZE BTREE
 * Return the size of a b-tree under the current node
 *******************************************************************/
template <class T>
inline size_t size(const BNode <T> * p)
{
    if (p == nullptr)
        return 0;
    else
		return 1 + size(p->pLeft) + size(p->pRight);
}


/******************************************************
 * ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <class T>
inline void addLeft(BNode <T> * pNode, BNode <T> * pAdd)
{
    if (pNode == nullptr || pAdd == nullptr)
		return;
    pNode->pLeft = pAdd;
	pAdd->pParent = pNode;
}

/******************************************************
 * ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <class T>
inline void addRight (BNode <T> * pNode, BNode <T> * pAdd)
{

}

/******************************************************
 * ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <class T>
inline void addLeft (BNode <T> * pNode, const T & t) 
{
	if (pNode == nullptr)
		return;
	BNode<T>* pAdd = new BNode<T>(t);
	pNode->pLeft = pAdd;
	pAdd->pParent = pNode;

}

template <class T>
inline void addLeft(BNode <T>* pNode, T && t)
{

}

/******************************************************
 * ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <class T>
void addRight (BNode <T> * pNode, const T & t)
{

}

template <class T>
void addRight(BNode <T>* pNode, T && t)
{

}

/*****************************************************
 * DELETE BINARY TREE
 * Delete all the nodes below pThis including pThis
 * using postfix traverse: LRV
 ****************************************************/
template <class T>
void clear(BNode <T> * & pThis)
{

}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(BNode <T>*& pLHS, BNode <T>*& pRHS)
{

}

/**********************************************
 * COPY BINARY TREE
 * Copy pSrc->pRight to pDest->pRight and
 * pSrc->pLeft onto pDest->pLeft
 *********************************************/
template <class T>
BNode <T> * copy(const BNode <T> * pSrc) 
{
   return new BNode<T>;
}

/**********************************************
 * assign
 * copy the values from pSrc onto pDest preserving
 * as many of the nodes as possible.
 *********************************************/
template <class T>
void assign(BNode <T> * & pDest, const BNode <T>* pSrc)
{
	// If the source is null, then we need to clear the destination
	if (pSrc == nullptr)
	{
		clear(pDest);
		return;
	}
	// If the destination is null, then we need to create a new node
	if (pDest == nullptr)
	{
		pDest = new BNode<T>(pSrc->data);
	}
	else   // use existing pDest
	{ 
		pDest->data = pSrc->data;
	}

	// Recursive ops on children
	assign(pDest->pLeft, pSrc->pLeft);
	assign(pDest->pRight, pSrc->pRight);

	//properly assign parent pointers
	if (pDest->pLeft)
		pDest->pLeft->pParent = pDest;
	if (pDest->pRight)
		pDest->pRight->pParent = pDest;


}