#include "ServiceResolvers.h"

#include <unordered_map>
#include <algorithm>

namespace DI
{
	namespace Details
	{
		
		class ServiceResolvers::Impl
		{
		public:
			void add(std::shared_ptr<ServiceResolver<>> resolver)
			{
				_service_resolvers[resolver->getServiceType()] = resolver;
			}

			void merge(const Impl& other)
			{
				std::for_each(
					begin(other._service_resolvers), end(other._service_resolvers),
					[this](auto& resolver_with_type_index) { _service_resolvers[resolver_with_type_index.first] = resolver_with_type_index.second; }
				);
			}

			auto empty() const -> bool
			{
				return _service_resolvers.empty();
			}

			auto get(TypeIndex<> type_index) const -> std::shared_ptr<ServiceResolver<>>
			{
				auto resolver_it = _service_resolvers.find(type_index);
				if (resolver_it == end(_service_resolvers))
					return nullptr;

				return resolver_it->second;
			}

		private:
			std::unordered_map<Details::TypeIndex<>, std::shared_ptr<Details::ServiceResolver<>>> _service_resolvers;
		};

		ServiceResolvers::ServiceResolvers()
			: _impl(std::make_unique<Impl>())
		{}

		ServiceResolvers::ServiceResolvers(ServiceResolvers&& other)
			: _impl(std::move(other._impl))
		{}

		ServiceResolvers::ServiceResolvers(std::initializer_list<std::shared_ptr<ServiceResolver<>>> resolvers)
			: ServiceResolvers()
		{
			std::for_each(
				begin(resolvers), end(resolvers),
				[this](auto& resolver) { _impl->add(resolver); }
			);
		}

		ServiceResolvers::~ServiceResolvers()
		{}

		void ServiceResolvers::add(std::shared_ptr<ServiceResolver<>> resolver)
		{
			_impl->add(resolver);
		}

		void ServiceResolvers::merge(const ServiceResolvers& other)
		{
			_impl->merge(*other._impl);
		}

		auto ServiceResolvers::empty() const -> bool
		{
			return _impl->empty();
		}

		auto ServiceResolvers::get(TypeIndex<> type_index) const -> std::shared_ptr<ServiceResolver<>>
		{
			return _impl->get(type_index);
		}

	}
}