#pragma once

#include "ServiceRegisterer.h"
#include "ServiceTypeResolver.h"
#include "SingleInstanceServiceTypeResolver.h"
#include "../ServiceTypeRegisterer.h"

namespace DI
{
	namespace Details
	{

		template<class T>
		class ServiceTypeRegisterer : public ServiceRegisterer<T>, public DI::ServiceTypeRegisterer<T>
		{
		public:
			virtual std::shared_ptr<ServiceResolver<>> getServiceResolver() const override
			{
				static_assert(!std::is_abstract<T>::value, "Cannot register an abstract type");

				if (_single_instance)
					return std::make_shared<SingleInstanceServiceTypeResolver<T>>();

				return std::make_shared<ServiceTypeResolver<T>>();
			}

		protected:
			virtual void setSingleInstance() override
			{
				_single_instance = true;
			}

			virtual void registerAlias(std::shared_ptr<ServiceAliasRegisterer<>> alias_registerer) override
			{
				ServiceRegisterer<T>::registerAlias(alias_registerer);
			}

		private:
			bool _single_instance = false;
		};

	}
}