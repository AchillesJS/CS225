/**
 * @file heap.cpp
 * Implementation of a heap class.
 */

#include "heap.h"
#include <algorithm>

template <class T, class Compare>
size_t heap<T, Compare>::root() const
{
    return 0;
}

template <class T, class Compare>
size_t heap<T, Compare>::leftChild(size_t currentIdx) const
{
    return 2 * currentIdx + 1;
}

template <class T, class Compare>
size_t heap<T, Compare>::rightChild(size_t currentIdx) const
{
    return 2 * currentIdx + 2;
}

template <class T, class Compare>
size_t heap<T, Compare>::parent(size_t currentIdx) const
{
    return (currentIdx - 1) / 2;
}

template <class T, class Compare>
bool heap<T, Compare>::hasAChild(size_t currentIdx) const
{
    return leftChild(currentIdx) < _elems.size();
}

template <class T, class Compare>
size_t heap<T, Compare>::maxPriorityChild(size_t currentIdx) const
{
    size_t leftIdx = leftChild(currentIdx);
    size_t rightIdx = rightChild(currentIdx);
    if (rightIdx >= _elems.size()) {
        return leftIdx;
    }
    if (higherPriority(_elems[leftIdx], _elems[rightIdx])) {
        return leftIdx;
    }
    return rightIdx;
}

template <class T, class Compare>
void heap<T, Compare>::heapifyDown(size_t currentIdx)
{
    while (hasAChild(currentIdx)) {
        size_t c = maxPriorityChild(currentIdx);
        if (higherPriority(_elems[c], _elems[currentIdx])) {
            std::swap(_elems[currentIdx], _elems[c]);
            currentIdx = c;
        } else {
            break;
        }
    }
}

template <class T, class Compare>
void heap<T, Compare>::heapifyUp(size_t currentIdx)
{
    if (currentIdx == root()) return;
    size_t p = parent(currentIdx);
    if (higherPriority(_elems[currentIdx], _elems[p])) {
        std::swap(_elems[currentIdx], _elems[p]);
        heapifyUp(p);
    }
}

template <class T, class Compare>
heap<T, Compare>::heap()
    : _elems(), higherPriority(Compare())
{
}

template <class T, class Compare>
heap<T, Compare>::heap(const std::vector<T>& elems)
    : _elems(elems), higherPriority(Compare())
{
    if (_elems.empty()) return;
    int lastParent = static_cast<int>(parent(_elems.size() - 1));
    for (int idx = lastParent; idx >= 0; --idx) {
        heapifyDown(static_cast<size_t>(idx));
    }
}

template <class T, class Compare>
T heap<T, Compare>::pop()
{
    if (empty()) {
        throw std::out_of_range("pop() called on empty heap");
    }
    T topElem = _elems[root()];
    size_t lastIdx = _elems.size() - 1;
    _elems[root()] = _elems[lastIdx];
    _elems.pop_back();
    if (!empty()) {
        heapifyDown(root());
    }
    return topElem;
}

template <class T, class Compare>
T heap<T, Compare>::peek() const
{
    if (empty()) {
        throw std::out_of_range("peek() called on empty heap");
    }
    return _elems[root()];
}

template <class T, class Compare>
void heap<T, Compare>::push(const T& elem)
{
    _elems.push_back(elem);
    heapifyUp(_elems.size() - 1);
}

template <class T, class Compare>
bool heap<T, Compare>::empty() const
{
    return _elems.empty();
}

template <class T, class Compare>
void heap<T, Compare>::getElems(std::vector<T> & heaped) const
{
    for (size_t i = root(); i < _elems.size(); i++) {
        heaped.push_back(_elems[i]);
    }
}

