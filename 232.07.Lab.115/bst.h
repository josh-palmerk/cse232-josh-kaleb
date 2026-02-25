/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        BST                 : A class that represents a binary search tree
 *        BST::iterator       : An iterator through BST
 * Author
 *    Josh, Kaleb, Spencer
 ************************************************************************/

#pragma once

#ifdef DEBUG
#define debug(x) x
#else // !DEBUG
#define debug(x)
#endif // !DEBUG

#include <cassert>          // because I am paranoid
#include <memory>           // for std::allocator
#include <functional>       // for std::less
#include <utility>          // for std::pair
#include <initializer_list> // for the initializer list, of course!

class TestBST; // forward declaration for unit tests
class TestSet;
class TestMap;

namespace custom
{

   template <typename TT>
   class set;
   template <typename KK, typename VV>
   class map;

/*****************************************************************
 * BINARY SEARCH TREE
 * Create a Binary Search Tree
 *****************************************************************/
template <typename T>
class BST
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestSet;
   friend class ::TestMap;

   template <class TT>
   friend class custom::set;

   template <class KK, class VV>
   friend class custom::map;
public:
   //
   // Construct
   //

   BST();
   BST(const BST &  rhs);
   BST(      BST && rhs);
   BST(const std::initializer_list<T>& il);
   ~BST();

   //
   // Assign
   //

   BST & operator = (const BST &  rhs);
   BST & operator = (      BST && rhs);
   BST & operator = (const std::initializer_list<T>& il);
   void swap(BST & rhs);

   //
   // Iterator
   //

   class iterator;
   iterator   begin() const noexcept;
   iterator   end()   const noexcept { return iterator(nullptr); }

   //
   // Access
   //

   iterator find(const T& t);

   // 
   // Insert
   //

   std::pair<iterator, bool> insert(const T&  t, bool keepUnique = false);
   std::pair<iterator, bool> insert(      T&& t, bool keepUnique = false);

   //
   // Remove
   // 

   iterator erase(iterator& it);
   void   clear() noexcept;



   // 
   // Status
   //

   bool   empty() const noexcept { return root == nullptr; }
   size_t size()  const noexcept { return numElements;     }
   
private:
   class BNode;
   void clear(BNode* node);

   BNode * root;              // root node of the binary search tree
   size_t numElements;        // number of elements currently in the tree
};


/*****************************************************************
 * BINARY NODE
 * A single node in a binary tree. Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <typename T>
class BST <T> :: BNode
{
public:
   // 
   // Construct
   //
	BNode() : data(), pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(true)
   {

   }

   BNode(const T& t) : data(t), pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(true)
   {

   }


   BNode(T&& t) : data(std::move(t)), pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(true)
   {

   }

   //
   // Insert
   //
   void addLeft (BNode * pNode);
   void addRight(BNode * pNode);
   void addLeft (const T &  t);
   void addRight(const T &  t);
   void addLeft (      T && t);
   void addRight(      T && t);

   // 
   // Status
   //
   bool isRightChild(BNode* pNode) const { return pNode->pParent && pNode == pNode->pParent->pRight; }
   bool isLeftChild( BNode* pNode) const { return pNode->pParent && pNode == pNode->pParent->pLeft;  }

   // balance the tree
   void balance();

   // rotate nodes
   void rotateLeft( BNode* pNode);
   void rotateRight(BNode* pNode);

#ifdef DEBUG
   //
   // Verify
   //
   std::pair <T,T> verifyBTree() const;
   int findDepth() const;
   bool verifyRedBlack(int depth) const;
   int computeSize() const;
#endif // DEBUG

   //
   // Data
   //
   T data;                  // Actual data stored in the BNode
   BNode* pLeft;          // Left child - smaller
   BNode* pRight;         // Right child - larger
   BNode* pParent;        // Parent
   bool isRed;              // Red-black balancing stuff
};

/**********************************************************
 * BINARY SEARCH TREE ITERATOR
 * Forward and reverse iterator through a BST
 *********************************************************/
