#pragma once
#include <array>
#include <functional>
#include <map>
#include <vector>

template <class InputType, class OutputType>
std::vector<OutputType> ApplyFuncToVector(std::vector<InputType> vec, std::function<OutputType(InputType)> func)
{
	std::vector<OutputType> result = {};
	for (auto& x : vec)
	{
		result.push_back(func(x));
	}
	return result;
}

template <class K, class V>
typename std::map<K, V>::const_iterator NextInMap(const std::map<K, V>& m, K key)
{
	assert(m.count(key) == 1);
	auto it = m.find(key);
	++it;
	if (it == m.end())
	{
		return --it;
	}
	return it;
}
template <class K, class V>
typename std::map<K, V>::const_iterator PrevInMap(const std::map<K, V>& m, K key)
{
	assert(m.count(key) == 1);
	auto it = m.find(key);
	if (it == m.begin())
	{
		return it;
	}
	--it;
	return it;
}

// NOTE: The array is assumed sorted
template <class T, std::size_t SIZE>
unsigned FindClosest(const std::array<T, SIZE>& sorted_arr, const T& value)
{
	for (unsigned i = 0; i < sorted_arr.size(); ++i)
	{
		if (value < sorted_arr[i])
		{
			if (i == 0)
			{
				return i;
			}
			if (value - sorted_arr[i - 1] < sorted_arr[i] - value)
			{
				return i - 1;
			}
			else
			{
				return i;
			}
		}
	}
	return sorted_arr.size() - 1;
}
