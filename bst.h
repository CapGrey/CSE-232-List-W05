/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        BST                 : A class that represents a binary search tree
 *        BST::iterator       : An iterator through BST
 * Author
 *    Austin Eldredge, Jake Rammel
 ************************************************************************/

#pragma once

#ifdef DEBUG
#define debug(x) x
#else // !DEBUG
#define debug(x)
#endif // !DEBUG

#include <cassert>
#include <utility>
#include <memory>     // for std::allocator
#include <functional> // for std::less
#include <utility>    // for std::pair

class TestBST; // forward declaration for unit tests
class TestMap;
class TestSet;

namespace custom
{

   template <class TT>
   class set;
   template <class KK, class VV>
   class map;

/*****************************************************************
 * BINARY SEARCH TREE
 * Create a Binary Search Tree
 *****************************************************************/
template <typename T>
class BST
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestMap;
   friend class ::TestSet;

   template <class KK, class VV>
   friend class map;

   template <class TT>
   friend class set;

   template <class KK, class VV>
   friend void swap(map<KK, VV>& lhs, map<KK, VV>& rhs);
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

   bool   empty() const noexcept { return (numElements == 0 ? true : false); }
   size_t size()  const noexcept { return numElements;   }
   

private:

   class BNode;
   BNode * root;              // root node of the binary search tree
   size_t numElements;        // number of elements currently in the tree
   void assign(BST<T>::BNode*& pDes, BST<T>::BNode* pSrc);
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
   BNode()
   {
       data = T();
       pLeft = pRight = pParent = nullptr;
   }
   BNode(const T &  t) : data{T(t)}
   {
       pLeft = pRight = pParent = nullptr;
   }
   BNode(T && t) 
   {  
       data = t;
       pLeft = pRight = pParent = nullptr;
   }

   //
   // Insert
   //
   void addLeft (BNode * pNode);
   void addRight(BNode * pNode);
   void addLeft (const T &  t);
   void addRight(const T &  t);
   void addLeft(       T && t);
   void addRight(      T && t);

   // 
   // Status
   //
   bool isRightChild(BNode * pNode) const { return true; }
   bool isLeftChild( BNode * pNode) const { return true; }

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
   friend class ::TestMap;
   friend class ::TestSet;

   template <class KK, class VV>
   friend class map;

   template <class TT>
   friend class set; 
public:
   // constructors and assignment
   iterator(BNode * p = nullptr) : pNode{p}
   { 
   }
   iterator(const iterator & rhs) : pNode{rhs.pNode}
   { 
   }
   iterator & operator = (const iterator & rhs)
   {
       this->pNode = rhs.pNode;
      return *this;
   }

   // compare
   bool operator == (const iterator & rhs) const
   {
      return (pNode==rhs.pNode);
   }
   bool operator != (const iterator & rhs) const
   {
      return !(pNode==rhs.pNode);
   }

   // de-reference. Cannot change because it will invalidate the BST
   const T & operator * () const 
   {
      return pNode->data;
   }

