/***********************************************************************
 * Header:
 *    HASH
 * Summary:
 *    Our custom implementation of std::unordered_Set
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        unordered_set           : A class that represents a hash
 *        unordered_set::iterator : An interator through hash
 * Author
 *    Josh, Kaleb, Spencer
 ************************************************************************/

#pragma once

#include "list.h"     // because this->buckets[0] is a list
#include <memory>     // for std::allocator
#include <functional> // for std::hash
#include <cmath>      // for std::ceil
#include <utility>    // for std::swap
   

class TestHash;             // forward declaration for Hash unit tests

namespace custom
{
/************************************************
 * UNORDERED SET
 * A set implemented as a hash
 ************************************************/
template <typename T>
class unordered_set
{
   friend class ::TestHash;   // give unit tests access to the privates
public:
   //
   // Construct
   //
   unordered_set() : numElements(0)
   {
   }
   unordered_set(unordered_set&  rhs) : numElements(0)
   {
      for (size_t i = 0; i < 10; ++i)
      {
         for (auto it = rhs.buckets[i].begin(); it != rhs.buckets[i].end(); ++it)
         {
            buckets[i].push_back(*it);
            ++numElements;
         }
      }
   }
   unordered_set(unordered_set&& rhs) : numElements(0)
   {
      swap(rhs);
   }
   template <class Iterator>
   unordered_set(Iterator first, Iterator last) : numElements(0)
   {
      for (auto it = first; it != last; ++it)
         insert(*it);
   }

   //
   // Assign
   //
   unordered_set& operator=(unordered_set& rhs)
   {
      this->numElements = rhs.numElements;
      for (size_t i = 0; i < 10; ++i)
         {
         this->buckets[i].clear();
         for (auto it = rhs.buckets[i].begin(); it != rhs.buckets[i].end(); ++it)
         {
            this->buckets[i].push_back(*it);
         }
      }
      return *this;
   }
   unordered_set& operator=(unordered_set&& rhs)
   {
      this->numElements = std::move(rhs.numElements);
      for (size_t i = 0; i < 10; ++i)
      {
         this->buckets[i].clear();
         for (auto it = rhs.buckets[i].begin(); it != rhs.buckets[i].end(); ++it)
         {
            this->buckets[i].push_back(std::move(*it));
         }
      }
      rhs.clear();
      return *this;
   }
   unordered_set& operator=(const std::initializer_list<T>& il)
   {
      return *this;
   }
   void swap(unordered_set& rhs)
   {
      std::swap(numElements, rhs.numElements);
      for (size_t i = 0; i < 10; ++i)
         custom::swap(buckets[i], rhs.buckets[i]);
   }

   // 
   // Iterator
   //
   class iterator;
   class local_iterator;
   iterator begin()
   {
      for (int i = 0; i < 10; ++i)
      {
         if (!buckets[i].empty())
         {
            return iterator(buckets + i, buckets + 10, buckets[i].begin());
         }
      }
      return end();
   }
   iterator end()
   {
      return iterator(&buckets[10], &buckets[10], buckets[0].end());
   }

   local_iterator begin(size_t iBucket)
   {
      return local_iterator(buckets[iBucket].begin());
   }
   local_iterator end(size_t iBucket)
   {
      return local_iterator(buckets[iBucket].end());
   }

   //
   // Access
   //
   size_t bucket(const T& t)
   {
      return std::hash<T>{}(t) % 10;
   }
   iterator find(const T& t);

   //
   // Insert
   //
   custom::pair<iterator, bool> insert(const T& t);
   void insert(const std::initializer_list<T> & il);


   //
   // Remove
   //
   void clear() noexcept
   {
      for (size_t i = 0; i < 10; ++i)
         buckets[i].clear();
      numElements = 0;
   }
   iterator erase(const T& t);

   //
   // Status
   //
   size_t size() const 
   { 
      return numElements;
   }
   bool empty() const 
   { 
	  return numElements == 0;
   }
   size_t bucket_count() const 
   { 
      return buckets->size();
   }
   size_t bucket_size(size_t i) const
   {
      return buckets[i].size();
   }

private:

   custom::list<T> buckets [10];   // exactly 10 buckets
   int numElements;                // number of elements in the Hash
};


/************************************************
 * UNORDERED SET ITERATOR
 * Iterator for an unordered set
 ************************************************/
template <typename T>
class unordered_set <T> ::iterator
{
   friend class ::TestHash;   // give unit tests access to the privates
   template <class TT>
   friend class custom::unordered_set;
public:
   // 
   // Construct
   //
   iterator()  
   {  
   }
   iterator(typename custom::list<T>* pBucket,
            typename custom::list<T>* pBucketEnd,
            typename custom::list<T>::iterator itList)
            : pBucket(pBucket),
            pBucketEnd(pBucketEnd),
            itList(itList)
   {
   }
   iterator(const iterator& rhs)
            : pBucket(rhs.pBucket),
            pBucketEnd(rhs.pBucketEnd),
            itList(rhs.itList)
   {
   }

