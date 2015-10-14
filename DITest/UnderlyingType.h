#pragma once

#include <memory>

template<class T>
struct UnderlyingType
{
	using Type = T;
};

template<class T>
struct UnderlyingType<T *>
{
	using Type = typename UnderlyingType<T>::Type;
};

template<class T>
struct UnderlyingType<T &>
{
	using Type = typename UnderlyingType<T>::Type;
};

template<class T>
struct UnderlyingType<T &&>
{
	using Type = typename UnderlyingType<T>::Type;
};

template<class T>
struct UnderlyingType<std::shared_ptr<T>>
{
	using Type = typename UnderlyingType<T>::Type;
};

template<class T>
struct UnderlyingType<std::unique_ptr<T>>
{
	using Type = typename UnderlyingType<T>::Type;
};
