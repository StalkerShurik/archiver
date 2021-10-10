#include "my_heap.h"


/*void MyHeap<T>::Insert(T x) {
    if (heap_size_ == heap_storage_.size()) {
        heap_storage_.resize(2 * heap_size_ + 1);
    }
    heap_storage_[heap_size_] = x;
    size_t i = heap_size_;
    heap_size_++;
    while (i > 0 && heap_storage_[(i - 1) / 2] > heap_storage_[i]) {
        std::swap(heap_storage_[i], heap_storage_[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}*/

/*T MyHeap<T>::GetMinAndDelete() {
    T ans = heap_storage_[0];
    std::swap(heap_storage_[0], heap_storage_[heap_size_ - 1]);
    heap_size_--;
    size_t i = 0;
    while (true) {
        size_t j = i;
        if (2 * i + 1 < heap_size_ && heap_storage_[j] > heap_storage_[2 * i + 1]) {
            j = 2 * i + 1;
        }
        if (2 * i + 2 < heap_size_ && heap_storage_[j] > heap_storage_[2 * i + 2]) {
            j = 2 * i + 2;
        }
        if (j == i) {
            break;
        }
        std::swap(heap_storage_[j], heap_storage_[i]);
        i = j;
    }
    return ans;
}*/


/*void MyHeap<T>::Clear() {
    heap_size_ = 0;
    heap_storage_.clear();
}*/

/*size_t MyHeap<T>::Size() {
    return heap_size_;
}*/

/*T MyHeap<T>::GetMin() {
    if (heap_size_ > 0) {
        return heap_storage_[0];
    } else {
        std::cerr << "I want min element but heap is empty";
        throw ;
    }
}*/

/*MyHeap<T>::~MyHeap() {
    heap_storage_.clear();
    heap_size_ = 0;
}*/