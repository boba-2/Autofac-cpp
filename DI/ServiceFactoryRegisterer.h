#pragma once

#include "Details/ServiceAliasRegisterer.h"
#include "ServiceRegisterer.h"

namespace DI
{

	class ServiceFactoryRegistererImpl
	{
	public:
		virtual ~ServiceFactoryRegistererImpl() {}

		virtual void setSingleInstance() = 0;
		virtual void setAutoManaged() = 0;
		virtual void registerAlias(std::shared_ptr<Details::ServiceAliasRegisterer<>> alias_registerer) = 0;
	};

	template<class T>
	class ServiceFactoryRegisterer : public ServiceRegisterer<ServiceFactoryRegistererImpl>
	{
	public:
		using ServiceRegisterer::ServiceRegisterer;

		ServiceFactoryRegisterer& singleInstance()
		{
			_impl->setSingleInstance();

			return *this;
		}

		ServiceFactoryRegisterer& autoManaged()
		{
			_impl->setAutoManaged();

			return *this;
		}

		template<class V>
		ServiceFactoryRegisterer& as()
		{
			static_assert(std::is_base_of<V, T>::value, "Alias should be a resolvable base class of the service class being registered");

			_impl->registerAlias(std::make_shared<Details::ServiceAliasRegisterer<V, T>>());

			return *this;
		}

		ServiceFactoryRegisterer& asSelf()
		{
			return as<T>();
		}
	};

}