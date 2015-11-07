#pragma once

#include "ServiceRegisterer.h"
#include "ServiceTypeResolver.h"
#include "SingletonServiceResolver.h"
#include "AutoManagedServiceResolver.h"

namespace DI
{
	namespace Details
	{

		template<class T>
		class ServiceTypeRegisterer : public ServiceRegisterer<T>
		{
		public:
			virtual ServiceResolverPtr<> getServiceResolver() const override
			{
				auto resolver = std::static_pointer_cast<ServiceResolver<T>>(std::make_shared<ServiceTypeResolver<T>>());
				if (_single_instance)
					resolver = std::make_shared<SingletonServiceResolver<T>>(resolver);
				if (_auto_managed)
					resolver = std::make_shared<AutoManagedServiceResolver<T>>(resolver);

				return resolver;
			}

			virtual void setSingleInstance()
			{
				_single_instance = true;
			}

			virtual void setAutoManaged()
			{
				_auto_managed = true;
			}

		private:
			bool _single_instance = false;
			bool _auto_managed = false;
		};

	}
}