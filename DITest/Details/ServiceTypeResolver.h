#pragma once
#include "ServiceResolver.h"

namespace DI
{
	namespace Details
	{
		
		template<class T>
		class ServiceTypeResolver : public ServiceResolver<T>
		{
			virtual std::shared_ptr<T> getService() const override
			{
				return std::make_shared<T>();
			}
		};

	}
}