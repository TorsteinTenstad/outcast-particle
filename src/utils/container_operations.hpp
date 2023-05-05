#pragma once
#include <array>
#include <cassert>
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
template <class T>
unsigned FindClosest(const std::vector<T>& sorted_arr, const T& value)
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

template <typename T, typename... Args>
std::vector<std::tuple<T&, Args&...>> zip(std::vector<T>& a, std::vector<Args>&... args)
{
	std::vector<std::tuple<T&, Args&...>> zipped;
	assert(((a.size() == args.size()) && ...));
	for (auto i = 0; i < a.size(); i++)
	{
		zipped.push_back({ a[i], args[i]... });
	}
	return zipped;
}

template <typename T, typename... Args>
std::vector<std::tuple<const T&, const Args&...>> zip(const std::vector<T>& a, const std::vector<Args>&... args)
{
	std::vector<std::tuple<const T&, const Args&...>> zipped;
	assert(((a.size() == args.size()) && ...));
	for (auto i = 0; i < a.size(); i++)
	{
		zipped.push_back({ a[i], args[i]... });
	}
	return zipped;
}

template <typename T>
std::vector<std::tuple<int, T&>> enumerate(std::vector<T>& vec)
{
	std::vector<std::tuple<int, T&>> enumerated;
	int i = 0;
	for (T& elem : vec)
	{
		enumerated.push_back({ i, elem });
	}
	i++;
	return enumerated;
}

template <typename T>
std::vector<std::tuple<int, const T&>> enumerate(const std::vector<T>& vec)
{
	std::vector<std::tuple<int, const T&>> enumerated;
	int i = 0;
	for (const T& elem : vec)
	{
		enumerated.push_back({ i, elem });
		i++;
	}
	return enumerated;
}