#pragma once

namespace bct
{
    namespace detail
    {
        template <typename T>
        struct node {
            node() : next(), prev(), data(0) { }
            node(T data) : next(), prev(), data(data) { }
            node* next;
            node* prev;
            T data;
        };

        template <typename T>
        class List_iterator {
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type        = T;
            using reference         = T&;
            using pointer           = T*;
            using node              = node<T>;
            using difference_type   = ptrdiff_t;
            using _self             = List_iterator<T>;

            node* _node;

        public:
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

            _self&
            operator++() noexcept
            {
                _node = _node->next;
                return *this;
            }

            _self
            operator++(int) noexcept
            {
                _self tmp = *this;
                _node = _node->next;
                return tmp;
            }

            _self&
            operator--() noexcept
            {
                _node = _node->prev;
                return _node;
            }

            _self
            operator--(int) noexcept
            {
                _self tmp = *this;
                _node = _node->prev;
                return tmp;
            }

            friend bool
            operator==(const _self& lhs, const _self& rhs) noexcept
            {
                return (lhs._node == rhs._node);
            }

            friend bool
            operator!=(const _self& lhs, const _self& rhs) noexcept
            {
                return (lhs._node != rhs._node);
            }
        };
    }

    template <typename T>
    class List {

        public:
            using _self           = List<T>&;
            using value_type      = T;
            using size_type       = size_t;
            using reference       = T&;
            using pointer         = T*;
            using const_reference = const T&;
            using iterator        = detail::List_iterator<T>;
            using node            = detail::node<T>;
            using nodePtr         = node*;

            using ostream = std::ostream&;
            using istream = std::istream&;

            explicit
            List(size_type _size, value_type init = 0) noexcept
                : _head(new node)
                 ,_size(_size)
            {
                node* curr = _head;
                curr->next = new node(init);
                for(size_type i = 0; i <= _size; ++i) {
                curr->next = new node(init);
                auto tmp = curr;
                curr = curr->next;
                curr->prev = tmp;
            }
        }

        size_type
        size() const noexcept
        { return _size; }

        iterator
        begin() noexcept
        { return iterator(this->_head->next); }

        iterator
        end() noexcept
        { return iterator(this->_head); }


    private:

        nodePtr _head;
        size_type _size = 0;
};
}