template <typename T>
class BST <T> :: iterator
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestSet;
   friend class ::TestMap;

   template <class KK, class VV>
   friend class custom::map;
public:
   // constructors and assignment
   iterator(BNode * p = nullptr)          
   { 
	   pNode = p;
   }
   iterator(const iterator & rhs)         
   { 
	   pNode = rhs.pNode;
   }
   iterator & operator = (const iterator & rhs)
   {
      pNode = rhs.pNode;
	   return *this;
   }

   // compare
   bool operator == (const iterator & rhs) const
   {
	  return pNode == rhs.pNode;
   }
   bool operator != (const iterator & rhs) const
   {
	   return pNode != rhs.pNode;
   }

   // de-reference. Cannot change because it will invalidate the BST
   const T & operator * () const 
   {
	   assert(pNode != nullptr);
	   return pNode->data;
   }

   // increment and decrement
   iterator & operator ++ ();
   iterator   operator ++ (int postfix)
   {
      iterator temp = *this;
      if (pNode != nullptr)
      {
         ++(*this);
      }
      return temp;
   }
   iterator & operator -- ();
   iterator   operator -- (int postfix)
   {
      iterator temp = *this;
      if (pNode != nullptr)
      {
         --(*this);
      }
      return temp;
   }

   // must give friend status to remove so it can call getNode() from it
   friend BST <T> :: iterator BST <T> :: erase(iterator & it);

private:
   
    // the node
    BNode * pNode;
};


/*********************************************
 *********************************************
 *********************************************
 ******************** BST ********************
 *********************************************
 *********************************************
 *********************************************/


 /*********************************************
  * BST :: DEFAULT CONSTRUCTOR
  ********************************************/
template <typename T>
BST <T> :: BST()
{
   numElements = 0;
	root = nullptr;
}

/*********************************************
 * BST :: COPY CONSTRUCTOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST(const BST<T>& rhs) : numElements(rhs.numElements), root(nullptr)
{
   *this = rhs;
}

/*********************************************
 * BST :: MOVE CONSTRUCTOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST(BST <T> && rhs) 
{
   numElements = std::move(rhs.numElements);
   root = std::move(rhs.root);
	rhs.numElements = 0;
	rhs.root = nullptr;
}

/*********************************************
 * BST :: INITIALIZER LIST CONSTRUCTOR
 * Create a BST from an initializer list
 ********************************************/
template <typename T>
BST <T> ::BST(const std::initializer_list<T>& il)
{
	root = nullptr;
	numElements = 0;
   for (const T& t : il)
   {
      insert(t);
	}
}

/*********************************************
 * BST :: DESTRUCTOR
 ********************************************/
template <typename T>
BST <T> :: ~BST()
{
   clear();
}

/*********************************************
 * BST :: ASSIGNMENT OPERATOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const BST <T> & rhs)
{
   if (this == &rhs)
      return *this;

   std::function<void(BNode*&, const BNode*, BNode*)> assign = [&](BNode*& dest, const BNode* src, BNode* parent)
   {
      if (src && dest) // both exist → assign
      {
         dest->data = src->data;
         dest->isRed = src->isRed;
         dest->pParent = parent;

         assign(dest->pLeft,  src->pLeft,  dest);
         assign(dest->pRight, src->pRight, dest);
      }
      else if (src && !dest) // src but no dest, need a new node
      {
         dest = new BNode(src->data);
         dest->isRed = src->isRed;
         dest->pParent = parent;

         assign(dest->pLeft,  src->pLeft,  dest);
         assign(dest->pRight, src->pRight, dest);
      }
      else if (!src && dest) // delete subtree
      {
         // Recursively delete children first
         if (dest->pLeft)
            assign(dest->pLeft, nullptr, dest);
         if (dest->pRight)
            assign(dest->pRight, nullptr, dest);

         delete dest;
         dest = nullptr;
      }
   };

   assign(root, rhs.root, nullptr);
   numElements = rhs.numElements;

   return *this;
}

/*********************************************
 * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
 * Copy nodes onto a BTree
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const std::initializer_list<T>& il)
{
   clear();
   for (const T& value : il)
   {
      insert(value);
   }
   return *this;
}

/*********************************************
 * BST :: ASSIGN-MOVE OPERATOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (BST <T> && rhs)
{
    if (this != &rhs)
   {
      clear();
      numElements = rhs.numElements;
      root = rhs.root;
      rhs.numElements = 0;
      rhs.root = nullptr;
   }
	return *this;
}

/*********************************************
 * BST :: SWAP
 * Swap two trees
 ********************************************/
