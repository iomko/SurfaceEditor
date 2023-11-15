#pragma once
#include <unordered_map>

template <class T>
class TestClass
{
public:
	TestClass() = default;
public:
	std::unordered_map<T, unsigned int> map;
};