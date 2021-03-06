#pragma once

#include <vector>
#include "ServiceResolver.h"

namespace DI
{
	namespace Details
	{

		template<class T>
		class AutoManagedServiceResolver : public ServiceResolver<T>
		{
			using ServiceType = typename ServiceResolver<T>::ServiceType;
			using ServiceRefType = typename ServiceResolver<T>::ServiceRefType;
			using ServicePtrType = typename ServiceResolver<T>::ServicePtrType;
			using ServiceSharedPtrType = typename ServiceResolver<T>::ServiceSharedPtrType;
			using ServiceUniquePtrType = typename ServiceResolver<T>::ServiceUniquePtrType;

		public:
			explicit AutoManagedServiceResolver(ServiceResolverPtr<T> inner_resolver)
				: _inner_resolver(inner_resolver)
			{}

		private:
			virtual ServiceType getService(Container* container) override
			{
				return _inner_resolver->getService(container);
			}

			virtual ServiceRefType getServiceAsRef(Container* container) override
			{
				return *getNewManagedInstance(container).get();
			}

			virtual ServicePtrType getServiceAsPtr(Container* container) override
			{
				return getNewManagedInstance(container).get();
			}

			virtual ServiceSharedPtrType getServiceAsSharedPtr(Container* container) override
			{
				return _inner_resolver->getServiceAsSharedPtr(container);
			}

			virtual ServiceUniquePtrType getServiceAsUniquePtr(Container* container) override
			{
				return _inner_resolver->getServiceAsUniquePtr(container);
			}

			ServiceSharedPtrType getNewManagedInstance(Container* container)
			{
				_managed_instances.push_back(getServiceAsSharedPtr(container));

				return _managed_instances.back();
			}

			ServiceResolverPtr<T> const _inner_resolver;
			std::vector<std::shared_ptr<T>> _managed_instances;
		};

	}
}