template <typename T>
void BST <T> :: swap (BST <T>& rhs)
{
   std::swap(root, rhs.root);
	std::swap(numElements, rhs.numElements);
}

/*****************************************************
 * BST :: INSERT
 * Insert a node at a given location in the tree
 ****************************************************/
template <typename T>
std::pair<typename BST <T> :: iterator, bool> BST <T> :: insert(const T & t, bool keepUnique)
{
   if (root == nullptr)
   {
      root = new BNode(t);
      root->isRed = false; // root is always black
      ++numElements;
      return std::make_pair(iterator(root), true);
   }

   BNode* current = root;
   BNode* parent = nullptr;
   bool goLeft = true;

   while (current != nullptr)
   {
      parent = current;

      if (keepUnique && t == current->data)
         return std::make_pair(iterator(current), false);

      if (t < current->data)
      {
         goLeft = true;
         current = current->pLeft;
      }
      else
      {
         goLeft = false;
         current = current->pRight;
      }
   }

   BNode* newNode = new BNode(t);
   newNode->isRed = true;
   newNode->pParent = parent;

   if (goLeft)
      parent->pLeft = newNode;
   else
      parent->pRight = newNode;

   ++numElements;

   // Balance the tree after insertion
   newNode->balance();

   // Update root if necessary
   while (root->pParent)
      root = root->pParent;

   root->isRed = false;

   return std::make_pair(iterator(newNode), true);
}

template <typename T>
std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(T&& t, bool keepUnique)
{
   if (root == nullptr)
   {
      root = new BNode(std::move(t));
      root->balance();
      ++numElements;
      return std::make_pair(iterator(root), true);
   }

   BNode* current = root;
   BNode* parent = nullptr;
   bool goLeft = true;

   while (current != nullptr)
   {
      parent = current;

      if (keepUnique && t == current->data)
         return std::make_pair(iterator(current), false);

      if (t < current->data)
      {
         goLeft = true;
         current = current->pLeft;
      }
      else
      {
         goLeft = false;
         current = current->pRight;
      }
   }

   BNode* newNode = new BNode(std::move(t));
   newNode->isRed = true;
   newNode->pParent = parent;

   if (goLeft)
      parent->addLeft(newNode);
   else
      parent->addRight(newNode);
   ++numElements;
   
   newNode->pParent = parent;

   // Balance the tree after insertion
   newNode->balance();

   // Update root if necessary
   while (root->pParent)
      root = root->pParent;

   root->balance();

   return std::make_pair(iterator(newNode), true);
}

/*************************************************
 * BST :: ERASE
 * Remove a given node as specified by the iterator
 ************************************************/
