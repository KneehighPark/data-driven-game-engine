#include "pch.h"
#include "HashFunctions.h"

namespace Library
{
	inline size_t AdditiveHash(const uint8_t* data, size_t size)
	{
		size_t hash = 0;
		const size_t prime = 127;
		for (size_t i = 0; i < size; ++i)
		{
			hash += prime * data[i];
		}
		return hash;
	}

	template <typename TKey>
	inline size_t HashFunctions<TKey>::operator()(const TKey& key) const
	{
		return AdditiveHash(reinterpret_cast<const uint8_t*>(&key), sizeof(key));
	}

	inline size_t HashFunctions<char*>::operator()(const char* key) const
	{
		return AdditiveHash(reinterpret_cast<const uint8_t*>(key), strlen(key));
	}

	inline size_t HashFunctions<std::string>::operator()(const std::string& key) const
	{
		return AdditiveHash(reinterpret_cast<const uint8_t*>(key.c_str()), key.size());
	}
}