   //
   // Assign
   //
   iterator& operator = (const iterator& rhs)
   {
	  this->pBucket = rhs.pBucket;
	  this->pBucketEnd = rhs.pBucketEnd;
	  this->itList = rhs.itList;

      return *this;
   }

   //
   // Compare
   //
   bool operator != (const iterator& rhs) const 
   { 
	  return this->pBucket != rhs.pBucket || this->pBucketEnd != rhs.pBucketEnd || this->itList != rhs.itList;
   }
   bool operator == (const iterator& rhs) const 
   { 
	  return this->pBucket == rhs.pBucket && this->pBucketEnd == rhs.pBucketEnd && this->itList == rhs.itList;
   }

   // 
   // Access
   //
   T& operator * ()
   {
      return *itList;
   }

   //
   // Arithmetic
   //
   iterator& operator ++ ();
   iterator operator ++ (int postfix)
   {
      iterator temp(*this);
      ++itList;
      return temp;
   }

private:
   custom::list<T> *pBucket;
   custom::list<T> *pBucketEnd;
   typename list<T>::iterator itList;
};


/************************************************
 * UNORDERED SET LOCAL ITERATOR
 * Iterator for a single bucket in an unordered set
 ************************************************/
template <typename T>
class unordered_set <T> ::local_iterator
{
   friend class ::TestHash;   // give unit tests access to the privates

   template <class TT>
   friend class custom::unordered_set;
public:
   // 
   // Construct
   //
   local_iterator()  
   {
   }
   local_iterator(const typename custom::list<T>::iterator& itList) : itList(itList)
   {
   }
   local_iterator(const local_iterator& rhs) : itList(rhs.itList)
   { 
   }

   //
   // Assign
   //
   local_iterator& operator = (const local_iterator& rhs)
   {
	  this->itList = rhs.itList;
      return *this;
   }

   // 
   // Compare
   //
   bool operator != (const local_iterator& rhs) const
   {
	  return itList != rhs.itList;
   }
   bool operator == (const local_iterator& rhs) const
   {
	  return itList == rhs.itList;
   }

   // 
   // Access
   //
   T& operator * ()
   {
      return *itList;
   }

   // 
   // Arithmetic
   //
   local_iterator& operator ++ ()
   {
      ++itList;
      return *this;
   }
   local_iterator operator ++ (int postfix)
   {
      iterator temp(*this);
      ++itList;
      return temp;
   }


private:
   typename list<T>::iterator itList;
};


/*****************************************
 * UNORDERED SET :: ERASE
 * Remove one element from the unordered set
 ****************************************/
template <typename T>
typename unordered_set <T> ::iterator unordered_set<T>::erase(const T& t)
{
   auto itErase = find(t);
   if (itErase == end())
      return itErase;
   auto itReturn = itErase;
   ++itReturn;
   buckets[itErase.pBucket - buckets].erase(itErase.itList);
   --numElements;
   return itReturn;
}

/*****************************************
 * UNORDERED SET :: INSERT
 * Insert one element into the hash
 ****************************************/
template <typename T>
custom::pair<typename custom::unordered_set<T>::iterator, bool> unordered_set<T>::insert(const T& t)
{
   size_t iBucket = bucket(t);
   for (auto it = buckets[iBucket].begin(); it != buckets[iBucket].end(); ++it)
   {
      if (*it == t)
         return custom::pair<iterator, bool>(iterator(buckets + iBucket, buckets + 10, it), false);
   }
   buckets[iBucket].push_back(t);
   ++numElements;

   auto itLast = buckets[iBucket].begin();
   auto it = itLast;
   for (; it != buckets[iBucket].end(); ++it)
      itLast = it;

   return custom::pair<iterator, bool>(iterator(buckets + iBucket, buckets + 10, itLast), true);

}
template <typename T>
void unordered_set<T>::insert(const std::initializer_list<T> & il)
{
   for (const auto & value : il)
      insert(value);
}

/*****************************************
 * UNORDERED SET :: FIND
 * Find an element in an unordered set
 ****************************************/
template <typename T>
typename unordered_set <T> ::iterator unordered_set<T>::find(const T& t)
{
   size_t iBucket = bucket(t);
   auto itList = buckets[iBucket].begin();
   for (; itList != buckets[iBucket].end(); ++itList)
   {
      if (*itList == t)
         break;
   }
   if (itList != buckets[iBucket].end())
      return iterator(buckets + iBucket, buckets + 10, itList);
   else
      return end();
}

/*****************************************
 * UNORDERED SET :: ITERATOR :: INCREMENT
 * Advance by one element in an unordered set
 ****************************************/
template <typename T>
typename unordered_set <T> ::iterator & unordered_set<T>::iterator::operator++()
{
   ++itList;

   while (pBucket != pBucketEnd && itList == pBucket->end())
   {
      ++pBucket;

      if (pBucket == pBucketEnd)
         break;

      if (!pBucket->empty())
      {
         itList = pBucket->begin();
         return *this;
      }
   }

   return *this;
}


/*****************************************
 * SWAP
 * Stand-alone unordered set swap
 ****************************************/
template <typename T>
void swap(unordered_set<T>& lhs, unordered_set<T>& rhs)
{
	lhs.swap(rhs);
}

}
