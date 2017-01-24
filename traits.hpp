#pragma once

#include <type_traits>


//#define REQUIRES(...)  typename std::enable_if<(alg::models<__VA_ARGS__>()), int>::type = 0

namespace traits
{

    template< typename T >
      struct type_
        { using type = T; };

    template <typename...>
      struct mp_list { };

    template< size_t v >
	  using int_ = std::integral_constant<size_t, v>;

    using std::bool_constant;
	using std::void_t;
    using std::true_type;
    using std::false_type;
	 
    using zero = int_<0>;
    using one  = int_<1>;

    struct anything_t {
		template <typename T>
		constexpr anything_t(T) { }
		constexpr anything_t() = default;
        template <typename T>
        constexpr operator T() const { }
    };

    template <typename, typename = void>
      struct has_type
        : false_type { };

    template <typename T>
      struct has_type<T, void_t<typename T::type> >
        : true_type { };



    template <typename T, bool = has_type<T>::value>
      struct __type_of_impl
        : type_<typename T::type> { };

    template <typename T>
      struct __type_of_impl<T,false>
        : type_<T> { };

	// Un fel de std::invoke dar pentru template
	// Merge cu Alias si cu structs care au ::type
    template <typename T>
      using type_of = typename __type_of_impl<T>::type;


    namespace detail // ONLY STAFF
    {
        template <typename,template<typename...>class>
          struct mp_rename_impl;

        template <template<typename> class, typename>
          struct mp_transform_impl;

        template <typename>
          struct mp_head_impl;

        template <typename>
          struct mp_tail_impl;

        template <typename, typename>
          struct rebind_container_impl;

        template <
            template <typename...> typename A
            , typename... Ts
            , template <typename...> typename B>
          struct mp_rename_impl< A<Ts...>, B >
            : type_< B<Ts...> > { };

        template <
            template <typename> class F
            , template <typename...> class L
            , typename... Ts>
          struct mp_transform_impl< F, L<Ts...> >
            : type_< L< type_of< F<Ts> >... > > { };

        template <
            template<typename...> class L
            , typename T
            , typename... Ts>
          struct mp_head_impl< L<T,Ts...> >
            : type_<T> { };

        template <
            template<typename...> class L
            , typename T
            , typename... Ts>
          struct mp_tail_impl< L<T,Ts...> >
            : type_<mp_list<Ts...>> { };


        template <
            template<typename,typename> class C
            , typename T
            , typename A
            , typename U>
          struct rebind_container_impl< C<T,A> ,U>
            : type_< C<U,typename A::template rebind<U>::other> > { };

        template <
            template<typename,typename,typename> class C
            , typename T
            , template<typename> class COMP
            , typename A
            , typename U>
          struct rebind_container_impl< C<T,COMP<T>,A> ,U>
            : type_< C<U,COMP<U>,typename A::template rebind<U>::other> > { };

        template <
            template<typename,typename,typename,typename> class C
            , typename T
            , template<typename> class H
            , template<typename> class E
            , typename A
            , typename U>
          struct rebind_container_impl< C<T,H<T>,E<T>,A> ,U>
            : type_< C<U,H<U>,E<U>,typename A::template rebind<U>::other> > { };

    } // NAMESPACE DETAIL


	// Metafunctie pentru a tranfer de argumente
    template<typename A, template<typename...> class B>
      using mp_rename = typename detail::mp_rename_impl<A, B>::type;

	// Metafunctie care returneaza numarul de elem din argumentele template
    template <typename... Args>
      using mp_lenght = int_<sizeof...(Args)>;

	// Metafunctie care returneaza numarul de elem din lista
    template <typename L>
      using mp_size = mp_rename<L,mp_lenght>;

    // Alpica o metafunctie pe o lista
	// Sunt acceptate si structs si alias templtes
    template <template<typename> class F, typename L>
      using mp_transform = typename detail::mp_transform_impl<F,L>::type;

	// Metafunctie care returneaza primul elem al unei liste					
    template <typename L>
      using mp_head = typename detail::mp_head_impl<L>::type;

	// Metafunctie care taie primul elem
    template <typename L>
      using mp_tail = typename detail::mp_tail_impl<L>::type;

	// Metafunctie pentru a schimba tipul unui container
	// Exemplu : using newC = rebind_container<C,T>
    template <typename C, typename T>
      using rebind_container = typename detail::rebind_container_impl<C,T>::type;

/*
    /// implementare de CONCEPTS LITE
    template<class Concept, class Enable = void>
      struct models
        : false_type { };

    template<class Concept, class... Ts>
      struct models<Concept(Ts...), void_t<
        decltype(std::declval<Concept>().requires_(std::declval<Ts>()...))>>
        : true_type { };

    struct Incrementable {
        template< class T >
        auto requires_(T&& x) -> decltype(++x);
    };

    /// Acests este un exemplu de folosire a concepteror
    template< typename T, REQUIRES(Incrementable(T)) >
      void increment(T& x) { ++x; }

*/


    template< typename T, typename... Args >
      struct is_one_of;

    template< typename T >
      struct is_one_of<T>
        : false_type { };

    template< typename T, typename... Args >
      struct is_one_of<T, T, Args...>
        : true_type { };

    template< typename T, typename U, typename... Args >
      struct is_one_of<T, U, Args...>
        : is_one_of <T, Args...> { };



    template< typename C >
      using const_iter = typename C::const_iterator;

    template <typename, typename = void>
      struct is_container
        : false_type { };

    template< typename C >
      struct is_container<C, void_t< decltype(std::declval<const_iter<C>&>() = std::declval<C&>().begin())
                                   , decltype(std::declval<const_iter<C>&>() = std::declval<C&>().end())
                                   >
                         > : true_type { };

    template <typename T, size_t N>
      struct is_container<T[N]> : true_type { };

    template <size_t N>
      struct is_container<char[N]> : false_type { };

    template <typename T1, typename T2>
      struct is_container<std::pair<T1, T2>> : true_type { };



    template <size_t Num>
      struct Factorial
        : int_< Num * Factorial<Num-1>::value> { };

    template <>
      struct Factorial<1>
        : int_<1> { };

    template <>
      struct Factorial<0>
        : int_<1> { };



    template< size_t Base, bool impar = Base%2 >
      struct PowH {
        size_t operator()(size_t x)const{
          return PowH<Base/2>()(x) * PowH<Base/2>()(x);
        }
      };

    template< size_t Base >
      struct PowH <Base,true> {
        size_t operator()(size_t x)const{
          return x * PowH<(Base-1)/2>()(x) * PowH<(Base-1)/2>()(x);
        }
      };

    template <>
      struct PowH <1,true> {
        size_t operator()(size_t x)const{
          return x;
        }
      };

    template <>
      struct PowH <0,false> {
        size_t operator()()const{
          return 1;
        }
      };

    template< size_t Base >
      using Pow = PowH<Base>;





    // folosinta   all_of<std::is_integral, int, float, char> ();
    template <template <class> class Op>
    constexpr bool all_of ()
    {
        return true;
    }

    template <template <class> class Op
               , typename T
               , typename... Ts>
    constexpr bool all_of ()
    {
        return Op<T>::value && all_of<Op,Ts...>();
    }

    template <template <class> class Op>
    constexpr bool any_of ()
    {
        return true;
    }

    template <template <class> class Op
               , typename T, typename... Ts>
    constexpr bool any_of ()
    {
        return Op<T>::value || any_of<Op,Ts...>();
    }

}; // NAMESPACE ALG

