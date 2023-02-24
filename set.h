/***********************************************************************
* Header:
*    Set
* Summary:
*      __       ____       ____         __
*     /  |    .'    '.   .'    '.   _  / /
*     `| |   |  .--.  | |  .--.  | (_)/ /
*      | |   | |    | | | |    | |   / / _
*     _| |_  |  `--'  | |  `--'  |  / / (_)
*    |_____|  '.____.'   '.____.'  /_/
*
*    This will contain the class definition of:
*        set                 : A class that represents a Set
*        set::iterator       : An iterator through Set
* Author
*    Jake Rammell, Austin Elderidge
************************************************************************/

#pragma once

#include <cassert>
#include <iostream>
#include "bst.h"
#include <memory>     // for std::allocator
#include <functional> // for std::less

class TestSet;        // forward declaration for unit tests

namespace custom
{

    /************************************************
     * SET
     * A class that represents a Set
     ***********************************************/
    template <typename T>
    class set
    {
        friend class ::TestSet; // give unit tests access to the privates
    public:

        // 
        // Construct
        //
        set() :
            bst{ BST<T>() } //just instantiate a new BST blank
        {
        }
        set(const set& rhs) :
            bst{ BST<T>(rhs.bst) }
        {
        }
        set(set&& rhs) :
            bst{ BST<T>(std::move(rhs.bst)) } //move the BST using the move constructor
        {
        }
        set(const std::initializer_list <T>& il) :
            bst{ BST<T>(il) }
        {
        }
        template <class Iterator>
        set(Iterator first, Iterator last)
        {
            insert(first, last); //insert all elements between the two iterators
        }
        ~set() { clear(); }

        //
        // Assign
        //

        set& operator = (const set& rhs)
        {
            clear();
            bst = rhs.bst;
            return *this;
        }
        set& operator = (set&& rhs)
        {
            clear();
            bst.swap(rhs.bst);
            return *this;
        }
        set& operator = (const std::initializer_list <T>& il)
        {
            clear();
            for (auto element : il) {
                bst.insert(element);
            }
            return *this;
        }
        void swap(set& rhs) noexcept
        {
            bst.swap(rhs.bst);
        }

        //
        // Iterator
        //

        class iterator;
        iterator begin() const noexcept
        {
            return iterator(bst.begin());
        }
        iterator end() const noexcept
        {
            return iterator(bst.end());
        }

        //
        // Access
        //
        iterator find(const T& t)
        {
            return iterator(bst.find(t));
        }

        //
        // Status
        //
        bool   empty() const noexcept
        {
            return bst.empty();
        }
        size_t size() const noexcept
        {
            return bst.size();
        }

        //
        // Insert
        //
        std::pair<iterator, bool> insert(const T& t)
        {
            std::pair<iterator, bool> bstPair = bst.insert(t, true);
            return bstPair;
        }
        std::pair<iterator, bool> insert(T&& t)
        {
            std::pair<iterator, bool> bstPair = bst.insert(t, true);
            return bstPair;
        }
        void insert(const std::initializer_list <T>& il)
        {
            for (auto element : il)
            {
                insert(element);
            }
        }
        template <class Iterator>
        void insert(Iterator first, Iterator last)
        {
            while (first != last)
            {
                insert((*first));
                first++;
            }
        }


        //
        // Remove
        //
        void clear() noexcept
        {
            bst.clear();
        }
        iterator erase(iterator& it)
        {
            return iterator(bst.erase(it.it));
        }
        size_t erase(const T& t)
        {
            iterator it = find(t);
            if (it == end())
                return 0;

            erase(it);
            return 1;
        }
        iterator erase(iterator& itBegin, iterator& itEnd)
        {
            while (itBegin != itEnd)
            {
                itBegin = erase(itBegin);
            }
            return itEnd;
        }

    private:

        custom::BST <T> bst;
    };


    /**************************************************
     * SET ITERATOR
     * An iterator through Set
     *************************************************/
    template <typename T>
    class set <T> ::iterator
    {
        friend class ::TestSet; // give unit tests access to the privates
        friend class custom::set<T>;

    public:
        // constructors, destructors, and assignment operator
        iterator() : it{ BST<T>::iterator() }
        {
        }
        iterator(const typename custom::BST<T>::iterator& itRHS) : it{ BST<T>::iterator(itRHS) } //use the paramater BST iterator to create our own
        {
        }
        iterator(const iterator& rhs) : it{ BST<T>::iterator(rhs.it) } //retrieve the iterators BST iterator and use it to construct our own
        {
        }
        iterator& operator = (const iterator& rhs)
        {

            it = rhs.it; //make the right hand sides iterator equal to ours
            return *this;
        }

        // equals, not equals operator
        bool operator != (const iterator& rhs) const
        {
            return (this->it != rhs.it);
        }
        bool operator == (const iterator& rhs) const
        {

            return (this->it == rhs.it);
        }

        // dereference operator: by-reference so we can modify the Set
        const T& operator * () const
        {
            return *this->it;
        }

        // prefix increment
        iterator& operator ++ ()
        {
            it++;
            return *this;
        }

        // postfix increment
        iterator operator++ (int postfix)
        {
            iterator previous = iterator(this);
            it++;
            return previous;
        }

        // prefix decrement
        iterator& operator -- ()
        {
            it--;
            return *this;
        }

        // postfix decrement
        iterator operator-- (int postfix)
        {
            iterator previous = iterator(this);
            it--;
            return previous;
        }

    private:

        typename custom::BST<T>::iterator it;
    };

    /***********************************************
     * SET : EQUIVALENCE
     * See if two sets are the same size
     ***********************************************/
    template <typename T>
    bool operator == (const set <T>& lhs, const set <T>& rhs)
    {
        return true;
    }

    template <typename T>
    inline bool operator != (const set <T>& lhs, const set <T>& rhs)
    {
        return true;
    }

    /***********************************************
     * SET : RELATIVE COMPARISON
     * See if one set is lexicographically before the second
     ***********************************************/
    template <typename T>
    bool operator < (const set <T>& lhs, const set <T>& rhs)
    {
        return true;
    }

    template <typename T>
    inline bool operator > (const set <T>& lhs, const set <T>& rhs)
    {
        return true;
    }

}; // namespace custom
