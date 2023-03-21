/***********************************************************************
 * Header:
 *    DEQUE
 * Summary:
 *    Our custom implementation of a deque
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        deque                 : A class that represents a deque
 *        deque::iterator       : An iterator through a deque
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

 // Debug stuff
#include <cassert>
#include <utility>

class TestDeque;    // forward declaration for TestDeque unit test class

namespace custom
{

    /******************************************************
     * DEQUE
     *   0   1   2   3   4
     * +---+---+---+---+---+
     * |   | A | B | C |   |
     * +---+---+---+---+---+
     * iaFront = 1
     * numElements = 3
     * numCapacity = 5
     *****************************************************/
    template <class T>
    class deque
    {
        friend class ::TestDeque; // give unit tests access to the privates
    public:

        // 
        // Construct
        //
        deque()
        {
            data = nullptr;
            numElements = 0;
            numCapacity = 0;
            iaFront = 0;
        }
        deque(int newCapacity);
        deque(const deque <T>& rhs);
        ~deque()
        {
        }

        //
        // Assign
        //
        deque<T>& operator = (const deque <T>& rhs);

        //
        // Iterator
        //
        class iterator;
        iterator begin()
        {
            return iterator(this, iaFront);
        }
        iterator end()
        {
           
          return iterator(this, numElements);
          
        }

        //
        // Access
        //
        T& front();
        T& back();
        const T& front() const;
        const T& back()  const;
        const T& operator[](size_t index) const;
        T& operator[](size_t index);

        // 
        // Insert
        //
        void push_front(const T& t);
        void push_back(const T& t);

        //
        // Remove
        //
        void clear()
        {
            for (int id = 0; id < numElements; id++)
                data[id] = NULL;
            numElements = 0;
        }
        void pop_front();
        void pop_back();

        // 
        // Status
        //
        size_t size() const
        {
            return numElements;
        }
        bool empty() const
        {
            return (size() == 0);
        }


    private:

        // fetch array index from the deque index
        int iaFromID(int id) const
        {
            // ensure that aiFront is a possitive value
            int frontId = iaFront;

            if (frontId < 0)
            {
                frontId = frontId % (int)numCapacity;
                frontId = numCapacity + frontId;
            }
               
            if (id < 0) {
                id += numElements - 1;
            }
            int ia = (id + frontId) % numCapacity;
            return ia;
        }

        void resize(int newCapacity = 0);

        // member variables
        T* data;           // dynamically allocated data for the deque
        size_t numCapacity; // the size of the data array
        size_t numElements; // number of elements in the deque
        int iaFront;        // the index of the first item in the array
    };



    /**********************************************************
     * DEQUE ITERATOR
     * Forward and reverse iterator through a deque, just call
     *********************************************************/
    template <typename T>
    class deque <T> ::iterator
    {
        friend class ::TestDeque; // give unit tests access to the privates
    public:
        //
        // Construct
        //
        iterator()
        {
            pDeque = nullptr;
            id = 0;
        }
        iterator(custom::deque<T>* pDeque, int id)
        {
            this->pDeque = pDeque;
            this->id = id;

        }
        iterator(const iterator& rhs)
        {
            id = rhs.id;
            pDeque = rhs.pDeque;
        }

        //
        // Assign
        //
        iterator& operator = (const iterator& rhs)
        {
            id = rhs.id;
            pDeque = rhs.pDeque;
            return *this;
        }

        //
        // Compare
        //
        bool operator == (const iterator& rhs) const { return &pDeque[id] == &rhs.pDeque[rhs.id]; }
        bool operator != (const iterator& rhs) const { return &pDeque[id] != &rhs.pDeque[rhs.id]; }
        

        // 
        // Access
        //
        const T& operator * () const
        {
            return pDeque[id];
        }
        T& operator * ()
        {
            return pDeque->data[id];
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
        iterator  operator -- (int postfix)
        {
            iterator temp = *this;
            --id;
            return temp;
        }

    private:

        // Member variables
        int id;             // deque index
        deque<T>* pDeque;
    };


    /****************************************************
     * DEQUE : CONSTRUCTOR - non-default
     ***************************************************/
    template <class T>
    deque <T> ::deque(int newCapacity)
    {
        data = nullptr;
        numElements = 0;
        numCapacity = newCapacity;
        iaFront = 0;
    }

    /****************************************************
     * DEQUE : CONSTRUCTOR - copy
     ***************************************************/
    template <class T>
    deque <T> ::deque(const deque <T>& rhs)
    {
        data = rhs.data;
        numCapacity = rhs.numCapacity;
        numElements = rhs.numElements;
        iaFront = rhs.iaFront;
    }


    /****************************************************
     * DEQUE : ASSIGNMENT OPERATOR
     ***************************************************/
    template <class T>
    deque <T>& deque <T> :: operator = (const deque <T>& rhs)
    {
        
        iterator itLHS = this->begin();
        deque<T> rhsCopy(rhs); //making a non-const copy
        iterator itRHS = rhsCopy.begin();

        if (rhsCopy.data == nullptr) {
            this->data = nullptr;
            numCapacity = 0;
            numElements = 0;
        }

        iterator testValue2 = rhsCopy.end();
        while (itLHS != this->end() && itRHS != rhsCopy.end()) {
            *itLHS = *itRHS;
            ++itLHS;
            ++itRHS;
        }
        if (itLHS != this->end()) {
            bool test = true;
        }
        iterator testValue = rhsCopy.end();
        auto what = (itRHS == rhsCopy.end());
        while (itRHS != rhsCopy.end()) { //getting rid of any extra slots
            push_back(*itRHS);
            ++itRHS;
            
        }
        
        numElements = rhsCopy.numElements;
        

        return *this;
    }


    /**************************************************
     * DEQUE :: FRONT
     * Fetch the item that is at the beginning of the deque
     *************************************************/
    template <class T>
    const T& deque <T> ::front() const
    {
        return data[iaFront % (int)numCapacity];
    }
    template <class T>
    T& deque <T> ::front()
    {
        return data[iaFront % (int)numCapacity];
    }

    /**************************************************
     * DEQUE :: BACK
     * Fetch the item that is at the end of the deque
     *************************************************/
    template <class T>
    const T& deque <T> ::back() const
    {
        return data[iaFromID(numElements - 1)];
    }
    template <class T>
    T& deque <T> ::back()
    {
        return data[iaFromID(numElements - 1)];
    }

    /**************************************************
     * DEQUE :: SUBSCRIPT
     * Fetch the item in the deque
     *************************************************/
    template <class T>
    const T& deque <T> ::operator[](size_t index) const
    {
        
        return data[iaFromID(index)];
    }
    template <class T>
    T& deque <T> ::operator[](size_t index)
    {
        return data[iaFromID(index)];
    }

    /*****************************************************
     * DEQUE : POP_BACK
     *****************************************************/
    template <class T>
    void deque <T> ::pop_back()
    {
        --numElements;
    }

    /*****************************************************
     * DEQUE : POP_FRONT
     *****************************************************/
    template <class T>
    void deque <T> ::pop_front()
    {
        numElements--;
        ++iaFront;
        if (iaFront == numCapacity)
            iaFront = 0;
    }

    /******************************************************
     * DEQUE : PUSH_BACK
     ******************************************************/
    template <class T>
    void deque <T> ::push_back(const T& t)
    {
        if (numElements == numCapacity) {
            resize(numCapacity * 2);
        }
        data[iaFromID(numElements++)] = t;
        numElements;
    }

    /******************************************************
     * DEQUE : PUSH_FRONT
     ******************************************************/
    template <class T>
    void deque <T> ::push_front(const T& t)
    {
        if (numElements == numCapacity) {
            resize(numCapacity * 2);
        }
        iaFront--;
        if (iaFront < 0) {
            iaFront = numCapacity - 1;
        }
        int testVariable = data[iaFront];
        data[iaFront] = t;
        numElements++;
    }

    /****************************************************
     * DEQUE :: GROW
     * If the deque is currently empty, allocate to size 2.
     * Otherwise, double the size
     ***************************************************/
    template <class T>
    void deque <T> ::resize(int newCapacity)
    {
        if (numCapacity == 0) { //create two spots if size is 0
            data = new T[1];
            this->numCapacity = 1;
            return;
        }
        assert(newCapacity > 0 && newCapacity > numElements);

        T* dataNew = new T[newCapacity]; //the size of T times the number of T's space we want
        for (size_t id = 0; id < numElements; id++)
        {
            dataNew[id] = std::move(this->data[id]);
        }
        this->numCapacity = newCapacity;
        iaFront = 0;
        delete[] data;
        data = dataNew;

    }

} // namespace custom
