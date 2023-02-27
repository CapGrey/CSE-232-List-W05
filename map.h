/***********************************************************************
 * Header:
 *    map
 * Summary:
 *    Our custom implementation of a std::map
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        map                 : A class that represents a map
 *        map::iterator       : An iterator through a map
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include "pair.h"     // for pair
#include "bst.h"      // no nested class necessary for this assignment

#ifndef debug
#ifdef DEBUG
#define debug(x) x
#else
#define debug(x)
#endif // DEBUG
#endif // !debug

class TestMap; // forward declaration for unit tests

namespace custom
{

/*****************************************************************
 * MAP
 * Create a Map, similar to a Binary Search Tree
 *****************************************************************/
template <class K, class V>
class map
{
   friend ::TestMap; // give unit tests access to the privates
   template <class KK, class VV>
   friend void swap(map<KK, VV>& lhs, map<KK, VV>& rhs);
public:
   using Pairs = custom::pair<K, V>;

   // 
   // Construct
   //
   map() : bst{BST<Pairs>()}
   {
   }
   map(const map &  rhs)  : bst{BST<Pairs>(rhs.bst)}
   { 
   }
   map(map&& rhs) : bst{BST<Pairs>(std::move(rhs.bst))}
   { 
   }
   template <class Iterator>
   map(Iterator first, Iterator last) : bst{BST<Pairs>()}
   {
       insert(first, last);
   }
   map(const std::initializer_list <Pairs>& il) : bst{BST<Pairs>()}
   {
       insert(il);
   }
  ~map()         
   {
      
   }

   //
   // Assign
   //
   map & operator = (const map & rhs) 
   {
       clear();
       bst = rhs.bst;
      return *this;
   }
   map & operator = (map && rhs)
   {
       clear();
      swap(this->bst, rhs.bst);
      return *this;
   }
   map & operator = (const std::initializer_list <Pairs> & il)
   {
       clear();
       for (auto element : il)
       {
           insert(element);
       }
      return *this;
   }
   
   // 
   // Iterator
   //
   class iterator;
   iterator begin() 
   { 

      return bst.begin();
   }
   iterator end() 
   { 
      return bst.end();
   }

   // 
   // Access
   //
   const V & operator [] (const K & k) const;
         V & operator [] (const K & k);
   const V & at (const K& k) const;
         V & at (const K& k);
   iterator    find(const K & k)
   {
      
       
      return bst.find(k);
   }

   //
   // Insert
   //
   custom::pair<typename map::iterator, bool> insert(Pairs && rhs)
   {
       auto insertAttempt = bst.insert(std::move(rhs));

       return make_pair(iterator(insertAttempt.first), insertAttempt.second);
   }
   custom::pair<typename map::iterator, bool> insert(const Pairs & rhs)
   {
       auto insertAttempt = bst.insert(rhs);
       
      return make_pair(iterator(insertAttempt.first), insertAttempt.second);
   }

   template <class Iterator>
   void insert(Iterator first, Iterator last)
   {
       for (auto p = first; p != last; p++)
       {
           insert(*p);
       }
   }
   void insert(const std::initializer_list <Pairs>& il)
   {
       for (auto element : il) {
           insert(element);
       }
   }

   //
   // Remove
   //
   void clear() noexcept
   {
       bst.clear();
   }
   size_t erase(const K& k);
   iterator erase(iterator it);
   iterator erase(iterator first, iterator last);

   //
   // Status
   //
   bool empty() const noexcept { return bst.numElements==0; }
   size_t size() const noexcept { return bst.size(); }


private:

   // the students DO NOT need to use a nested class
   BST < pair <K, V >> bst;
};


/**********************************************************
 * MAP ITERATOR
 * Forward and reverse iterator through a Map, just call
 * through to BSTIterator
 *********************************************************/