template <typename T>
typename BST<T>::iterator BST<T>::erase(iterator& it)
{
   if (it == end())
      return end();

   BNode* pToRemove = it.pNode;
   BNode* pResult = nullptr;

   // Case 1: two children
   if (pToRemove->pLeft && pToRemove->pRight)
   {
      // find in-order successor (leftmost of right subtree)
      BNode* succ = pToRemove->pRight;
      while (succ->pLeft)
         succ = succ->pLeft;

      // detach succ from its current position
      BNode* succParent = succ->pParent;
      BNode* succChild = succ->pRight; // succ always has no left child

      if (succParent->pLeft == succ)
         succParent->pLeft = succChild;
      else
         succParent->pRight = succChild;

      if (succChild)
         succChild->pParent = succParent;

      // now move succ into node's position
      succ->pParent = pToRemove->pParent;
      succ->pLeft = pToRemove->pLeft;
      succ->pRight = (pToRemove->pRight == succ ? succChild : pToRemove->pRight);

      if (succ->pLeft)
         succ->pLeft->pParent = succ;
      if (succ->pRight)
         succ->pRight->pParent = succ;

      if (!pToRemove->pParent)
         root = succ;
      else if (pToRemove->pParent->pLeft == pToRemove)
         pToRemove->pParent->pLeft = succ;
      else
         pToRemove->pParent->pRight = succ;

      pResult = succ;
   }


   // Case 2: one child
   else if (pToRemove->pLeft || pToRemove->pRight)
   {
      BNode* child = pToRemove->pLeft ? pToRemove->pLeft : pToRemove->pRight;

      // reattach child to parent
      child->pParent = pToRemove->pParent;

      if (!pToRemove->pParent)
         root = child;
      else if (pToRemove->isLeftChild(pToRemove))
         pToRemove->pParent->pLeft = child;
      else
         pToRemove->pParent->pRight = child;

      // iterator should point to in-order successor
      pResult = child;
      while (pResult->pLeft)
         pResult = pResult->pLeft;
   }
   else
   {
      // Case 3: leaf
      if (!pToRemove->pParent)
         root = nullptr;
      else if (pToRemove->isLeftChild(pToRemove))
         pToRemove->pParent->pLeft = nullptr;
      else
         pToRemove->pParent->pRight = nullptr;
      pResult = pToRemove->pParent;
   }
   --numElements;
   delete pToRemove;
   pToRemove = nullptr;
   pResult->balance();
   return iterator(pResult);
}

/*****************************************************
 * BST :: CLEAR
 * Removes all the BNodes from a tree
 ****************************************************/
template <typename T>
void BST <T> ::clear() noexcept
{
   clear(root);
   root = nullptr;
   numElements = 0;
}

/*****************************************************
 * BST :: CLEAR (PRIVATE)
 * Helper function to recursively clear a tree from a given node
 ****************************************************/
template <typename T>
void BST<T>::clear(BNode* node)
{
   if (!node)
      return;

   clear(node->pLeft);
   clear(node->pRight);
   delete node;
}


/*****************************************************
 * BST :: BEGIN
 * Return the first node (left-most) in a binary search tree
 ****************************************************/
template <typename T>
typename BST <T> :: iterator custom :: BST <T> :: begin() const noexcept
{
   if (root == nullptr)
	   return end();
   BNode* pCurrent = root;
   while (pCurrent->pLeft != nullptr)
	   pCurrent = pCurrent->pLeft;
   return iterator(pCurrent);
}


/****************************************************
 * BST :: FIND
 * Return the node corresponding to a given value
 ****************************************************/
template <typename T>
typename BST <T> :: iterator BST<T> :: find(const T & t)
{
   BNode* pCurrent = root;
   while (pCurrent != nullptr)
   {
      if (t == pCurrent->data)
         return iterator(pCurrent);
      else if (t < pCurrent->data)
         pCurrent = pCurrent->pLeft;
      else
         pCurrent = pCurrent->pRight;
   }
   return end();
}

/******************************************************
 ******************************************************
 ******************************************************
 *********************** B NODE ***********************
 ******************************************************
 ******************************************************
 ******************************************************/

 
