/**
 * @file list-inl.h
 * Doubly Linked List (MP 3).
 */

template <class T>
List<T>::~List() {
    clear();
}

template <class T>
void List<T>::clear() {
    ListNode* current = head_;
    while (current != nullptr) {
        ListNode* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head_ = nullptr;
    tail_ = nullptr;
    length_ = 0;
}

template <class T>
void List<T>::insertFront(T const & ndata) {
    ListNode* node = new ListNode(ndata);
    if (head_ == nullptr) {
        head_ = node;
        tail_ = node;
    } else {
        node->next = head_;
        head_->prev = node;
        head_ = node;
    }
    length_++;
}

template <class T>
void List<T>::insertBack(const T & ndata) {
    ListNode* node = new ListNode(ndata);
    if (tail_ == nullptr) {
        head_ = node;
        tail_ = node;
    } else {
        tail_->next = node;
        node->prev = tail_;
        tail_ = node;
    }
    length_++;
}

template <class T>
void List<T>::reverse() {
    reverse(head_, tail_);
}

template <class T>
void List<T>::reverse(ListNode *& startPoint, ListNode *& endPoint) {
    if (startPoint == nullptr || endPoint == nullptr) return;
    ListNode* currNode = startPoint;
    ListNode* tempNode = nullptr;
    ListNode* stop = endPoint->next;

    while (currNode != stop) {
        tempNode = currNode->next;
        currNode->next = currNode->prev;
        currNode->prev = tempNode;
        currNode = tempNode;
    }

    ListNode* oldStart = startPoint;
    startPoint = endPoint;
    endPoint = oldStart;
}

template <class T>
void List<T>::reverseNth(int n) {
    if (n <= 1 || head_ == nullptr) return;
    ListNode* blockStart = head_;
    ListNode* cursor = head_;
    int count = 1;

    while (cursor != nullptr) {
        if (count % n == 0) {
            ListNode* nextBlock = cursor->next;
            reverse(blockStart, cursor);
            if (blockStart->prev != nullptr) {
                blockStart->prev->next = blockStart;
            } else {
                head_ = blockStart;
            }
            if (cursor->next != nullptr) {
                cursor->next->prev = cursor;
            }
            blockStart->next = nextBlock;
            if (nextBlock != nullptr) {
                nextBlock->prev = blockStart;
            } else {
                tail_ = blockStart;
            }
            blockStart = nextBlock;
            cursor = nextBlock;
            count++;
            continue;
        }
        cursor = cursor->next;
        count++;
    }
    if (blockStart != nullptr && blockStart != cursor) {
        reverse(blockStart, cursor == nullptr ? tail_ : cursor->prev);
    }
}

template <class T>
void List<T>::waterfall() {
    if (head_ == nullptr || head_->next == nullptr) return;
    ListNode* curr = head_->next;
    while (curr != nullptr && curr != tail_) {
        ListNode* toMove = curr;
        curr = curr->next;

        toMove->prev->next = toMove->next;
        if (toMove->next) {
            toMove->next->prev = toMove->prev;
        }
        
        tail_->next = toMove;
        toMove->prev = tail_;
        toMove->next = nullptr;
        tail_ = toMove;
        // следующий “удаляемый” — через одну позицию
        if (curr != nullptr) {
            curr = curr->next;
        }
    }
}

template <class T>
List<T> List<T>::split(int splitPoint) {
    if (splitPoint <= 0) {
        List<T> ret;
        ret.head_ = head_;
        ret.tail_ = tail_;
        ret.length_ = length_;
        head_ = nullptr;
        tail_ = nullptr;
        length_ = 0;
        return ret;
    }
    if (splitPoint >= length_) {
        return List<T>();
    }

    ListNode* secondHead = split(head_, splitPoint);
    int oldLength = length_;

    if (secondHead == head_) {
        head_ = nullptr;
        tail_ = nullptr;
        length_ = 0;
    } else {
        tail_ = head_;
        while (tail_->next != nullptr) {
            tail_ = tail_->next;
        }
        length_ = splitPoint;
    }

    List<T> ret;
    ret.head_ = secondHead;
    ret.tail_ = secondHead;
    int count = 1;
    while (ret.tail_->next != nullptr) {
        ret.tail_ = ret.tail_->next;
        count++;
    }
    ret.length_ = oldLength - splitPoint;
    return ret;
}

template <class T>
typename List<T>::ListNode * List<T>::split(ListNode * start, int splitPoint) {
    if (start == nullptr) return nullptr;
    ListNode* current = start;
    for (int i = 1; i < splitPoint && current->next != nullptr; i++) {
        current = current->next;
    }
    ListNode* secondHead = current->next;
    if (secondHead != nullptr) {
        secondHead->prev = nullptr;
    }
    current->next = nullptr;
    return secondHead;
}

template <class T>
void List<T>::mergeWith(List<T> & otherList) {
    head_ = merge(head_, otherList.head_);
    tail_ = head_;
    int total = 0;
    while (tail_ != nullptr && tail_->next != nullptr) {
        tail_ = tail_->next;
        total++;
    }
    length_ = length_ + otherList.length_;

    otherList.head_ = nullptr;
    otherList.tail_ = nullptr;
    otherList.length_ = 0;
}

template <class T>
typename List<T>::ListNode * List<T>::merge(ListNode * first, ListNode* second) {
    if (!first) return second;
    if (!second) return first;

    ListNode* newHead = nullptr;
    if (first->data < second->data) {
        newHead = first;
        first = first->next;
    } else {
        newHead = second;
        second = second->next;
    }
    ListNode* cursor = newHead;

    while (first && second) {
        if (first->data < second->data) {
            cursor->next = first;
            first->prev = cursor;
            first = first->next;
        } else {
            cursor->next = second;
            second->prev = cursor;
            second = second->next;
        }
        cursor = cursor->next;
    }
    if (first) {
        cursor->next = first;
        first->prev = cursor;
    } else if (second) {
        cursor->next = second;
        second->prev = cursor;
    }
    return newHead;
}

template <class T>
void List<T>::sort() {
    if (empty()) return;
    head_ = mergesort(head_, length_);
    tail_ = head_;
    while (tail_->next != nullptr) {
        tail_ = tail_->next;
    }
}

template <class T>
typename List<T>::ListNode* List<T>::mergesort(ListNode * start, int chainLength) {
    if (chainLength <= 1) {
        return start;
    }
    int mid = chainLength / 2;
    ListNode* secondHead = split(start, mid);
    ListNode* leftSorted = mergesort(start, mid);
    ListNode* rightSorted = mergesort(secondHead, chainLength - mid);
    return merge(leftSorted, rightSorted);
}

