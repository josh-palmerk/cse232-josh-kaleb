/***********************************************************************
 * Header:
 *    DEQUE
 * Summary:
 *    Our custom implementation of a deque 
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        deque                 : A class that represents a deque
 *        deque::iterator       : An iterator through a deque
 * Author
 *    Josh, Kaleb, Spencer
 ************************************************************************/

#pragma once

// Debug stuff
#include <cassert>
#include <memory>   // for std::allocator

class TestDeque;    // forward declaration for TestDeque unit test class

namespace custom
{

/******************************************************
 * DEQUE
 *****************************************************/
template <typename T, typename A = std::allocator<T>>
class deque
{
   friend class ::TestDeque; // give unit tests access to the privates
public:

   // 
   // Construct
   //
   deque(const A & a = A()) 
   { 
       data = nullptr;
       numCells = 16;        
       numBlocks = 0;
       numElements = 0;
       iaFront = 0;
   }
   deque(deque & rhs);
   ~deque()
   {
   }

   //
   // Assign
   //
   deque & operator = (deque& rhs);

   // 
   // Iterator
   //
   class iterator;
   iterator begin() 
   { 
      return iterator(0, this); 
   }
   iterator end()   
   { 
      return iterator(numElements, this); 
   }

   // 
   // Access
   //
   T & front()       
   { 
      return *(new T);
   }
   const T & front() const 
   {
      return *(new T);
   }
   T & back()
   {
      return *(new T);
   }
   const T & back() const
   {
      return *(new T);
   }
   T & operator[](int id)
   {
      return data[ibFromID(id)][icFromID(id)];
   }
   const T & operator[](int id) const
   {
      return data[ibFromID(id)][icFromID(id)];
   }

   //
   // Insert
   //
   void push_back(const T & t);
   void push_back(T && t);
   void push_front(const T& t);
   void push_front(T&& t);

   //
   // Remove
   //
   void pop_front();
   void pop_back();
   void clear();

   //
   // Status
   //
   size_t size()  const { return numElements; }
   bool   empty() const { return numElements == 0; }
   
private:
   // array index from deque index
   int iaFromID(int id) const
   {
      return (iaFront + id) % (numCells * numBlocks); // (iafront + id) % capacity
   }

   // block index from deque index
   int ibFromID(int id) const
   {
      return iaFromID(id) / numCells;
   }

   // cell index from deque index
   int icFromID(int id) const
   {
       return iaFromID(id) % numCells;
   }

   // reallocate
   void reallocate(int numBlocksNew);

   A    alloc;                // use alloacator for memory allocation
   size_t numCells;           // number of cells in a block
   size_t numBlocks;          // number of blocks in the data array
   size_t numElements;        // number of elements in the deque
   int iaFront;               // array-centered index of the front of the deque
   T ** data;                 // array of arrays
};

/**************************************************
 * DEQUE ITERATOR
 * An iterator through deque.  You only need to
 * support the following:
 *   1. Constructors (default and copy)
 *   2. Not equals operator
 *   3. Increment (prefix and postfix)
 *   4. Dereference
 * This particular iterator is a bi-directional meaning
 * that ++ and -- both work.  Not all iterators are that way.
 *************************************************/
template <typename T, typename A>
class deque <T, A> ::iterator
{
   friend class ::TestDeque; // give unit tests access to the privates
public:
   // 
   // Construct
   //
   iterator() 
   {
   }
   iterator(int id, deque* d) 
   {
      this->id = id;
      this->d = d;
   }
   iterator(const iterator& rhs) 
   {
      this->id = rhs.id;
      this->d = rhs.d;
   }

   //
   // Assign
   //
   iterator& operator = (const iterator& rhs)
   {
	   this->id = rhs.id;
	   this->d = rhs.d;
      return *this;
   }

   // 
   // Compare
   //
   bool operator != (const iterator& rhs) const { return this->id != rhs.id || this->d != rhs.d; }
   bool operator == (const iterator& rhs) const { return this->id == rhs.id && this->d == rhs.d; }

   // 
   // Access
   //
   T& operator * ()
   {
      return (*d)[id];
   }

