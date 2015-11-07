﻿#include "ServiceAliasRegisterers.h"

#include <vector>
#include <algorithm>

namespace DI
{
	namespace Details
	{
		
		class ServiceAliasRegisterers::Impl
		{
		public:
			void add(std::shared_ptr<ServiceAliasRegisterer<>> alias_registerer)
			{
				_alias_registerers.push_back(alias_registerer);
			}

			ServiceResolvers getServiceResolvers(std::shared_ptr<ServiceResolver<>> main_resolver)
			{
				auto result = ServiceResolvers();

				std::for_each(
					begin(_alias_registerers), end(_alias_registerers),
					[&](auto& registerer) { result.add(registerer->getServiceAliasResolver(main_resolver)); }
				);

				return result;
			}

		private:
			std::vector<std::shared_ptr<ServiceAliasRegisterer<>>> _alias_registerers;
		};

		ServiceAliasRegisterers::ServiceAliasRegisterers()
			: _impl(std::make_unique<Impl>())
		{}

		ServiceAliasRegisterers::ServiceAliasRegisterers(ServiceAliasRegisterers&& other)
			: _impl(std::move(other._impl))
		{}

		ServiceAliasRegisterers::~ServiceAliasRegisterers()
		{}

		void ServiceAliasRegisterers::add(std::shared_ptr<ServiceAliasRegisterer<>> alias_registerer)
		{
			_impl->add(alias_registerer);
		}

		auto ServiceAliasRegisterers::getServiceResolvers(std::shared_ptr<ServiceResolver<>> main_resolver) const -> ServiceResolvers
		{
			return _impl->getServiceResolvers(main_resolver);
		}

	}
}