
#pragma once
#include <iosfwd>   // for stream operators
#include <stddef.h> // for ptrdiff_t
#include <memory>

namespace hask
{

    namespace detail
    {
        template <typename T>
        class ConstListIterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type        = T;
            using const_reference   = const T&;
            using const_pointer     = const T*;
            using node              = struct node<T>;
            using difference_type   = ptrdiff_t;
            using self              = ConstListIterator<T>;

            node* node_;

            explicit ConstListIterator() : node_() { }
            explicit ConstListIterator(node* node_) : node_(node_) { }

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
                auto tmp = *this;
                node_ = node_->next;
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
        using self            = const List<T>&;
        using value_type      = T;
        using size_type       = size_t;
        using const_reference = const T&;
        using const_iterator  = detail::ConstListIterator<T>;

    private:

        struct node {
            T data;
            std::unique_ptr<T> next;
            node()
                : next(nullptr)
            node(T data)
                : data(data)
                , next(nullptr)
            node(T data, std::unique_ptr<T> next, node* prev)
                : data(data)
        };

        std::unique_ptr<node> root = get_node();
        size_type size_ = 0;

    public:

        friend value_type head(self list)
        {
            return root->data;
        }
        
        friend self tail(self list)
        {
            return root->next;
        }

        bool
        empty() const noexcept
        { return size_ == 0; }

        size_type
        size() const noexcept
        { return size_; }

        friend std::ostream& operator<<(std::ostream& stream, List<T>& _this)
        {
            for(auto e: _this){
                stream << e << " ";
            }
        }

    };

    private:
        void
}
