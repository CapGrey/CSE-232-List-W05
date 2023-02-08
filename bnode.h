/***********************************************************************
 * Header:
 *    BINARY NODE
 * Summary:
 *    One node in a binary tree (and the functions to support them).
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        BNode         : A class representing a BNode
 *    Additionally, it will contain a few functions working on Node
 * Author
 *    <your names here>
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
      data = T();
      pLeft = pRight = pParent = nullptr;
   }
   BNode(const T& t)
   {
      data = T(t);
      pLeft = pRight = pParent = nullptr;
   }
   BNode(T&& t)
   {
      data = t;
      pLeft = pRight = pParent = nullptr;
   }

   //
   // Data
   //
   BNode <T>* pLeft;
   BNode <T>* pRight;
   BNode <T>* pParent;
   T data;
};

/*******************************************************************
 * SIZE BTREE
 * Return the size of a b-tree under the current node
 *******************************************************************/
template <class T>
inline size_t size(const BNode <T> * p)
{
    if (p == nullptr) {
        return 0;
   }
   return size(p->pRight) + 1 + size(p->pRight);
}


/******************************************************
 * ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <class T>
inline void addLeft(BNode <T> * pNode, BNode <T> * pAdd)
{

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
    if (pThis == nullptr) {
        return;
    }
    clear(pThis->pLeft);
    clear(pThis->pRight);
    delete pThis;
    pThis = nullptr;
}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(BNode <T>*& pLHS, BNode <T>*& pRHS)
{
    BNode<T>* temp = pLHS;
    pLHS = pRHS;
    pRHS = temp;

}

/**********************************************
 * COPY BINARY TREE
 * Copy pSrc->pRight to pDest->pRight and
 * pSrc->pLeft onto pDest->pLeft
 *********************************************/
template <class T>
BNode <T> * copy(const BNode <T> * pSrc)
{
    if (pSrc == nullptr) {
        return nullptr;
    }
    BNode<T>* pDes = new BNode<T>(pSrc->data);

    pDes->pLeft = copy(pSrc->pLeft);
    if (pDes->pLeft != nullptr) {
        pDes->pLeft->pParent = pDes;
    }

    pDes->pRight = copy(pSrc->pRight);
    if (pDes->pRight != nullptr) {
        pDes->pRight->pParent = pDes;
    }

    return pDes;
}

/**********************************************
 * assign
 * copy the values from pSrc onto pDest preserving
 * as many of the nodes as possible.
 *********************************************/
template <class T>
void assign(BNode <T> * & pDest, const BNode <T>* pSrc)
{
    if (pSrc == nullptr) { //termination condition
        clear(pDest);
        return;
    }
    if (pDest == nullptr && pSrc != nullptr) {
        pDest = new BNode<T>(pSrc->data);
       
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