   // 
   // Arithmetic
   //
   int operator - (iterator it) const
   {
      return id - it.id;
   }
   iterator& operator += (int offset)
   {
      id += offset;
      return *this;
   }
   iterator& operator ++ ()
   {
      ++id;
      return *this;
   }
   iterator operator ++ (int postfix)
   {
      iterator temp = *this;
      ++id;
      return temp;
   }
   iterator& operator -- ()
   {
      --id;
      return *this;
   }
   iterator operator -- (int postfix)
   {
      iterator temp = *this;
      --id;
      return temp;
   }

private:
   int id;
   deque* d;
};

/*****************************************
 * DEQUE :: COPY CONSTRUCTOR
 * Allocate the space for the elements and
 * call the copy constructor on each element
 ****************************************/
template <typename T, typename A>
deque <T, A> ::deque(deque& rhs) 
{

}

/*****************************************
 * DEQUE :: COPY-ASSIGN
 * Allocate the space for the elements and
 * call the copy constructor on each element
 ****************************************/
template <typename T, typename A>
deque <T, A> & deque <T, A> :: operator = (deque & rhs)
{
   return *this;
}

/*****************************************
 * DEQUE :: PUSH_BACK
 * add an element to the back of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_back(const T& t)
{
}

/*****************************************
 * DEQUE :: PUSH_BACK - move
 * add an element to the back of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_back(T && t)
{

}

/*****************************************
 * DEQUE :: PUSH_FRONT
 * add an element to the front of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_front(const T& t)
{
}

/*****************************************
 * DEQUE :: PUSH_FRONT - move
 * add an element to the front of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_front(T&& t)
{
}

/*****************************************
 * DEQUE :: CLEAR
 * Remove all the elements from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::clear()
{
   for (int id = 0; id < numElements; id++)
   {
      alloc.destroy(&data[ibFromID(id)][icFromID(id)]);
   }
   for (int ib = 0; ib < numBlocks; ib++)
   {
      if (data[ib] != nullptr)
      {
         alloc.deallocate(data[ib], numCells);
         data[ib] = nullptr;
      }
   }
   numElements = 0;
}

/*****************************************
 * DEQUE :: POP FRONT
 * Remove the front element from a deque
 ****************************************/
template <typename T, typename A>
void deque<T, A>::pop_front()
{
   // id of the element to remove (always 0 for pop_front)
   int idRemove = 0;

   // destroy the front element
   alloc.destroy(&data[ibFromID(idRemove)][icFromID(idRemove)]);

   // determine whether the block should be deleted
   bool deleteBlock =
      (numElements == 1) ||
      (icFromID(idRemove) == numCells - 1 && ibFromID(idRemove) != ibFromID(numElements - 1));

   if (deleteBlock)
   {
      int ib = ibFromID(idRemove);

      // deallocate the block using allocator
      alloc.deallocate(data[ib], numCells);

      // mark the block as empty
      data[ib] = nullptr;
   }

   // advance the front index
   iaFront = (iaFront + 1) % (numCells * numBlocks);

   // one fewer element now
   numElements--;
}

/*****************************************
 * DEQUE :: POP BACK
 * Remove the back element from a deque
 ****************************************/
template <typename T, typename A>
void deque<T, A>::pop_back()
{
   // id of the element to remove
   int idRemove = numElements - 1;

   // destroy the element at the back
   alloc.destroy(&data[ibFromID(idRemove)][icFromID(idRemove)]);

   // determine whether the block should be deleted
   bool deleteBlock =
      (numElements == 1) ||
      (icFromID(idRemove) == 0 && ibFromID(idRemove) != ibFromID(0));

   if (deleteBlock)
   {
      int ib = ibFromID(idRemove);

      // deallocate the block using allocator
      alloc.deallocate(data[ib], numCells);

      // mark the block as empty
      data[ib] = nullptr;
   }

   // one fewer element now
   numElements--;
}

/*****************************************
 * DEQUE :: REALLOCATE
 * Remove all the elements from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> :: reallocate(int numBlocksNew)
{

}

} // namespace custom
