#pragma once
#include <stddef.h>  // for ptrdiff_t
#include <iosfwd>    // for stream operators

namespace detail {

template <typename T>
struct node {
    T data;
    node* next;
    node* prev;
    node() : next(nullptr), prev(nullptr) {}
    node(T data) : data(data), next(nullptr), prev(nullptr) {}
    node(T data, node* next, node* prev) : data(data), next(next), prev(prev) {}
};

template <typename T>
class List_iterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using reference = T&;
    using pointer = T*;
    using node = struct node<T>;
    using difference_type = ptrdiff_t;
    using self = List_iterator<T>;

    node* node_;

    explicit List_iterator() : node_() {}
    explicit List_iterator(node* node_) : node_(node_) {}

    reference operator*() const noexcept { return node_->data; }

    pointer operator->() const noexcept { return &node_->data; }

    self& operator++() noexcept {
        node_ = node_->next;
        return *this;
    }

    self operator++(int)noexcept {
        self tmp = *this;
        node_ = node_->next;
        return tmp;
    }

    self& operator--() noexcept {
        node_ = node_->prev;
        return *this;
    }

    self operator--(int)noexcept {
        self tmp = *this;
        node_ = node_->prev;
        return tmp;
    }

    friend bool operator==(const self& lhs, const self& rhs) noexcept {
        return (lhs.node_ == rhs.node_);
    }

    friend bool operator!=(const self& lhs, const self& rhs) noexcept {
        return (lhs.node_ != rhs.node_);
    }
};
}  // namespace detail

template <typename T>
class List {
   public:
    using self = List<T>&;
    using value_type = T;
    using size_type = size_t;
    using reference = T&;
    using pointer = T*;
    using const_reference = const T&;
    using iterator = detail::List_iterator<T>;
    using node = struct detail::node<T>;

   private:
    node* first_;
    node* last_;
    size_type size_;

   public:
    explicit List() : size_(0) { first_ = last_ = nullptr; }

    explicit List(size_type size_) noexcept : List(size_, value_type{}) {}

    explicit List(size_type size_, value_type init) noexcept {
        if (size_ == 0) {
            first_ = last_ = nullptr;
        } else {
            first_ = new node(init);
            node* curr = first_;
            for (size_type i = 0; i < size_; ++i) {
                curr->next = new node(init);
                auto tmp = curr;
                curr = curr->next;
                curr->prev = tmp;
            }
            last_ = curr;
        }
        // de rezolvat linkurile
        link_head_last();
    }

    void push_back(const_reference x) {
        ++size_;
        node* tmp = new node(x);
        if (first_ == nullptr) {
            first_ = last_ = tmp;
            link_head_last();
        } else {
            last_ = node_insert_after(last_, tmp);
        }
    }

    size_type size() const noexcept { return this->size_; }

    iterator begin() const noexcept { return iterator(this->first_); }

    iterator end() const noexcept { return iterator(this->last_); }

    reference front() noexcept { return *begin(); }

    reference back() noexcept { return *end(); }

    bool empty() const noexcept { return this->first_ == this->last_; }

    friend std::istream& operator>>(std::istream& stream, List<T>& _this) {
        T temp;
        while (stream >> temp) {
            _this.push_back(temp);
        }
        return stream;
    }

    friend std::ostream& operator<<(std::ostream& stream, List<T>& _this) {
        // test pt link
        for (auto e : _this) {
            stream << e << " ";
        }
    }

   private:
    // before A <---> C
    // after  A <---> B <---> C
    // return B
    node* node_insert_after(node* pos1, node* tmp) {
        node* pos3 = pos1->next;
        tmp->prev = pos1;
        tmp->next = pos3;
        pos1->next = tmp;
        pos3->prev = tmp;
        return tmp;
    }

    void link_head_last() {
        first_->next = last_;
        first_->prev = last_;
        last_->next = first_;
        last_->prev = first_;
    }
};
