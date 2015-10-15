#pragma once

template<int = 0>
class DummyService
{
public:
	explicit DummyService(const int value = 0)
		: _value(value)
	{
	}

	DummyService(const DummyService &other)
		: _value(other._value)
	{
	}

	DummyService(DummyService &&other)
	{
		std::swap(_value, other._value);
	}

	DummyService &operator=(const DummyService &other)
	{
		_value = other._value;

		return *this;
	}

	DummyService &operator=(DummyService &&other)
	{
		std::swap(_value, other._value);

		return *this;
	}

	virtual ~DummyService()
	{
	}

	int _value;
};
