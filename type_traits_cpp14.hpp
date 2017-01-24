
namespace std
{

  /// Alias template for remove_const
  template<typename _Tp>
    using remove_const_t = typename remove_const<_Tp>::type;

  /// Alias template for remove_volatile
  template<typename _Tp>
    using remove_volatile_t = typename remove_volatile<_Tp>::type;

  /// Alias template for remove_cv
  template<typename _Tp>
    using remove_cv_t = typename remove_cv<_Tp>::type;

  /// Alias template for add_const
  template<typename _Tp>
    using add_const_t = typename add_const<_Tp>::type;

  /// Alias template for add_volatile
  template<typename _Tp>
    using add_volatile_t = typename add_volatile<_Tp>::type;

  /// Alias template for add_cv
  template<typename _Tp>
    using add_cv_t = typename add_cv<_Tp>::type;

  /// Alias template for remove_reference
  template<typename _Tp>
    using remove_reference_t = typename remove_reference<_Tp>::type;

  /// Alias template for add_lvalue_reference
  template<typename _Tp>
    using add_lvalue_reference_t = typename add_lvalue_reference<_Tp>::type;

  /// Alias template for add_rvalue_reference
  template<typename _Tp>
    using add_rvalue_reference_t = typename add_rvalue_reference<_Tp>::type;

  /// Alias template for make_signed
  template<typename _Tp>
    using make_signed_t = typename make_signed<_Tp>::type;

  /// Alias template for make_unsigned
  template<typename _Tp>
    using make_unsigned_t = typename make_unsigned<_Tp>::type;

  /// Alias template for remove_extent
  template<typename _Tp>
    using remove_extent_t = typename remove_extent<_Tp>::type;

  /// Alias template for remove_all_extents
  template<typename _Tp>
    using remove_all_extents_t = typename remove_all_extents<_Tp>::type;

      /// Alias template for remove_pointer
  template<typename _Tp>
    using remove_pointer_t = typename remove_pointer<_Tp>::type;

  /// Alias template for add_pointer
  template<typename _Tp>
    using add_pointer_t = typename add_pointer<_Tp>::type;

  /// Alias template for aligned_storage
  template<size_t _Len, size_t _Align =
	    __alignof__(typename __aligned_storage_msa<_Len>::__type)>
    using aligned_storage_t = typename aligned_storage<_Len, _Align>::type;

  template <size_t _Len, typename... _Types>
    using aligned_union_t = typename aligned_union<_Len, _Types...>::type;

  /// Alias template for decay
  template<typename _Tp>
    using decay_t = typename decay<_Tp>::type;

  /// Alias template for enable_if
  template<bool _Cond, typename _Tp = void>
    using enable_if_t = typename enable_if<_Cond, _Tp>::type;

  /// Alias template for conditional
  template<bool _Cond, typename _Iftrue, typename _Iffalse>
    using conditional_t = typename conditional<_Cond, _Iftrue, _Iffalse>::type;

  /// Alias template for common_type
  template<typename... _Tp>
    using common_type_t = typename common_type<_Tp...>::type;

  /// Alias template for underlying_type
  template<typename _Tp>
    using underlying_type_t = typename underlying_type<_Tp>::type;

  /// Alias template for result_of
  template<typename _Tp>
    using result_of_t = typename result_of<_Tp>::type;
}