   // increment and decrement
   iterator & operator ++ ();
   iterator   operator ++ (int postfix)
   {
      return *this;
   }
   iterator & operator -- ();
   iterator   operator -- (int postfix)
   {
      return *this;;
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
BST <T> ::BST()
{
   numElements = 0;
   root = nullptr;
}

/*********************************************
 * BST :: COPY CONSTRUCTOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> ::BST(const BST<T>& rhs) : root{ nullptr }, numElements { 0 }
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
   numElements = rhs.numElements;
   root = rhs.root;

   rhs.root = nullptr;
   rhs.numElements = 0;
}

/*********************************************
 * BST :: INITIALIZER LIST CONSTRUCTOR
 * Create a BST from an initializer list
 ********************************************/
template <typename T>
BST <T> ::BST(const std::initializer_list<T>& il)
{
   numElements = 99;
   root = new BNode;
}

/*********************************************
 * BST :: DESTRUCTOR
 ********************************************/
template <typename T>
BST <T> :: ~BST()
{

}


/*********************************************
 * BST :: ASSIGNMENT OPERATOR
 * Copy one tree to another
 ********************************************/
template <class T>
void BST <T> ::assign(BST<T>::BNode*& pDest, BST<T>::BNode* pSrc) {
    if (pSrc == nullptr) { //termination condition
        //clear(pDest);
        return;
    }
    if (pDest == nullptr && pSrc != nullptr) {
        pDest = new BNode(pSrc->data);

    }
    else if (pDest != nullptr and pSrc != nullptr) {
        pDest->data = pSrc->data;
    }
    assign(pDest->pRight, pSrc->pRight);
    assign(pDest->pLeft, pSrc->pLeft);
    if (pDest->pRight != nullptr) {
        pDest->pRight->pParent = pDest;
    }
    if (pDest->pLeft != nullptr) {
        pDest->pLeft->pParent = pDest;
    }
}

template <typename T>
BST <T> & BST <T> :: operator = (const BST <T> & rhs)
{
    if (rhs.empty()) {
        clear();
   }
   assign(root, rhs.root);
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
    for (auto element : il) {
        insert(element);
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
   return *this;
}

/*********************************************
 * BST :: SWAP
 * Swap two trees
 ********************************************/
template <typename T>
void BST <T> :: swap (BST <T>& rhs)
{

}

/*****************************************************
 * BST :: INSERT
 * Insert a node at a given location in the tree
 ****************************************************/
template <typename T>
std::pair<typename BST <T> :: iterator, bool> BST <T> :: insert(const T & t, bool keepUnique)
{
   std::pair<iterator, bool> pairReturn(end(), false);
   return pairReturn;
}

template <typename T>
std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(T && t, bool keepUnique)
{
   std::pair<iterator, bool> pairReturn(end(), false);
   return pairReturn;
}

/*************************************************
 * BST :: ERASE
 * Remove a given node as specified by the iterator
 ************************************************/
template <typename T>
typename BST <T> ::iterator BST <T> :: erase(iterator & it)
{  
   return end();
}

/*****************************************************
 * BST :: CLEAR
 * Removes all the BNodes from a tree
 ****************************************************/
template <typename T>
void BST <T> ::clear() noexcept
{
    iterator pNode = begin();
    /*while (*pNode) {
        T* pPast = *pNode;
        pNode++;
        delete pPast;
        
    }
    */
   
}

/*****************************************************
 * BST :: BEGIN
 * Return the first node (left-most) in a binary search tree
 ****************************************************/
template <typename T>
typename BST <T> :: iterator custom :: BST <T> :: begin() const noexcept
{
    if (empty()) {
        return end();
   }
    BNode* p = root;
    while (p->pLeft) {
        p = p->pLeft;
    }
    return p;
}


/****************************************************
 * BST :: FIND
 * Return the node corresponding to a given value
 ****************************************************/
template <typename T>
typename BST <T> :: iterator BST<T> :: find(const T & t)
{
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
    this->pLeft = pNode;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (BNode * pNode)
{
    this->pRight = pNode;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> :: BNode :: addLeft (const T & t)
{
    this->pLeft = new T(t);
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> ::BNode::addLeft(T && t)
{
    this->pLeft = new T(t);
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (const T & t)
{
    this->pRight = new T(t);
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> ::BNode::addRight(T && t)
{
    this->pRight = new T(t);
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
    if (!pNode) { //if were not pointing to anything
        return *this;
    }
    if (pNode->pRight) {
        pNode = pNode->pRight;
        while (pNode->pLeft) {
            pNode = pNode->pLeft;
        }
        return *this;
    }
    if (pNode->pRight == nullptr && pNode->pParent->pLeft == pNode) { //if this node is the parents left-most child
       pNode = pNode->pParent;
       return *this;
    }
    if (pNode->pRight == nullptr && pNode->pParent->pRight == pNode) { //if we are the parents righthand side child
        while (pNode && pNode->pParent->pRight == pNode) {
            pNode = pNode->pParent;
            if (pNode->pParent == nullptr) {
                pNode = nullptr; //there is no greater value, we increment to a nullptr
                return *this;  //this is the end condition if there is no greater value in the tree
            }
        }
        pNode = pNode->pParent;
        return *this;
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
   return *this;

}


} // namespace custom


