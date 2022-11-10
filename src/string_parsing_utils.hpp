#pragma once
#include "PCH.hpp"

std::string GetSubstrBetween(std::string s, std::string prefix, std::string postfix)
{
}

std::vector<std::string> SplitString(std::string s, std::string delimiter)
{
}

template <typename T>
std::string ToString(T x)
{
}
template std::string ToString(int);
template std::string ToString(float);
template std::string ToString(bool);

template <typename T>
T FromString(std::string s)
{
}

template <typename T>
void FromString(T& x, std::string s)
{
}
template void FromString(int&, std::string);
template void FromString(float&, std::string);
template void FromString(bool&, std::string);

void FromString(sf::Vector2f& x, std::string s)
{
}