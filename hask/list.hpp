#pragma once
#include <iosfwd>   // for stream operators
#include <stddef.h> // for ptrdiff_t
#include <memory>

namespace hask
{

    template <typename T>
    class List {

    public:
        using value_type      = T;
        using size_type       = size_t;

    private:

        struct node {
            T data;
            std::shared_ptr<T> next;
            node(T data, std::shared_ptr<T> next)
                : data(data)
                , next(next) { }
        };

        std::shared_ptr<node> root;

        List(value_type data, List tail)
            : root(std::make_shared<node>(data, tail->root)) { }

    public:

        friend value_type head(List list)
        {
            return root->data;
        }

        friend List tail(List list)
        {
            return root->next;
        }

        friend List cons(value_type data, List tail)
        {
            return List(data, tail);
        }

        bool empty()
        {
            return !root;
        }
    };

}
