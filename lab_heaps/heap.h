/**
 * @file heap.h
 * Definition of a heap class.
 */

#ifndef _HEAP_H_
#define _HEAP_H_

#include <vector>
#include <functional>
#include <stdexcept>

// forward declare descriptor (used for printing, ignore)
template <class Type, class Comp>
class HeapNodeDescriptor;

/**
 * heap: A priority queue implemented as a heap.
 *
 * @author Chase Geigle
 * @date Fall 2012
 */
template <class T, class Compare = std::less<T>>
class heap
{
  public:
    heap();
    heap(const std::vector<T>& elems);

    T pop();
    T peek() const;
    void push(const T& elem);
    bool empty() const;
    void getElems(std::vector<T> & heaped) const;

    template <class Type, class Comp>
    friend std::ostream& operator<<(std::ostream& out,
                                    const heap<Type, Comp>& toPrint);

    friend class HeapNodeDescriptor<T, Compare>;

  private:
    std::vector<T> _elems;
    Compare higherPriority;

    size_t root() const;
    size_t leftChild(size_t currentIdx) const;
    size_t rightChild(size_t currentIdx) const;
    size_t parent(size_t currentIdx) const;
    bool hasAChild(size_t currentIdx) const;
    size_t maxPriorityChild(size_t currentIdx) const;
    void heapifyDown(size_t currentIdx);
    void heapifyUp(size_t currentIdx);
};

#include "heap.cpp"
#include "printheap.cpp"

#endif

