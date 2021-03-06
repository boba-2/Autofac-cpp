#pragma once

#include <functional>

namespace DI
{
	namespace Details
	{
		namespace FunctionTraits
		{
			
			struct AnyType
			{
				template<class U>
				operator U() {};
			};
			
			struct AnyTypeRef
			{
				template<class U>
				operator U&() const {};
			};

			template<class T, size_t>
			using WrapType = T;

			template<class T, class... As>
			decltype(std::declval<T>()(std::declval<As>()...), std::true_type()) IsCallable(int) {};
			template<class T, class...>
			std::false_type IsCallable(...) {};
			template<class T, class... As>
			using IsCallableCheck = decltype(IsCallable<T, As...>(0));

			template<template<class...> class, class, class, class = void>
			struct FunctionArityImpl;

			template<template<class...> class C, class T, size_t... I>
			struct FunctionArityImpl<C, T, std::index_sequence<I...>,
				std::enable_if_t<
					(sizeof...(I) > 0) 
					&& C<T, WrapType<AnyType, I>...>::value
				>
			>
			{
				static constexpr size_t value = sizeof...(I);
			};

			template<template<class...> class C, class T, size_t... I>
			struct FunctionArityImpl<C, T, std::index_sequence<I...>,
				std::enable_if_t<
					(sizeof...(I) > 0)
					&& !C<T, WrapType<AnyType, I>...>::value
					&& C<T, WrapType<AnyTypeRef, I>...>::value
				>
			>
			{
				static constexpr size_t value = sizeof...(I);
			};

			template<template<class...> class C, class T, size_t... I>
			struct FunctionArityImpl<C, T, std::index_sequence<I...>,
				std::enable_if_t<
					(sizeof...(I) > 0) 
					&& !C<T, WrapType<AnyType, I>...>::value
					&& !C<T, WrapType<AnyTypeRef, I>...>::value
				>
			>
				: FunctionArityImpl<C, T, std::make_index_sequence<sizeof...(I) - 1>>
			{};

			template<template<class...> class C, class T>
			struct FunctionArityImpl<C, T, std::index_sequence<>>
			{
				static constexpr size_t value = 0;
			};

			template<template<class...> class, class, class, class = void>
			struct FunctionResultTypeImpl;

			template<template<class...> class C, class T, size_t... I>
			struct FunctionResultTypeImpl<C, T, std::index_sequence<I...>, std::enable_if_t<C<T, WrapType<AnyType, I>...>::value>>
			{
				using Type = std::result_of_t<T(WrapType<AnyType, I>...)>;
			};

			template<template<class...> class C, class T, size_t... I>
			struct FunctionResultTypeImpl<C, T, std::index_sequence<I...>, std::enable_if_t<!C<T, WrapType<AnyType, I>...>::value>>
			{
				using Type = std::result_of_t<T(WrapType<AnyTypeRef, I>...)>;
			};

		}

		template<class T, size_t Max = 10>
		struct FunctionArity
		{
			static constexpr size_t value = FunctionTraits::FunctionArityImpl<FunctionTraits::IsCallableCheck, T, std::make_index_sequence<Max>>::value;
		};

		template<class T>
		struct FunctionResultType
		{
			using Type = typename FunctionTraits::FunctionResultTypeImpl<FunctionTraits::IsCallableCheck, T, std::make_index_sequence<FunctionArity<T>::value>>::Type;
		};

	}
}