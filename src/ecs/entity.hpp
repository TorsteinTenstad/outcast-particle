#pragma once
#include <cassert>
#include <string>

struct Entity
{
private:
	unsigned long long int id = 0; //Should always be initialized to  != 0. Default initialization to 0 is a debugging aid.

public:
	Entity() {}
	Entity(unsigned long long int id) :
		id(id) {}

	bool operator<(const Entity& other) const
	{
		return id < other.id;
	}

	bool operator>(const Entity& other) const
	{
		return id > other.id;
	}

	bool operator==(const Entity& other) const
	{
		return id == other.id;
	}

	bool operator!=(const Entity& other) const
	{
		return id != other.id;
	}

	std::string ToString()
	{
		assert(id != 0);
		return std::to_string(id);
	}
};
