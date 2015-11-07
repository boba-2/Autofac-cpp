#pragma once

#include "Details/ServiceAliasRegisterer.h"
#include "ServiceInstanceRegistererImpl.h"
#include "Details/UnderlyingType.h"

namespace DI
{

	template<class T>
	class ServiceInstanceRegisterer : public ServiceRegisterer
	{
	public:
		using ServiceType = typename Details::UnderlyingType<T>::Type;

		auto asSelf() -> ServiceInstanceRegisterer&;

		template<class U>
		auto as() -> ServiceInstanceRegisterer&;

	private:
		ServiceInstanceRegisterer(std::shared_ptr<ServiceInstanceRegistererImpl> impl, ServiceRegisterer& service_registerer);

		friend class ServiceRegisterer;

		std::shared_ptr<ServiceInstanceRegistererImpl> const _impl;
	};

	template<class T>
	template<class U>
	auto ServiceInstanceRegisterer<T>::as() -> ServiceInstanceRegisterer&
	{
		static_assert(std::is_base_of<U, ServiceType>::value, "Alias should be a resolvable base class of the service being registered");

		_impl->registerAlias(std::make_shared<Details::ServiceAliasRegisterer<U, ServiceType>>());

		return *this;
	}

	template<class T>
	auto ServiceInstanceRegisterer<T>::asSelf() -> ServiceInstanceRegisterer&
	{
		return as<ServiceType>();
	}

	template<class T>
	ServiceInstanceRegisterer<T>::ServiceInstanceRegisterer(std::shared_ptr<ServiceInstanceRegistererImpl> impl, ServiceRegisterer& service_registerer)
		: ServiceRegisterer(service_registerer),
		_impl(impl)
	{}
}