/***********************************************************************
 * Header:
 *    PRIORITY QUEUE
 * Summary:
 *    Our custom implementation of std::priority_queue
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        priority_queue          : A class that represents a Priority Queue
 * Author
 *    Jake Rammell Austin Eldridge
 ************************************************************************/

#pragma once

#include <cassert>
#include "vector.h"

class TestPQueue;    // forward declaration for unit test class

namespace custom
{

    /*************************************************
     * P QUEUE
     * Create a priority queue.
     *************************************************/
    template<class T>
    class priority_queue
    {
        friend class ::TestPQueue; // give the unit test class access to the privates
        template <class TT>
        friend void swap(priority_queue<TT>& lhs, priority_queue<TT>& rhs);
    public:

        //
        // construct
        //
        priority_queue() : container{ vector<T>() }
        {

        }
        priority_queue(const priority_queue& rhs) : container{ vector<T>(rhs.container) }
        {
        }
        priority_queue(priority_queue&& rhs) : container{ vector<T>(std::move(rhs.container)) }
        {
        }
        template <class Iterator>
        priority_queue(Iterator first, Iterator last) : container{ vector<T>() }
        {

            for (auto it = first; it != last; it++) {
                container.push_back(*(it));

            }
            container.shrink_to_fit();



        }
        explicit priority_queue(custom::vector<T>&& rhs) : container(vector<T>(std::move(rhs)))
        {
        }
        explicit priority_queue(custom::vector<T>& rhs) : container(vector<T>(rhs))
        {
        }
        ~priority_queue() {}

        //
        // Access
        //
        const T& top() const;

        //
        // Insert
        //
        void  push(const T& t);
        void  push(T&& t);

        //
        // Remove
        //
        void  pop();

        //
        // Status
        //
        size_t size()  const
        {
            return container.size();
        }
        bool empty() const
        {
            return size() == size_t(0);
        }

    private:

        bool percolateDown(size_t indexHeap);      // fix heap from index down. This is a heap index!

        custom::vector<T> container;

    };

    /************************************************
     * P QUEUE :: TOP
     * Get the maximum item from the heap: the top item.
     ***********************************************/
    template <class T>
    const T& priority_queue <T> ::top() const
    {
        if (container.empty()) {
            throw "std:out_of_range";
        }
        return container[0];
    }

    /**********************************************
     * P QUEUE :: POP
     * Delete the top item from the heap.
     **********************************************/
    template <class T>
    void priority_queue <T> ::pop()
    {
        if (size())
        {
            std::swap(container[0], container[size() - 1]);
            container.pop_back();
            percolateDown(1);
        }
    }

    /*****************************************
     * P QUEUE :: PUSH
     * Add a new element to the heap, reallocating as necessary
     ****************************************/
    template <class T>
    void priority_queue <T> ::push(const T& t)
    {
        container.push_back(t);

        int currIndex = container.size() / 2;
        while (currIndex && percolateDown(currIndex))
            currIndex = currIndex / 2;
    }
    template <class T>
    void priority_queue <T> ::push(T&& t)
    {
        container.push_back(std::move(t));

        int currIndex = container.size() / 2;
        while (currIndex && percolateDown(currIndex))
            currIndex = currIndex / 2;
    }

    /************************************************
     * P QUEUE :: PERCOLATE DOWN
     * The item at the passed index may be out of heap
     * order. Take care of that little detail!
     * Return TRUE if anything changed.
     ************************************************/
    template <class T>
    bool priority_queue <T> ::percolateDown(size_t indexHeap)
    {
        //heap index starts at 1
        int indexTrueLeft = (indexHeap * 2) - 1;
        int indexTrueRight = indexTrueLeft + 1;
        int indexTrue = indexHeap - 1;

        int indexBigger;
        //find which child is bigger
        if (indexTrueRight < size() && container[indexTrueLeft] < container[indexTrueRight])
            indexBigger = indexTrueRight;
        else
            indexBigger = indexTrueLeft;

        //if the bigger child is greater than the parent, swap them
        if (container[indexTrue] < container[indexBigger] && indexBigger < size())
        {
            std::swap(container[indexTrue], container[indexBigger]);
            percolateDown(indexBigger + 1);
            return true;
        }



        return false;
    }


    /************************************************
     * SWAP
     * Swap the contents of two priority queues
     ************************************************/
    template <class T>
    inline void swap(custom::priority_queue <T>& lhs,
        custom::priority_queue <T>& rhs)
    {
        lhs.container.swap(rhs.container);
    }

};