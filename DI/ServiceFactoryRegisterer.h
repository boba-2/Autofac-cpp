#pragma once

#include "ServiceRegisterer.h"

namespace DI
{

	class ServiceFactoryRegistererImpl
	{
	public:
		virtual ~ServiceFactoryRegistererImpl() {}

		virtual void setAutoManaged() = 0;
		virtual void registerAlias(std::shared_ptr<Details::ServiceAliasRegisterer<>> alias_registerer) = 0;
	};

	struct NoAutoManage {};

	template<class T, class U = void>
	class ServiceFactoryRegisterer : public ServiceRegisterer<ServiceFactoryRegistererImpl>
	{
	public:
		using ServiceRegisterer::ServiceRegisterer;

		template<class V = U>
		ServiceFactoryRegisterer& autoManaged(std::enable_if_t<!std::is_same<V, NoAutoManage>::value>* = 0)
		{
			_impl->setAutoManaged();

			return *this;
		}
	};

}