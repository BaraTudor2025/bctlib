#pragma once

#define DEFAULT_FUNDAMENTAL_TYPE_MONOID(T)  template <> struct monoid<T> : default_fundamental_type_monoid<T> { };

#define DEFAULT_CONTAINER_MONOID(T)         template<typename... Args> struct monoid<T<Args...>> : default_container_monoid<T<Args...>> { };

#define DEFAULT_CONTAINER_FOLDABLE(T)       template<typename... Args> struct foldable<T<Args...>> : default_container_foldable<T<Args...>> { };

#define DEFAULT_CONTAINER_MAPPABLE(T)       template<typename... Args> struct mappable<T<Args...>> : default_container_mappable<T<Args...>> { };

#define DEFAULT_CONTAINER_FILTERABLE(T)     template<typename... Args> struct filterable<T<Args...>> : default_container_filterable<T<Args...>> { };

#define DEFAULT_CONTAINER_PACK(T)   \
    DEFAULT_CONTAINER_MONOID(T)     \
    DEFAULT_CONTAINER_FOLDABLE(T)   \
    DEFAULT_CONTAINER_MAPPABLE(T)   \
    DEFAULT_CONTAINER_FILTERABLE(T)

#include "traits.hpp"
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <set>
#include <unordered_set>
#include "utility"

namespace fc
{
    template <typename F>
    constexpr inline auto
    negate(F f) noexcept
    {
        return [=](auto x) { return !f(x); };
    }

    template <typename F, typename T>
    constexpr inline auto
    bind1st(F f, T x) noexcept
    {
        return [=](auto y) { return f(x,y); };
    }

    template <typename F, typename T>
    constexpr inline auto
    bind2st(F f, T y) noexcept
    {
        return [=](auto x) { return f(x,y); };
    }

    template <typename F>
    auto compose(F f)
    {
        return [=](auto x){ return f(x); };
    }

    template <typename F, typename... Fs>
    auto compose(F f, Fs... fs)
    {
        return [=](auto x){ return f(compose(fs...)(x)); };
    }

    template <typename> struct monoid;
    template <typename> struct container_traits;

    template <typename> struct foldable;
    template <typename> struct mappable;
    template <typename> struct filterable;

    template <typename> struct sequence_container_traits;
    template <typename> struct associative_container_traits;
    template <typename> struct hash_container_traits;
    template <typename> struct string_container_traits;
    template <typename> struct matrix_container_traits;

    template <typename T>
    auto fold(const T& in)
    {
        return foldable<T>::fold(in);
    };

    template <typename F, typename B, typename T>
    auto foldl(F&& f, B&& b, const T& in)
    {
        return foldable<T>::foldl(std::forward<F>(f),std::forward<B>(b),in);
    }

    template <typename F, typename T>
    auto map(F&& f, const T& in)
    {
        return mappable<T>::map(std::forward<F>(f),in);
    }

    template <typename F, typename T>
    auto filter(F&& f, const T& in)
    {
        return filterable<T>::filter(std::forward<F>(f),in);
    }


	// structuri pentru operatii cu containere 
	// container_traits va mostenii de la fiecare 
	// container_traits este filosit in implementare functiilor: map, filter, fold


	// operatii pentru std::vector, std::deque, std::list
    template
		<
        template <class,class> class C,
        typename T,
        typename A
        >
    struct sequence_container_traits< C <T ,A> >
    {
        static void add_element(C<T,A>& c, const T& t)
        {
            c.push_back(t);
        }

        static void concat(C<T,A>& lhs, const C<T,A>& rhs)
        {
            using std::begin;
            using std::end;

            lhs.insert(end(lhs),begin(rhs),end(rhs));
        }
		
        template <typename U>
        using rebind = C <U,typename A::template rebind<U>::other >;

        using value_type = T;

	};


	// operatii pentru std::set
    template
		<
        template <class,class,class> class C,
        typename T,
        template <class> class Comp,
        typename A
        >
    struct associative_container_traits< C <T, Comp<T>, A> >
    {
        static void add_element(C<T,Comp<T>,A>& c, const T& t)
        {
            c.insert(t);
        }

        static void concat(C<T,Comp<T>,A>& lhs, const C<T,Comp<T>,A>& rhs)
        {
            using std::begin;
            using std::end;

            lhs.insert(begin(rhs), end(rhs));
        }