/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addLeft (BNode * pNode)
{
   pLeft = pNode;
   pNode->pParent = this;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (BNode * pNode)
{
   pRight = pNode;
   pNode->pParent = this;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> :: BNode :: addLeft (const T & t)
{
   pLeft = new BNode(t);
   pLeft->pParent = this;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> ::BNode::addLeft(T && t)
{
   pLeft = new BNode(std::move(t));
   pLeft->pParent = this;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (const T & t)
{
   pRight = new BNode(t);
   pRight->pParent = this;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> ::BNode::addRight(T && t)
{
   pRight = new BNode(std::move(t));
   pRight->pParent = this;
}

#ifdef DEBUG
/****************************************************
 * BINARY NODE :: FIND DEPTH
 * Find the depth of the black nodes. This is useful for
 * verifying that a given red-black tree is valid
 ****************************************************/
template <typename T>
int BST <T> :: BNode :: findDepth() const
{
   // if there are no children, the depth is ourselves
   if (pRight == nullptr && pLeft == nullptr)
      return (isRed ? 0 : 1);

   // if there is a right child, go that way
   if (pRight != nullptr)
      return (isRed ? 0 : 1) + pRight->findDepth();
   else
      return (isRed ? 0 : 1) + pLeft->findDepth();
}

/****************************************************
 * BINARY NODE :: VERIFY RED BLACK
 * Do all four red-black rules work here?
 ***************************************************/
template <typename T>
bool BST <T> :: BNode :: verifyRedBlack(int depth) const
{
   bool fReturn = true;
   depth -= (isRed == false) ? 1 : 0;

   // Rule a) Every node is either red or black
   assert(isRed == true || isRed == false); // this feels silly

   // Rule b) The root is black
   if (pParent == nullptr)
      if (isRed == true)
         fReturn = false;

   // Rule c) Red nodes have black children
   if (isRed == true)
   {
      if (pLeft != nullptr)
         if (pLeft->isRed == true)
            fReturn = false;

      if (pRight != nullptr)
         if (pRight->isRed == true)
            fReturn = false;
   }

   // Rule d) Every path from a leaf to the root has the same # of black nodes
   if (pLeft == nullptr && pRight && nullptr)
      if (depth != 0)
         fReturn = false;
   if (pLeft != nullptr)
      if (!pLeft->verifyRedBlack(depth))
         fReturn = false;
   if (pRight != nullptr)
      if (!pRight->verifyRedBlack(depth))
         fReturn = false;

   return fReturn;
}


/******************************************************
 * VERIFY B TREE
 * Verify that the tree is correctly formed
 ******************************************************/
template <typename T>
std::pair <T, T> BST <T> :: BNode :: verifyBTree() const
{
   // largest and smallest values
   std::pair <T, T> extremes;
   extremes.first = data;
   extremes.second = data;

   // check parent
   if (pParent)
      assert(pParent->pLeft == this || pParent->pRight == this);

   // check left, the smaller sub-tree
   if (pLeft)
   {
      assert(!(data < pLeft->data));
      assert(pLeft->pParent == this);
      pLeft->verifyBTree();
      std::pair <T, T> p = pLeft->verifyBTree();
      assert(!(data < p.second));
      extremes.first = p.first;

   }

   // check right
   if (pRight)
   {
      assert(!(pRight->data < data));
      assert(pRight->pParent == this);
      pRight->verifyBTree();

      std::pair <T, T> p = pRight->verifyBTree();
      assert(!(p.first < data));
      extremes.second = p.second;
   }

   // return answer
   return extremes;
}

/*********************************************
 * COMPUTE SIZE
 * Verify that the BST is as large as we think it is
 ********************************************/
template <typename T>
int BST <T> :: BNode :: computeSize() const
{
   return 1 +
      (pLeft  == nullptr ? 0 : pLeft->computeSize()) +
      (pRight == nullptr ? 0 : pRight->computeSize());
}
#endif // DEBUG

/******************************************************
 * BINARY NODE :: BALANCE
 * Balance the tree from a given location
 ******************************************************/
template <typename T>
void BST<T>::BNode::balance()
{
   BNode* current = this;

   // Case 1: if we are the root, just make us black
   if (!current->pParent)
   {
      current->isRed = false;
      return;
   }

   while (current->pParent && current->pParent->isRed)
   {
      BNode* parent      = current->pParent;
      BNode* grandparent = parent->pParent;
      if (!grandparent)
         break; // safety guard

      // Parent is left child of grandparent
      if (parent == grandparent->pLeft)
      {
         BNode* aunt = grandparent->pRight;

         // Case 3: uncle is red
         if (aunt && aunt->isRed)
         {
            parent->isRed      = false;
            aunt->isRed       = false;
            grandparent->isRed = true;
            current = grandparent;
         }
         else
         {
            // Case 4a: current is right child -> rotate parent left
            if (current == parent->pRight)
            {
               current = parent;
               rotateLeft(current);
               parent      = current->pParent;
               grandparent = parent->pParent;
            }

            // Case 4c: current is left child -> rotate grandparent right
            rotateRight(grandparent);
            parent->isRed = false;
            if (grandparent)
               grandparent->isRed = true;
         }
      }
      else // Parent is right child of grandparent (mirror)
      {
         BNode* aunt = grandparent->pLeft;

         // Case 3 mirror
         if (aunt && aunt->isRed)
         {
            parent->isRed      = false;
            aunt->isRed       = false;
            grandparent->isRed = true;
            current = grandparent;
         }
         else
         {
            // Case 4b: current is left child -> rotate parent right
            if (current == parent->pLeft)
            {
               current = parent;
               rotateRight(current);
               parent      = current->pParent;
               grandparent = parent->pParent;
            }

            // Case 4d: current is right child -> rotate grandparent left
            rotateLeft(grandparent);
            parent->isRed = false;
            if (grandparent)
               grandparent->isRed = true;
         }
      }
   }
}

/*************************************************
* ROTATE LEFT
* Rotate a subtree left around a given node
**************************************************/
template <typename T>
void BST<T>::BNode::rotateLeft(BNode* pPivot)
{
   BNode* pChild = pPivot->pRight;
   if (!pChild) return;

   // Move child's left subtree to pivot's right
   pPivot->pRight = pChild->pLeft;
   if (pChild->pLeft)
      pChild->pLeft->pParent = pPivot;

   // Attach child to pivot's parent
   pChild->pParent = pPivot->pParent;
   if (pPivot->pParent)
   {
      if (pPivot == pPivot->pParent->pLeft)
         pPivot->pParent->pLeft = pChild;
      else
         pPivot->pParent->pRight = pChild;
   }

   // Put pivot on child's left
   pChild->pLeft = pPivot;
   pPivot->pParent = pChild;
}


/*************************************************
* ROTATE RIGHT
* Rotate a subtree right around a given node
* ************************************************/
template <typename T>
void BST<T>::BNode::rotateRight(BNode* pPivot)
{
   BNode* pChild = pPivot->pLeft;
   if (!pChild) return;

   // Move child's right subtree to pivot's left
   pPivot->pLeft = pChild->pRight;
   if (pChild->pRight)
      pChild->pRight->pParent = pPivot;

   // Attach child to pivot's parent
   pChild->pParent = pPivot->pParent;
   if (pPivot->pParent)
   {
      if (pPivot == pPivot->pParent->pLeft)
         pPivot->pParent->pLeft = pChild;
      else
         pPivot->pParent->pRight = pChild;
   }

   // Put pivot on child's right
   pChild->pRight = pPivot;
   pPivot->pParent = pChild;
}


/*************************************************
 *************************************************
 *************************************************
 ****************** ITERATOR *********************
 *************************************************
 *************************************************
 *************************************************/     

/**************************************************
 * BST ITERATOR :: INCREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator ++ ()
{
   if (!pNode)
      return *this;
   // Case 1
   if (pNode->pRight)
   {
      pNode = pNode->pRight;
      while (pNode->pLeft)
         pNode = pNode->pLeft;
   }
   else
   {
      BNode* parent = pNode->pParent;
      // Case 3
      while (parent && pNode == parent->pRight)
      {
         pNode = parent;
         parent = parent->pParent;
      }
      // By residing without the while loop, this also acomplishes Case 2
      pNode = parent;
   }
   return *this;  
}

/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator -- ()
{
   if (!pNode)
      return *this;
   // Reverse Case 1
   if (pNode->pLeft)
   {
      pNode = pNode->pLeft;
      while (pNode->pRight)
         pNode = pNode->pRight;
   }
   else
   {
      BNode* parent = pNode->pParent;
      // Reverse Case 3
      while (parent && pNode == parent->pLeft)
      {
         pNode = parent;
         parent = parent->pParent;
      }
      // Reverse Case 2 (and 3 still as well)
      pNode = parent;
   }
   return *this;
}

} // namespace custom