template <typename K, typename V>
class map <K, V> :: iterator
{
   friend class ::TestMap; // give unit tests access to the privates
   template <class KK, class VV>
   friend class custom::map;
public:
   //
   // Construct
   //
   iterator() : it{BST<Pairs>::iterator()}
   {
   }
   iterator(const typename BST < pair <K, V> > :: iterator & rhs) : it{BST<Pairs>::iterator(rhs)}
   { 
   }
   iterator(const iterator & rhs) : it{BST<Pairs>::iterator(rhs.it)}
   { 
   }

   //
   // Assign
   //
   iterator & operator = (const iterator & rhs)
   {
       it = rhs.it;
      return *this;
   }

   //
   // Compare
   //
   bool operator == (const iterator & rhs) const { return it == rhs.it; }
   bool operator != (const iterator & rhs) const { return it != rhs.it; }

   // 
   // Access
   //
   const pair <K, V> & operator * () const
   {
      return *(it);
   }

   //
   // Increment
   //
   iterator & operator ++ ()
   {
       it++;
      return *this;
   }
   iterator operator ++ (int postfix)
   {
       iterator placeholder(*this);
       it++;
      return *placeholder;
   }
   iterator & operator -- ()
   {
       it--;
      return *this;
   }
   iterator  operator -- (int postfix)
   {
       iterator placeholder(*this);
       it--;
      return *placeholder;
   }

private:

   // Member variable
   typename BST < pair <K, V >>  :: iterator it;   
};


/*****************************************************
 * MAP :: SUBSCRIPT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
V& map <K, V> :: operator [] (const K& key)
{
    pair<K, V> searchablePair(key, V());
    iterator it = bst.find(searchablePair);
    
    if (it.it.pNode != nullptr)
    {
        auto value = (*it).second;
        return value;
    }
    else
    {
        bst.insert(searchablePair);
        auto refrenceToValue = *(bst.find(searchablePair));
        return refrenceToValue.second;
    }
   
}

/*****************************************************
 * MAP :: SUBSCRIPT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
const V& map <K, V> :: operator [] (const K& key) const
{
    pair<K, V> searchablePair(key, V());
    iterator it = bst.find(searchablePair);

    if (it.it.pNode != nullptr)
    {
        auto value = (*it).second;
        return value;
    }
    else
    {
        bst.insert(searchablePair);
        return searchablePair.second;
    }
}

/*****************************************************
 * MAP :: AT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
V& map <K, V> ::at(const K& key)
{
    pair<K, V> searchablePair(key, V());
    iterator it = bst.find(searchablePair);

    if (it.it.pNode != nullptr)
    {
        auto value = (*it).second;
        return value;
    }
    else
    {
        throw std::out_of_range("invalid map<K, T> key");
    }
}

/*****************************************************
 * MAP :: AT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
const V& map <K, V> ::at(const K& key) const
{
    pair<K, V> searchablePair(key, V());
    iterator it = bst.find(searchablePair);

    if (it.it.pNode != nullptr)
    {
        auto value = (*it).second;
        return value;
    }
    else
    {
        throw std::out_of_range("invalid map<K, T> key");
    }
}

/*****************************************************
 * SWAP
 * Swap two maps
 ****************************************************/
template <typename K, typename V>
void swap(map <K, V>& lhs, map <K, V>& rhs)
{
    lhs.bst.swap(rhs.bst);
}

/*****************************************************
 * ERASE
 * Erase one element
 ****************************************************/
template <typename K, typename V>
size_t map<K, V>::erase(const K& k)
{
    //auto pairToErase = std::make_pair(k, V()); //this is a pair we can use to access the bst
   return size_t(99);
}

/*****************************************************
 * ERASE
 * Erase several elements
 ****************************************************/
template <typename K, typename V>
typename map<K, V>::iterator map<K, V>::erase(map<K, V>::iterator first, map<K, V>::iterator last)
{
   return iterator();
}

/*****************************************************
 * ERASE
 * Erase one element
 ****************************************************/
template <typename K, typename V>
typename map<K, V>::iterator map<K, V>::erase(map<K, V>::iterator it)
{
   return iterator();
}

}; //  namespace custom