        template <typename U>
        using rebind = C<U, Comp<U>, typename A::template rebind<U>::other>;

        using value_type = T;

    };

	// operatii pentru std::unordered_set
    template
		<
        template <class,class,class,class> class C,
        typename T,
        template <class> class H,
        template <class> class E,
        typename A
        >
    struct hash_container_traits< C <T, H<T>, E<T>, A> >
    {
        static void add_element(C<T,H<T>,E<T>,A>& c, const T& t)
        {
            c.insert(t);
        }

        static void concat(C<T,H<T>,E<T>,A>& lhs, const C<T,H<T>,E<T>,A>& rhs)
        {
            using std::begin;
            using std::end;

            lhs.insert(begin(rhs), end(rhs));
        }

        template <typename U>
        using rebind = C<U, H<U>, E<U>,typename A::template rebind<U>::other>;

        using value_type = T;

    }; 

	// operatii pentru std::string
    template
		<
        template <class,class,class> class S,
        typename C,
        template <class> class T,
        typename A
        >
    struct string_container_traits< S <C, T<C>, A> >
    {
        static void add_element(S<C,T<C>,A>& s, const C& c)
        {
            s.push_back(c);
        }

        static void concat(S<C,T<C>,A>& lhs, const S<C,T<C>,A>& rhs)
        {
            lhs += rhs;
        }

        template <typename U>
        using rebind = S <U, T<U>, typename A::template rebind<U>::other>;

        using value_type = C;

    }; 


    template <typename... Args> struct container_traits< std::vector <Args...> > : sequence_container_traits< std::vector <Args...> > {};
    template <typename... Args> struct container_traits< std::list   <Args...> > : sequence_container_traits< std::list   <Args...> > {};
    template <typename... Args> struct container_traits< std::deque  <Args...> > : sequence_container_traits< std::deque  <Args...> > {};

    template <typename... Args> struct container_traits< std::set      <Args...> > : associative_container_traits< std::set      <Args...> > {};
    template <typename... Args> struct container_traits< std::multiset <Args...> > : associative_container_traits< std::multiset <Args...> > {};

    template <typename... Args> struct container_traits< std::basic_string <Args...> > : string_container_traits< std::basic_string <Args...> > {};

    template <typename... Args> struct container_traits< std::unordered_set      <Args...> > : hash_container_traits< std::unordered_set      <Args...> > {};
    template <typename... Args> struct container_traits< std::unordered_multiset <Args...> > : hash_container_traits< std::unordered_multiset <Args...> > {};

    //template <typename... Args> struct container_traits< bct::Matrix <Args...> > : matrix_container_traits< bct::Matrix <Args...> > {};


    template <typename T>
    struct monoid
    {
        // T empty();
        // T append();
        static constexpr bool is_instance = false;
    };

    template <typename T>
    struct default_fundamental_type_monoid
    {
        static T empty() { return T{}; }
        static T append(const T& lhs, const T& rhs) { return lhs + rhs; }
        static constexpr bool is_instance = true;
    };

    template <typename T>
    struct default_container_monoid
    {
        static T empty() { return T{}; }
        static T append(T lhs, const T& rhs)
        {
            container_traits<T>::concat(lhs,rhs);
            return lhs;
        }
        static constexpr bool is_instance = true;
    };

    template <typename T>
    struct default_container_foldable // FOLD
    {
        template <typename U>
        using value_type = typename container_traits<U>::value_type;

        template<
            typename U,
            typename = std::enable_if_t<std::is_same<T,U>::value>,
            typename = std::enable_if_t<monoid<value_type<U>>::is_instance>
            >
        static auto fold(const U& in)
            -> decltype(foldl(monoid<value_type<U>>::append, monoid<value_type<U>>::empty(),in))
        {
            return foldl(monoid<value_type<U>>::append,
                         monoid<value_type<U>>::empty(),in);
        }

        template <typename F, typename B>
        static auto foldl(F&& f, B b, const T& in) -> B
        {
            for(const auto& elem : in)
                b = util::eval(std::forward<F>(f),b,elem);
            return b;
        }
    };

    template<typename T>
    struct default_container_mappable // MAP
    {
        template <typename U>
        using value_type = typename container_traits<U>::value_type;

