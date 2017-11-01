#pragma once

#include <type_traits>
#include <fstream>
#include <chrono>
#include <vector>
#include <random>
#include <numeric>

inline namespace util
{
    auto range(size_t li, size_t ls)
    {
        std::vector<int> v(ls-li+1);
		std::iota(v.begin(), v.end(), li);
        return v;
    }

    inline auto random_seed() noexcept
    {
        return std::chrono::system_clock::now().time_since_epoch().count();
    }

    struct file_guard {
        using file_type = std::fstream;
        file_type file;

        template <typename... Args>
        file_guard(Args... args) : file(args...){ }

        ~file_guard() { file.close(); }

        template <typename T>
        file_guard&
        operator<<(T const& t)
        { file << t; return *this; }

        template <typename T>
        file_guard&
        operator>>(T const& t)
        { file >> t; return *this; }
    };

    template <typename file_type>
    class file_closer {
        file_type& file;
    public:
        file_closer(file_type& f) : file(f) { }
        ~file_closer() { file.close(); }
    };

    template <typename T>
    std::enable_if_t<std::is_pointer<T>::value>
    delete_safe(T ptr)
    {
        if(ptr)
            delete ptr;
    }

    template<
        class F, class... Args,
        class = std::enable_if_t<!std::is_member_function_pointer<F>::value>,
        class = std::enable_if_t<!std::is_member_object_pointer<F>::value>
        >
    auto eval(F&& f, Args&&... args) -> decltype(f(std::forward<Args>(args)...))
    {
        return f(std::forward<Args>(args)...);
    }

    template<class R, class C, class... Args>
    auto eval(R(C::*f)() const, const C& c, Args&&... args) -> R
    {
        return (c.*f)(std::forward<Args>(args)...);
    }

    template<class R, class C, class... Args>
    auto eval(R(C::*f)() const, C& c, Args&&... args) -> R
    {
        return (c.*f)(std::forward<Args>(args)...);
    }

    template<class R, class C, class... Args>
    auto eval(R(C::*f)(), C& c, Args&&... args) -> R
    {
        return (c.*f)(std::forward<Args>(args)...);
    }

    template<class R, class C>
    auto eval(R(C::*m), const C& c) -> const R&
    {
        return c.*m;
    }

    template<class R, class C>
    auto eval(R(C::*m), C& c) -> R&
    {
        return c.*m;
    }
};


