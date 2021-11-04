#pragma once
#include "pch.h"
#include "Factory.h"

namespace Library
{
	template <typename AbstractProductType>
	HashMap<std::string, Factory<AbstractProductType>*> Factory<AbstractProductType>::mFactoriesTable(11);

	template<typename AbstractProductType>
	inline Factory<AbstractProductType>* Factory<AbstractProductType>::Find(const std::string& name)
	{
		auto iter = mFactoriesTable.Find(name);
		if (iter == mFactoriesTable.end())
		{
			return nullptr;
		}
		return (*iter).second;
	}

	template<typename AbstractProductType>
	inline gsl::owner<AbstractProductType*> Factory<AbstractProductType>::Create(const std::string& name)
	{
		return mFactoriesTable.At(name)->Create();
	}

	template<typename AbstractProductType>
	inline void Factory<AbstractProductType>::Add(Factory& factory)
	{
		mFactoriesTable.Insert(std::make_pair(factory.ClassName(), &factory));
	}

	template<typename AbstractProductType>
	inline void Factory<AbstractProductType>::Remove(Factory& factory)
	{
		mFactoriesTable.Remove(factory.ClassName());
	}

	template<typename AbstractProductType>
	inline size_t Factory<AbstractProductType>::GetFactoryTableSize()
	{
		return mFactoriesTable.Size();
	}
}