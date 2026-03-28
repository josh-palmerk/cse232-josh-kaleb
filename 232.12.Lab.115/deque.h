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
      clear();
      if (data)
      {
         delete[] data;
         data = nullptr;
      }
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
   T& front()
   {
      assert(numElements > 0);
      int ib = ibFromID(0);
      assert(data[ib] != nullptr);
      return data[ib][icFromID(0)];
   }
   const T& front() const
   {
      assert(numElements > 0);
      int ib = ibFromID(0);
      assert(data[ib] != nullptr);
      return data[ib][icFromID(0)];
   }
   T& back()
   {
      assert(numElements > 0);
      int id = numElements - 1;
      int ib = ibFromID(id);
      assert(data[ib] != nullptr);
      return data[ib][icFromID(id)];
   }
   const T& back() const
   {
      assert(numElements > 0);
      int id = numElements - 1;
      int ib = ibFromID(id);
      assert(data[ib] != nullptr);
      return data[ib][icFromID(id)];
   }
   T& operator[](int id)
   {
      assert(id >= 0 && id < static_cast<int>(numElements));
      int ib = ibFromID(id);
      assert(data[ib] != nullptr);
      return data[ib][icFromID(id)];
   }
   const T& operator[](int id) const
   {
      assert(id >= 0 && id < static_cast<int>(numElements));
      int ib = ibFromID(id);
      assert(data[ib] != nullptr);
      return data[ib][icFromID(id)];
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

   bool isAllBlocksFilled() const;

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
deque<T, A>::deque(deque& rhs): deque()
{
   for (int i = 0; i < rhs.numElements; ++i)
      push_back(rhs[i]);
}

/*****************************************
 * DEQUE :: COPY-ASSIGN
 * Allocate the space for the elements and
 * call the copy constructor on each element
 ****************************************/
template <typename T, typename A>
deque<T, A>& deque<T, A>::operator=(deque& rhs)
{
   if (this == &rhs)
      return *this;

   auto itLHS = begin();
   auto itRHS = rhs.begin();

   // 1. Assign overlapping elements
   while (itLHS != end() && itRHS != rhs.end())
   {
      *itLHS = *itRHS;
      ++itLHS;
      ++itRHS;
   }

   // 2. Erase leftover LHS elements
   while (itLHS != end())
   {
      pop_back();
   }

   // 3. Append leftover RHS elements
   while (itRHS != rhs.end())
   {
      push_back(*itRHS);
      ++itRHS;
   }

   return *this;
}

/*****************************************
 * DEQUE :: PUSH_BACK
 * add an element to the back of the deque
 ****************************************/
template <typename T, typename A>
void deque<T, A>::push_back(const T& t)
{
   // 1. Reallocate the array of blocks as needed
   int icTail = (numElements == 0)
      ? numCells - 1
      : icFromID(numElements - 1);

   if (isAllBlocksFilled() && icTail == numCells - 1)
   {
      int newNumBlocks = (numBlocks == 0 ? 1 : numBlocks * 2);
      reallocate(newNumBlocks);
   }

   // 2. Allocate a new block as needed
   int ib = ibFromID(numElements);
   if (data[ib] == nullptr)
   {
      data[ib] = alloc.allocate(numCells);
   }

   // 3. Assign the value into the block
   alloc.construct(&data[ib][icFromID(numElements)], t);

   numElements++;
}

/*****************************************
 * DEQUE :: PUSH_BACK - move
 * add an element to the back of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_back(T && t)
{
   // 1. Reallocate the array of blocks as needed
   int icTail = (numElements == 0)
      ? numCells - 1
      : icFromID(numElements - 1);

   if (isAllBlocksFilled() && icTail == numCells - 1)
   {
      int newNumBlocks = (numBlocks == 0 ? 1 : numBlocks * 2);
      reallocate(newNumBlocks);
   }

   // 2. Allocate a new block as needed
   int ib = ibFromID(numElements);
   if (data[ib] == nullptr)
   {
      data[ib] = alloc.allocate(numCells);
   }

   // 3. Assign the value into the block
   alloc.construct(&data[ib][icFromID(numElements)], std::move(t));

   numElements++;
}

/*****************************************
 * DEQUE :: PUSH_FRONT
 * add an element to the front of the deque
 ****************************************/
template <typename T, typename A>
void deque<T, A>::push_front(const T& t)
{
   // Handling an empty deque
   if (numElements == 0)
   {
      if (numBlocks == 0)
      {
         numBlocks = 1;
         data = new T * [numBlocks];
         data[0] = nullptr;
      }

      data[0] = alloc.allocate(numCells);

      iaFront = numCells - 1;
      alloc.construct(&data[0][iaFront], t);

      numElements = 1;
      return;
   }

   // 1. Reallocate the array of blocks as needed
   int icFront = icFromID(0);

   if (isAllBlocksFilled() && icFront == 0)
   {
      int newNumBlocks = (numBlocks == 0 ? 1 : numBlocks * 2);
      reallocate(newNumBlocks);
   }

   // 2. Adjust the front array index, wrapping as needed
   if (iaFront != 0)
      iaFront--;
   else
      iaFront = numBlocks * numCells - 1;

   // 3. Allocate a new block as needed
   int ib = ibFromID(0);
   if (data[ib] == nullptr)
   {
      data[ib] = alloc.allocate(numCells);
   }

   // 4. Assign the value into the block
   alloc.construct(&data[ib][icFromID(0)], t);

   numElements++;
}

/*****************************************
 * DEQUE :: PUSH_FRONT - move
 * add an element to the front of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_front(T&& t)
{
   // Handling an empty deque
   if (numElements == 0)
   {
      if (numBlocks == 0)
      {
         numBlocks = 1;
         data = new T * [numBlocks];
         data[0] = nullptr;
      }

      data[0] = alloc.allocate(numCells);

      iaFront = numCells - 1;
      alloc.construct(&data[0][iaFront], std::move(t));

      numElements = 1;
      return;
   }

   // 1. Reallocate the array of blocks as needed
   int icFront = icFromID(0);

   if (isAllBlocksFilled() && icFront == 0)
   {
      int newNumBlocks = (numBlocks == 0 ? 1 : numBlocks * 2);
      reallocate(newNumBlocks);
   }

   // 2. Adjust the front array index, wrapping as needed
   if (iaFront != 0)
      iaFront--;
   else
      iaFront = numBlocks * numCells - 1;

   // 3. Allocate a new block as needed
   int ib = ibFromID(0);
   if (data[ib] == nullptr)
   {
      data[ib] = alloc.allocate(numCells);
   }

   // 4. Assign the value into the block
   alloc.construct(&data[ib][icFromID(0)], std::move(t));

   numElements++;
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
void deque<T, A>::reallocate(int numBlocksNew)
{
   // 1. Allocate a new array of pointers
   T** dataNew = nullptr;
   if (numBlocksNew > 0)
   {
      dataNew = new T * [numBlocksNew];
   }

   // 2. Copy over the pointers, unwrapping as we go
   int ibNew = 0;

   for (int idOld = 0; idOld < static_cast<int>(numElements); idOld += numCells)
   {
      int ibOld = ibFromID(idOld);
      dataNew[ibNew] = data[ibOld];
      ibNew++;
   }

   // 3. Set remaining block pointers to NULL
   while (ibNew < numBlocksNew)
   {
      dataNew[ibNew] = nullptr;
      ibNew++;
   }

   // 4. If front and back share a block but wrap around, move back block
   if (numElements > 0 &&
      ibFromID(0) == ibFromID(numElements - 1) &&
      icFromID(0) > icFromID(numElements - 1))
   {
      int ibFrontOld = ibFromID(0);
      int ibBackOld = ibFromID(numElements - 1);

      int ibBackNew = numElements / numCells;

      dataNew[ibBackNew] = alloc.allocate(numCells);

      int icBackEnd = icFromID(numElements - 1);
      for (int ic = 0; ic <= icBackEnd; ++ic)
      {
         alloc.construct(&dataNew[ibBackNew][ic],
            std::move(data[ibBackOld][ic]));
         alloc.destroy(&data[ibBackOld][ic]);
      }

      alloc.deallocate(data[ibBackOld], numCells);
   }

   // 5. Replace old data with new data
   if (data)
   {
      delete[] data;
   }

   data = dataNew;
   numBlocks = numBlocksNew;

   iaFront = iaFront % numCells;
}

/*****************************************
 * DEQUE :: IS ALL BLOCKS FILLED?
 * return TRUE if all the blocks are filled
 ****************************************/
template <typename T, typename A>
bool deque <T, A> ::isAllBlocksFilled() const
{
   // We have no choice but to check each block looking for a NULLPTR
   for (size_t ib = 0; ib < numBlocks; ib++)
      if (nullptr == data[ib])
         return false;
   return true;
}

} // namespace custom