        template<class U>
        using remove_cv_ref_t = typename std::remove_cv<typename std::remove_reference<U>::type>::type;

        template<typename F, typename U>
        static auto map(F&& f, const U& in)
            -> typename container_traits<T>::template rebind<
                remove_cv_ref_t<decltype(eval(std::forward<F>(f),std::declval<value_type<U>>()))>>
        {
            using N = remove_cv_ref_t<decltype(eval(std::forward<F>(f),std::declval<value_type<U>>()))>;
            using R = typename container_traits<T>::template rebind<N>;

            R out;

            for (const auto& elem : in)
                container_traits<R>::add_element(out,eval(std::forward<F>(f),elem));

            return out;
        }
    };

    template<typename T>
    struct mappable // MAP
    {
        template <typename U>
        using value_type = typename container_traits<U>::value_type;

        template<class U>
        using remove_cv_ref_t = typename std::remove_cv<typename std::remove_reference<U>::type>::type;

        template<typename F, typename U>
        static auto map(F&& f, const U& in)
            -> typename container_traits<T>::template rebind<
                remove_cv_ref_t<decltype(eval(std::forward<F>(f),std::declval<value_type<U>>()))>>
        {
            using N = remove_cv_ref_t<decltype(eval(std::forward<F>(f),std::declval<value_type<U>>()))>;
            using R = typename container_traits<T>::template rebind<N>;

            R out(in.line(),in.coll());
            auto out_iter = out.begin();

            for (const auto& elem : in)
                *(out_iter++) = eval(std::forward<F>(f),elem);

            return out;
        }
    };

    template <typename T>
    struct default_container_filterable // FILTER
    {
        template <typename F>
        static auto filter(F&& f, const T& in) -> T
        {
            T out;

            for(const auto& elem : in)
                if(util::eval(std::forward<F>(f),elem))
                    container_traits<T>::add_element(out,elem);
            return out;
        }
    };



    DEFAULT_FUNDAMENTAL_TYPE_MONOID (bool)
    DEFAULT_FUNDAMENTAL_TYPE_MONOID (char)
    DEFAULT_FUNDAMENTAL_TYPE_MONOID (unsigned char)
    DEFAULT_FUNDAMENTAL_TYPE_MONOID (signed char)
    DEFAULT_FUNDAMENTAL_TYPE_MONOID (char16_t)
    DEFAULT_FUNDAMENTAL_TYPE_MONOID (char32_t)
    DEFAULT_FUNDAMENTAL_TYPE_MONOID (short)
    DEFAULT_FUNDAMENTAL_TYPE_MONOID (unsigned short)
    DEFAULT_FUNDAMENTAL_TYPE_MONOID (int)
    DEFAULT_FUNDAMENTAL_TYPE_MONOID (unsigned)
    DEFAULT_FUNDAMENTAL_TYPE_MONOID (long)
    DEFAULT_FUNDAMENTAL_TYPE_MONOID (unsigned long)
    DEFAULT_FUNDAMENTAL_TYPE_MONOID (long long)
    DEFAULT_FUNDAMENTAL_TYPE_MONOID (unsigned long long)
    DEFAULT_FUNDAMENTAL_TYPE_MONOID (float)
    DEFAULT_FUNDAMENTAL_TYPE_MONOID (double)
    DEFAULT_FUNDAMENTAL_TYPE_MONOID (std::string)

    DEFAULT_CONTAINER_PACK (std::vector)
    DEFAULT_CONTAINER_PACK (std::deque)
    DEFAULT_CONTAINER_PACK (std::list)
    DEFAULT_CONTAINER_PACK (std::basic_string)
    DEFAULT_CONTAINER_PACK (std::set)
    DEFAULT_CONTAINER_PACK (std::multiset)
    DEFAULT_CONTAINER_PACK (std::unordered_set)
    DEFAULT_CONTAINER_PACK (std::unordered_multiset)
}

#undef DEFAULT_FUNDAMENTAL_TYPE_MONOID
#undef DEFAULT_CONTAINER_MONOID
#undef DEFAULT_CONTAINER_FOLDABLE
#undef DEFAULT_CONTAINER_MAPPABLE
#undef DEFAULT_CONTAINER_FILTERABLE
#undef DEFAULT_CONTAINER_PACK
