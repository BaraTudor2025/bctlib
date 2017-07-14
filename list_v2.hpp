#pragma once
#include <iosfwd>   // for stream operators
#include <stddef.h> // for ptrdiff_t
#include <memory>   // for unique_ptr

namespace detail
{

    template <typename T>
    struct node {
        T data;
        std::unique_ptr<node> next;
        node* prev;
        node()
            : next(nullptr)
            , prev(nullptr) { }
        node(T data)
            : data(data)
            , next(nullptr)
            , prev(nullptr) { }
        node(T data, std::unique_ptr<node> next, node* prev)
            : data(data)
            , next(next)
            , prev(prev) { }
    };

    template <typename T>
    class list_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using reference         = T&;
        using pointer           = T*;
        using node              = struct node<T>;
        using difference_type   = ptrdiff_t;
        using self              = list_iterator<T>;

        node* node_;

        explicit list_iterator() : node_() { }
        explicit list_iterator(node* node_) : node_(node_) { }

        reference
        operator*() noexcept
        {
            return node_->data;
        }

        pointer
        operator->() noexcept
        {
            return &node_->data;
        }

        self&
        operator++() noexcept
        {
            node_ = node_->next;
            return *this;
        }

        self
        operator++(int) noexcept
        {
            self tmp = *this;
            node_ = node_->next;
            return tmp;
        }

        self&
        operator--() noexcept
        {
            node_ = node_->prev;
            return *this;
        }

        self
        operator--(int) noexcept
        {
            self tmp = *this;
            node_ = node_->prev;
            return tmp;
        }

        friend bool
        operator==(const self& lhs, const self& rhs) noexcept
        {
            return (lhs.node_ == rhs.node_);
        }

        friend bool
        operator!=(const self& lhs, const self& rhs) noexcept
        {
            return (lhs.node_ != rhs.node_);
        }
    };
   
    template <typename T>
    class const_list_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using const_reference   = const T&;
        using const_pointer     = const T*;
        using node              = struct node<T>;
        using difference_type   = ptrdiff_t;
        using self              = const_list_iterator<T>;

        node* node_;

        explicit const_list_iterator() : node_() { }
        explicit const_list_iterator(node* node_) : node_(node_) { }

        const_reference
        operator*() const noexcept
        {
            return node_->data;
        }

        const_pointer
        operator->() const noexcept
        {
            return &node_->data;
        }

        self&
        operator++() noexcept
        {
            node_ = node_->next;
            return *this;
        }

        self
        operator++(int) noexcept
        {
            self tmp = *this;
            node_ = node_->next;
            return tmp;
        }

        self&
        operator--() noexcept
        {
            node_ = node_->prev;
            return *this;
        }

        self
        operator--(int) noexcept
        {
            self tmp = *this;
            node_ = node_->prev;
            return tmp;
        }

        friend bool
        operator==(const self& lhs, const self& rhs) noexcept
        {
            return (lhs.node_ == rhs.node_);
        }

        friend bool
        operator!=(const self& lhs, const self& rhs) noexcept
        {
            return (lhs.node_ != rhs.node_);
        }
    };
    
} // namespace detail

template <typename T>
class List {

public:
    using self           = List<T>&;
    using value_type      = T;
    using size_type       = size_t;
    using reference       = T&;
    using pointer         = T*;
    using const_reference = const T&;
    using iterator        = detail::list_iterator<T>;
    using const_iterator  = detail::const_list_iterator<T>;
    using node            = struct detail::node<T>;

private:

    node node_;
    size_type size_;

public:
    explicit List(): size_(0)
    {
        node_.next = &node_;
        node_.prev = &node_;
    }

    explicit List(size_type size_) noexcept : List(size_, value_type{} ) { }

    explicit List(size_type size_, value_type init) noexcept : size_(size_)
    {
        if(size_ == 0){
            List();
        } else {
            node_.next = std::make_unique<node>(init);
            node* curr = node_;
            for(size_type i = 0; i < size_; ++i) {
                curr->next = new node(i+1);
                auto tmp = curr;
                curr = curr->next;
                curr->prev = tmp;
            }
            link_head_and_last();
        }
    }

    void push_back(const_reference x)
    {
        ++size_;
        auto tmp = std::make_unique<node>(x);
        if(this->empty()){

        } else {
            last_->next = tmp;
            tmp->prev = last_;
            last_ = tmp;
            //last_ = node_insert_after(last_, tmp);
        }
    }

    size_type
    size() const noexcept
    { return this->size_; }

    iterator
    begin() noexcept
    { return iterator(this->first_); }

    iterator
    end() noexcept
    { return iterator(this->last_); }

    reference
    front () noexcept
    { return *begin(); }

    reference
    back() noexcept
    { return *end(); }


    const_iterator
    cbegin() const noexcept
    { return const_iterator(this->first_); }

    const_iterator
    cend() const noexcept
    { return const_iterator(this->last_); }

    const_reference
    front () const noexcept
    { return *cbegin(); }

    const_reference
    back() const noexcept
    { return *cend(); }

    bool
    empty() const noexcept
    { return size_ == 0; }

    friend std::istream& operator>>(std::istream& stream, List<T>& _this)
    {
        T temp;
        while(stream >> temp){
            _this.push_back(temp);
        }
        return stream;
    }

    friend std::ostream& operator<<(std::ostream& stream, const List<T>& _this)
    {
        for(const auto& e: _this){
            stream << e << " ";
        }
        /*
        if(!_this.empty()){
            stream << "[";
            auto elem = _this.begin();
            const auto last_ = --_this.end();
            for(;elem != last_; ++elem){
                stream << *elem << ", ";
            }
            stream << *++elem << "]\n";
            return stream;
        } else {
            stream << "[]";
        }*/
    }
private:

    // before A <---> C
    // after  A <---> B <---> C
    void node_insert_after(node* pos1, node* tmp){
        auto pos3 = pos1->next;
        tmp->prev = pos1;
        tmp->next = pos3;
        pos1->next = tmp;
        pos3->prev = tmp;
    }

};
