#pragma once

#include <typeindex>
#include "UnderlyingType.h"

namespace DI
{
	namespace Details
	{

		template<class T = void>
		struct TypeIndex;

		template<>
		struct TypeIndex<void> : std::type_index
		{
		protected:
			explicit TypeIndex(const type_info& type_info)
				: type_index(type_info)
			{}
		};

		template<class T>
		struct TypeIndex : TypeIndex<>
		{
			TypeIndex()
				: TypeIndex<>(typeid(UnderlyingType<T>::Type))
			{}
		};

	}
}

namespace std
{
	template<>
	struct hash<DI::Details::TypeIndex<>>
	{
		size_t operator()(const DI::Details::TypeIndex<>& index) const
		{
			return std::hash<type_index>()(static_cast<const type_index&>(index));
		}
	};
}
