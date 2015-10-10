#include "stdafx.h"
#include "CppUnitTest.h"
#include <memory>
#include <typeindex>
#include <unordered_map>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using ServiceInstances = std::unordered_map<std::type_index, void *>;

namespace Error {
	class ServiceNotRegistered : public std::logic_error
	{
	public:
		template<class T>
		auto static fromType()
		{
			return ServiceNotRegistered(typeid(T).name());
		}

	private:
		explicit ServiceNotRegistered(std::string type_name)
			: logic_error(std::string("Trying to resolve service of an unknown type: ") + type_name)
		{}
	};
}

template<class T>
using UnderlyingType = std::remove_pointer_t<T>;

class Container
{
public:
	explicit Container(const ServiceInstances service_instances)
		: _service_instances(service_instances)
	{}

	template<class T>
	T resolve()
	{
		if (_service_instances.find(std::type_index(typeid(UnderlyingType<T>))) == _service_instances.end())
			throw Error::ServiceNotRegistered::fromType<UnderlyingType<T>>();

		return static_cast<T>(_service_instances[std::type_index(typeid(UnderlyingType<T>))]);
	}

private:
	ServiceInstances _service_instances;
};

class ContainerBuilder
{
public:
	template<class T>
	void registerInstance(T *service_instance)
	{
		_service_instances[std::type_index(typeid(T))] = service_instance;
	}

	std::unique_ptr<Container> build() const
	{
		return std::make_unique<Container>(_service_instances);
	}

private:
	ServiceInstances _service_instances;
};

class Class1 {};
class Class2 {};

TEST_CLASS(BaseContainerTest)
{
public:
	TEST_METHOD(ShouldResolveServiceByType_WhenInstanceRegistered)
	{
		Class1 service;

		builder()->registerInstance(&service);

		Assert::IsTrue(container()->resolve<Class1 *>() == &service);
	}

	TEST_METHOD(ShouldResolveServiceOfCorrectType_WhenInstancesOfDifferentTypesRegistered)
	{
		Class1 service1;
		Class2 service2;

		builder()->registerInstance(&service1);
		builder()->registerInstance(&service2);

		Assert::IsTrue(container()->resolve<Class1 *>() == &service1);
		Assert::IsTrue(container()->resolve<Class2 *>() == &service2);
	}

	TEST_METHOD(ShouldThrowException_WhenResolvingServiceOfUnregisteredType)
	{
		Class1 service1;

		builder()->registerInstance(&service1);

		Assert::ExpectException<Error::ServiceNotRegistered>([this]
		{
			container()->resolve<Class2 *>();
		});
	}

private:
	ContainerBuilder *builder() const
	{
		return _container_builder.get();
	}

	Container *container()
	{
		return (_container ? _container : _container = builder()->build()).get();
	}

	std::unique_ptr<ContainerBuilder> _container_builder = std::make_unique<ContainerBuilder>();
	std::unique_ptr<Container> _container;
};
