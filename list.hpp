#pragma once
#include <iosfwd>   // for stream operators
#include <stddef.h> // for ptrdiff_t

namespace detail
{

    template <typename T>
    struct node {
        T data;
        node* next;
        node* prev;
        node() : next(nullptr), prev(nullptr) { }
        node(T data) : data(data), next(nullptr), prev(nullptr) { }
        node(T data, node* next, node* prev) : data(data), next(next), prev(prev) { }
    };

    template <typename T>
    class List_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using reference         = T&;
        using pointer           = T*;
        using node              = struct node<T>;
        using difference_type   = ptrdiff_t;
        using self              = List_iterator<T>;

        node* _node;

        explicit List_iterator() : _node() { }
        explicit List_iterator(node* _node) : _node(_node) { }

        reference
        operator*() const noexcept
        {
            return _node->data;
        }

        pointer
        operator->() const noexcept
        {
            return &_node->data;
        }

        self&
        operator++() noexcept
        {
            _node = _node->next;
            return *this;
        }

        self
        operator++(int) noexcept
        {
            self tmp = *this;
            _node = _node->next;
            return tmp;
        }

        self&
        operator--() noexcept
        {
            _node = _node->prev;
            return *this;
        }

        self
        operator--(int) noexcept
        {
            self tmp = *this;
            _node = _node->prev;
            return tmp;
        }

        friend bool
        operator==(const self& lhs, const self& rhs) noexcept
        {
            return (lhs._node == rhs._node);
        }

        friend bool
        operator!=(const self& lhs, const self& rhs) noexcept
        {
            return (lhs._node != rhs._node);
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
    using iterator        = detail::List_iterator<T>;
    using node            = struct detail::node<T>;
    //using node_ptr        = node*;

private:

    node* first;
    node* last;
    size_type _size = 0;

public:
    explicit List() : first(), last() { }
    
    explicit List(size_type _size) noexcept
        : List(_size, value_type{} ) { }

    explicit List(size_type _size, value_type init) noexcept
    {
        if(_size == 0){
            first = last = nullptr;
        } else {
            first = new node(init);
            node* curr = first;
            curr->next = new node(init);
            for(size_type i = 0; i < _size; ++i) {
                curr->next = new node(init);
                auto tmp = curr;
                curr = curr->next;
                curr->prev = tmp;
            }
            last = curr;
        }
    }

    void push_back(T x)
    {
        if(first == nullptr){
            first = new node(x);
            last = first;
        } else {
            node* new_node = new node(x, nullptr, last);
            last->next = new_node;
            last = new_node;
        }
    }

    size_type
    size() const noexcept
    { return _size; }

    iterator
    begin() const noexcept
    { return iterator(this->first); }

    iterator
    end() const noexcept
    { return iterator(this->last); }

    friend std::istream& operator>>(std::istream& stream, List<T>& _this)
    {
        T temp;
        while(stream >> temp){
            _this.push_back(temp);
        }
        return stream;
    }

    friend std::ostream& operator<<(std::ostream& stream, List<T>& _this)
    {
        stream << "[";
        auto elem = _this.begin();
        const auto last = --_this.end();
        for(;elem != last; ++elem){
            stream << *elem << ", ";
        }
        stream << *++elem << "]\n";
        return stream;
    }
};

