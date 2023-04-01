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
 *    <your names here>
 ************************************************************************/

#pragma once

#include "list.h"     // because this->buckets[0] is a list
#include <memory>     // for std::allocator
#include <functional> // for std::hash
#include <cmath>      // for std::ceil


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
        unordered_set(unordered_set& rhs) : numElements(rhs.numElements)
        {
            *this = rhs;
        }
        unordered_set(unordered_set&& rhs) : numElements(std::move(numElements))
        {

            *this = std::move(rhs);
        }
        template <class Iterator>
        unordered_set(Iterator first, Iterator last)
        {
            numElements = 0;
            for (auto it = first; it != last; it++) {
                insert(*it);
            }
        }
        //
        // Assign
        //
        unordered_set& operator=(unordered_set& rhs)
        {
            clear();
            for (int i = 0; i < 10; i++)
            {
                if (!rhs.buckets[i].empty())
                    for (auto it = rhs.buckets[i].begin(); it != rhs.buckets[i].end(); it++)
                        insert((*it));
            }
            return *this;
        }

        unordered_set& operator=(unordered_set&& rhs)
        {
            numElements = rhs.numElements;
            for (int i = 0; i < 10; i++) {
                buckets[i] = std::move(rhs.buckets[i]);
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
            for (int i = 0; i < 10; i++)
                buckets[i].swap(rhs.buckets[i]);
        }

        // 
        // Iterator
        //
        class iterator;
        class local_iterator;
        iterator begin()
        {
            for (int i = 0; i < 10; i++) { //this itBucket is an iterator through the array of 10 lists
                if (!buckets[i].empty()) {
                    auto testbegin = buckets + 1;
                    auto testEnd = buckets + 10;
                    auto testIt = buckets[i].begin();
                    list<int> mylist;
                    auto it2 = mylist.begin();
                    iterator it = iterator(testbegin, testEnd, testIt);
                    return it;
                }
            }


            return end();
        }
        iterator end()
        {
            auto testbegin = buckets + 10;
            auto testEnd = buckets + 10;
            auto testIt = buckets[9].end();
            return iterator(testbegin, testEnd, testIt);
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
            std::hash<T> bucketHash;

            return bucketHash(t) % bucket_count();
        }
        iterator find(const T& t);

        //   
        // Insert
        //
        custom::pair<iterator, bool> insert(const T& t);
        void insert(const std::initializer_list<T>& il);


        // 
        // Remove
        //
        void clear() noexcept
        {
            for (int i = 0; i < 10; i++) {
                buckets[i].clear();
            }
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
            return 10;
        }
        size_t bucket_size(size_t i) const
        {
            return buckets[i].size();
        }

    private:

        custom::list<T> buckets[10];   // exactly 10 buckets
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
            pBucket = nullptr;
            pBucketEnd = nullptr;
            itList = nullptr;
        }
        iterator(typename custom::list<T>* pBucket,
            typename custom::list<T>* pBucketEnd,
            typename custom::list<T>::iterator itList)
        {
            this->pBucket = pBucket;
            this->pBucketEnd = pBucketEnd;
            this->itList = itList;
        }
        iterator(const iterator& rhs)
        {
            this->pBucket = rhs.pBucket;
            this->pBucketEnd = rhs.pBucketEnd;
            this->itList = rhs.itList;
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
            return this->itList != rhs.itList;
        }
        bool operator == (const iterator& rhs) const
        {
            return this->itList == rhs.itList;
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
            auto tempIt = this->itList;
            this->itList++;
            return tempIt;
        }

    private:
        custom::list<T>* pBucket;
        custom::list<T>* pBucketEnd;
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
            itList = nullptr;
        }
        local_iterator(const typename custom::list<T>::iterator& itList)
        {
            this->itList = itList;
        }
        local_iterator(const local_iterator& rhs)
        {
            this->itList = rhs.itList;
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
            return this->itList != rhs.itList;
        }
        bool operator == (const local_iterator& rhs) const
        {
            return this->itList == rhs.itList;
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
            itList++;
            return *this;
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

        itErase.pBucket->erase(itErase.itList);
        numElements--;
        return itReturn;
    }

    /*****************************************
     * UNORDERED SET :: INSERT
     * Insert one element into the hash
     ****************************************/
    template <typename T>
    custom::pair<typename custom::unordered_set<T>::iterator, bool> unordered_set<T>::insert(const T& t)
    {
        // find the bucket where we need to add
        int iBucket = bucket(t);

        // check for duplicates
        auto it = buckets[iBucket].begin();
        for (it; it != buckets[iBucket].end(); it++)
            if ((*it) == t)
                return custom::pair<custom::unordered_set<T>::iterator, bool>(iterator(buckets + iBucket, buckets + 10, it), false);

        buckets[iBucket].push_back(t);
        numElements++;
        it = buckets[iBucket].rbegin();
        return custom::pair<custom::unordered_set<T>::iterator, bool>(iterator(buckets + iBucket, buckets + 10, it), true);


    }
    template <typename T>
    void unordered_set<T>::insert(const std::initializer_list<T>& il)
    {
        for (auto it = il.begin(); it != il.end(); it++)
        {
            insert((*it));
        }
    }

    /*****************************************
     * UNORDERED SET :: FIND
     * Find an element in an unordered set
     ****************************************/
    template <typename T>
    typename unordered_set <T> ::iterator unordered_set<T>::find(const T& t)
    {
        // find the bucket the element would be in.
        int iBucket = bucket(t);
        auto itList = buckets[iBucket].find(t);

        if (itList != buckets[iBucket].end())
        {
            auto testbegin = buckets + iBucket;
            auto testEnd = buckets + 10;
            return iterator(testbegin, testEnd, itList);
        }
        return end();
    }

    /*****************************************
     * UNORDERED SET :: ITERATOR :: INCREMENT
     * Advance by one element in an unordered set
     ****************************************/
    template <typename T>
    typename unordered_set <T> ::iterator& unordered_set<T>::iterator::operator ++ ()
    {
        /*
         itList = itList
          * itVector = pBucket
          * itVectorEnd = pBucketEnd
          */
          // 1. Only advance if we are not already at the end.
        if (pBucket == pBucketEnd)
            return *this;

        // 2. Advance the lister iterator. Check to see if we are at the end.
        ++itList;
        if (itList != pBucket->end())
            return *this;
        // 3. If we are at the end, move to the next bucket.
        ++pBucket;
        while (pBucket != pBucketEnd && pBucket->empty())
            ++pBucket;

        if (pBucket != pBucketEnd)
            itList = pBucket->begin();
        return *this;
    }

    /*****************************************
     * SWAP
     * Stand-alone unordered set swap
     ****************************************/
    template <typename T>
    void swap(unordered_set<T>& lhs, unordered_set<T>& rhs)
    {
        std::swap(lhs, rhs);
    }
}