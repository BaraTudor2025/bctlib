#pragma once

#include <>

template <typename T, size_t N>
class small_vector {

private:
    int capacity_;
    T* begin_, end_;
    T buffer_[N];

public:

    using iterator = T*;
    using const_iterator = const T*;

    small_vector(): begin_(buffer_), end_(buffer_), capacity_(N) {  }

    iterator begin() { return begin_; }
    iterator end() { return end_; }
    const_iterator cbegin() { return begin_; }
    const_iterator cend() { return end_; }

    void push_back(const T& value)
    {
        if(size() < capacity_){
            *end_ = value;
            ++end_;
        } else {
            // allocate
        }
    }

    void push_back(T&& value)
    {
        if(size() < capacity_){
            *end_ = value;
            ++end_;
        } else {
            // allocate
        }
    }

    int size() const { return end_ - begin_; }

};


