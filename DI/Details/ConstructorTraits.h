#pragma once

#include <type_traits>

namespace DI
{
	namespace Details
	{
		namespace ConstructorTraits
		{

			template<class T, class U>
			using IsSame = std::is_same<T, std::remove_cv_t<std::remove_reference_t<U>>>;

			template<class T>
			struct AnyType
			{
				template<class U, class = std::enable_if_t<!IsSame<T, U>::value>>
				operator U() {};
			};

			template<class T>
			struct AnyTypeRef
			{
				template<class U, class = std::enable_if_t<!IsSame<T, U>::value>>
				operator U&() const {};
			};

			template<class T, size_t>
			using WrapType = T;

			template<class T, class... As>
			decltype(void(T(std::declval<As>()...)), std::true_type()) IsConstructible(int) {};
			template<class T, class...>
			std::false_type IsConstructible(...) {};
			template<class T, class... As>
			using IsConstructibleCheck = decltype(IsConstructible<T, As...>(0));

			template<template<class...> class, class, class, class = void>
			struct ConstructorArityImpl;

			template<template<class...> class C, class T, size_t... I>
			struct ConstructorArityImpl<C, T, std::index_sequence<I...>,
				std::enable_if_t<
					(sizeof...(I) > 0) 
					&& C<T, WrapType<AnyType<T>, I>...>::value
				>
			>
			{
				static constexpr size_t value = sizeof...(I);
			};

			template<template<class...> class C, class T, size_t... I>
			struct ConstructorArityImpl<C, T, std::index_sequence<I...>,
				std::enable_if_t<
					(sizeof...(I) > 0) 
					&& !C<T, WrapType<AnyType<T>, I>...>::value 
					&& C<T, WrapType<AnyTypeRef<T>, I>...>::value
				>
			>
			{
				static constexpr size_t value = sizeof...(I);
			};

			template<template<class...> class C, class T, size_t... I>
			struct ConstructorArityImpl<C, T, std::index_sequence<I...>,
				std::enable_if_t<
					(sizeof...(I) > 0) 
					&& !C<T, WrapType<AnyType<T>, I>...>::value 
					&& !C<T, WrapType<AnyTypeRef<T>, I>...>::value
				>
			>
				: ConstructorArityImpl<C, T, std::make_index_sequence<sizeof...(I)-1>>
			{};

			template<template<class...> class C, class T>
			struct ConstructorArityImpl<C, T, std::index_sequence<>>
			{
				static constexpr size_t value = 0;
			};

		}

		template<class T, size_t Max = 10>
		struct ConstructorArity
		{
			static constexpr size_t value = ConstructorTraits::ConstructorArityImpl<ConstructorTraits::IsConstructibleCheck, T, std::make_index_sequence<Max>>::value;
		};

	}
}