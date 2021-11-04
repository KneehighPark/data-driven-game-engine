#pragma once
#include <cstring>

namespace Library
{
	template <typename TKey>

	class HashFunctions
	{
	public:
		size_t operator()(const TKey& key) const;
	};

	template <>
	class HashFunctions<char*>
	{
	public:
		size_t operator()(const char* key) const;
	};

	template <>
	class HashFunctions<std::string>
	{
	public:
		size_t operator()(const std::string& key) const;
	};
}
#include "HashFunctions.inl"