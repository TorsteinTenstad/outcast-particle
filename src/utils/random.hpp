#pragma once
#include <random>

unsigned long long int Rand()
{
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<unsigned long long int> dis;
	return dis(gen);
}