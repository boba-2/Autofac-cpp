#pragma once

#include "ConstructorTraits.h"
#include "../Container.h"

namespace DI
{
	class Container;

	namespace Details
	{

		template<class T>
		struct ServiceCreator
		{
			template<class S = T>
			static S createService(DI::Container* container)
			{
				auto resolver = ArgumentResolver(container);
				return Factory<S, std::make_index_sequence<ConstructorArity<T>::value>>::createWith(resolver);
			}

			struct ArgumentResolver
			{
				explicit ArgumentResolver(DI::Container* container)
					: _container(container)
				{}

				template<class U, class S>
				using IsSame = std::is_same<U, std::remove_cv_t<std::remove_reference_t<S>>>;

				template<class U, class = std::enable_if_t<!IsSame<T, U>::value>>
				operator U()
				{
					return _container->resolve<U>();
				}

				template<class U, class = std::enable_if_t<!IsSame<T, U>::value>>
				operator U&() const
				{
					return _container->resolve<U&>();
				}

				DI::Container* _container;
			};

			template<class, class>
			struct Factory;

#pragma warning(disable:4100)
			template<class U, size_t... I>
			struct Factory<U, std::index_sequence<I...>>
			{
				static U createWith(ArgumentResolver& resolver)
				{
					return U(wrapValue<I>(resolver)...);
				}
			};

			template<class U, size_t... I>
			struct Factory<std::shared_ptr<U>, std::index_sequence<I...>>
			{
				static std::shared_ptr<U> createWith(ArgumentResolver& resolver)
				{
					return std::make_shared<U>(wrapValue<I>(resolver)...);
				}
			};

			template<class U, size_t... I>
			struct Factory<std::unique_ptr<U>, std::index_sequence<I...>>
			{
				static std::unique_ptr<U> createWith(ArgumentResolver& resolver)
				{
					return std::make_unique<U>(wrapValue<I>(resolver)...);
				}
			};
#pragma warning(default:4100)

			template<size_t>
			static ArgumentResolver& wrapValue(ArgumentResolver& object)
			{
				return object;
			}
		};

	}
}