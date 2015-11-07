#pragma once

#include "Details/UnderlyingType.h"
#include "ServiceTypeRegistererImpl.h"

namespace DI
{

	template<class T>
	class ServiceTypeRegisterer : public ServiceRegisterer
	{
	public:
		static_assert(!std::is_abstract<T>::value, "Cannot register service of an abstract type");
		static_assert(std::is_same<T, typename Details::UnderlyingType<T>::Type>::value, "Cannot register service of a decorated type");

		using ServiceType = T;

		ServiceTypeRegisterer(std::shared_ptr<ServiceTypeRegistererImpl> impl, ServiceRegisterer& service_registerer)
			: ServiceRegisterer(service_registerer),
			_impl(impl)
		{}

		ServiceTypeRegisterer& singleInstance()
		{
			_impl->setSingleInstance();

			return *this;
		}

		ServiceTypeRegisterer& autoManaged()
		{
			_impl->setAutoManaged();

			return *this;
		}

		template<class U>
		ServiceTypeRegisterer& as()
		{
			static_assert(std::is_base_of<U, ServiceType>::value, "Alias should be a resolvable base class of the service class being registered");

			_impl->registerAlias(std::make_shared<Details::ServiceAliasRegisterer<U, ServiceType>>());

			return *this;
		}

		ServiceTypeRegisterer& asSelf()
		{
			return as<ServiceType>();
		}

	private:
		std::shared_ptr<ServiceTypeRegistererImpl> const _impl;
	};

}