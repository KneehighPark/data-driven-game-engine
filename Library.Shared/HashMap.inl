#pragma once
#include "HashMap.h"

namespace Library
{
	template <typename TKey, typename TData, typename HashFunctor>
	const HashFunctor HashMap<TKey, TData, HashFunctor>::mHashFunction;
#pragma region HashMap

	template <typename TKey, typename TData, typename HashFunctor>
	HashMap<TKey, TData, HashFunctor>::HashMap(std::initializer_list<PairType> list) : HashMap(static_cast<size_t>(list.size()))
	{
		for (const auto& value : list)
		{
			Insert(value);
		}
	}


	template <typename TKey, typename TData, typename HashFunctor>
	inline HashMap<TKey, TData, HashFunctor>::HashMap(size_t numberOfBuckets) : mSize(0), mBuckets(0)
	{
		mBuckets.Resize(numberOfBuckets);
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline TData& HashMap<TKey, TData, HashFunctor>::operator[](const TKey& key)
	{
		//cplusplus.com/reference/map/map/operator[]
		return ((*(Insert(std::make_pair(key, TData())))).second);
	}


	template <typename TKey, typename TData, typename HashFunctor>
	inline typename HashMap<TKey, TData, HashFunctor>::Iterator HashMap<TKey, TData, HashFunctor>::Find(const TKey& key)
	{
		size_t index;
		return Find(key, index);
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline typename HashMap<TKey, TData, HashFunctor>::Iterator HashMap<TKey, TData, HashFunctor>::Find(const TKey& key, size_t& index)
	{
		index = mHashFunction(key) % mBuckets.Size();
		Iterator iter = end();
		ChainingIterator chain_iter = mBuckets[index].begin();

		for (; chain_iter != mBuckets[index].end(); ++chain_iter)
		{
			if ((*chain_iter).first == key)
			{
				iter = Iterator(*this, index, chain_iter);
				break;
			}
		}
		return iter;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline typename HashMap<TKey, TData, HashFunctor>::ConstIterator HashMap<TKey, TData, HashFunctor>::Find(const TKey& key) const
	{
		size_t index = mHashFunction(key) % mBuckets.Size();
		ConstIterator iter = end();
		ConstChainingIterator chain_iter = mBuckets[index].begin();

		for (; chain_iter != mBuckets[index].end(); ++chain_iter)
		{
			if ((*chain_iter).first == key)
			{
				iter = ConstIterator(*this, index, chain_iter);
				break;
			}
		}
		return iter;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline TData& HashMap<TKey, TData, HashFunctor>::At(const TKey& key)
	{
		Iterator iter = Find(key);
		if (iter == end())
		{
			throw std::runtime_error("Provided key is not in the hashmap!");
		}
		return iter->second;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline const TData& HashMap<TKey, TData, HashFunctor>::At(const TKey& key) const
	{
		ConstIterator iter = Find(key);
		if (iter == end())
		{
			throw std::runtime_error("Provided key is not in the hashmap!");
		}
		return iter->second;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline size_t HashMap<TKey, TData, HashFunctor>::Size() const
	{
		return mSize;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline typename HashMap<TKey, TData, HashFunctor>::Iterator HashMap<TKey, TData, HashFunctor>::Insert(const PairType& pair)
	{
		size_t index = mHashFunction(pair.first) % mBuckets.Size();
		Iterator iter;
		if (ContainsKey(pair.first, iter))
		{
			return iter;
		}
		iter = Iterator(*this, index, mBuckets[index].PushBack(pair));
		mSize++;
		return iter;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline typename HashMap<TKey, TData, HashFunctor>::Iterator HashMap<TKey, TData, HashFunctor>::Insert(const PairType& pair, bool& result)
	{
		result = false;
		size_t index = mHashFunction(pair.first) % mBuckets.Size();
		Iterator iter;
		if (ContainsKey(pair.first, iter))
		{
			return iter;
		}
		iter = Iterator(*this, index, mBuckets[index].PushBack(pair));
		mSize++;
		result = true;
		return iter;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline bool HashMap<TKey, TData, HashFunctor>::ContainsKey(const TKey& key)
	{
		Iterator iter;
		return ContainsKey(key, iter);
	}


	template <typename TKey, typename TData, typename HashFunctor>
	inline bool HashMap<TKey, TData, HashFunctor>::ContainsKey(const TKey& key, Iterator& iter)
	{
		iter = Find(key);
		if (iter != end())
		{
			return true;
		}
		return false;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline void HashMap<TKey, TData, HashFunctor>::Remove(const TKey& key)
	{
		size_t index;
		Iterator iter = Find(key, index);
		if (iter != end())
		{
			mBuckets[index].Remove(iter.mPair);
			mSize--;
		}
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline void HashMap<TKey, TData, HashFunctor>::Clear()
	{
		size_t vectorSize = mBuckets.Size();
		for (size_t i = 0; i < vectorSize; ++i)
		{
			mBuckets[i].Clear();
		}
		mSize = 0;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	void HashMap<TKey, TData, HashFunctor>::Rehash(size_t numberOfBuckets)
	{
		HashMap new_hashmap(numberOfBuckets);
		for (Iterator iter = begin(); iter != end(); ++iter)
		{
			new_hashmap.Insert(*iter);
		}
		*this = std::move(new_hashmap);
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline size_t HashMap<TKey, TData, HashFunctor>::NumberOfBuckets() const
	{
		return mBuckets.Size();
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline float_t HashMap<TKey, TData, HashFunctor>::LoadFactor() const
	{
		if (mBuckets.IsEmpty())
		{
			return 0;
		}

		size_t usedBuckets = 0;

		for (auto& iter : mBuckets)
		{
			if (!iter.IsEmpty())
			{
				++usedBuckets;
			}
		}
		return (static_cast<float_t>(usedBuckets))/(static_cast<float_t>(mBuckets.Size()));
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline typename HashMap<TKey, TData, HashFunctor>::Iterator HashMap<TKey, TData, HashFunctor>::begin()
	{
		size_t vectorSize = mBuckets.Size();
		for (size_t i = 0; i < vectorSize; ++i)
		{
			if (mBuckets[i].Size() > 0)
			{
				return Iterator(*this, i, mBuckets[i].begin());
			}
		}
		return end();
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline typename HashMap<TKey, TData, HashFunctor>::ConstIterator HashMap<TKey, TData, HashFunctor>::begin() const
	{
		size_t vectorSize = mBuckets.Size();
		for (size_t i = 0; i < vectorSize; ++i)
		{
			if (mBuckets[i].Size() > 0)
			{
				return ConstIterator(*this, i, mBuckets[i].begin());
			}
		}
		return end();
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline typename HashMap<TKey, TData, HashFunctor>::ConstIterator HashMap<TKey, TData, HashFunctor>::cbegin() const
	{
		size_t vectorSize = mBuckets.Size();
		for (size_t i = 0; i < vectorSize; ++i)
		{
			if (mBuckets[i].Size() > 0)
			{
				return ConstIterator(*this, i, mBuckets[i].begin());
			}
		}
		return end();
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline typename HashMap<TKey, TData, HashFunctor>::Iterator HashMap<TKey, TData, HashFunctor>::end()
	{
		return Iterator(*this, mBuckets.Size(), mBuckets[mBuckets.Size() - 1].end());
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline typename HashMap<TKey, TData, HashFunctor>::ConstIterator HashMap<TKey, TData, HashFunctor>::end() const
	{
		return ConstIterator(*this, mBuckets.Size(), mBuckets[mBuckets.Size() - 1].end());
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline typename HashMap<TKey, TData, HashFunctor>::ConstIterator HashMap<TKey, TData, HashFunctor>::cend() const
	{
		return ConstIterator(*this, mBuckets.Size(), mBuckets[mBuckets.Size() - 1].end());
	}

#pragma endregion

#pragma region Iterator

	template <typename TKey, typename TData, typename HashFunctor>
	inline HashMap < TKey, TData, HashFunctor>::Iterator::Iterator(HashMap& owner, size_t currentIndex, typename const HashMap::ChainingIterator& currentPair) :
		mPair(currentPair), mBucketIndex(currentIndex), mOwner(&owner)
	{

	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline typename HashMap<TKey, TData, HashFunctor>::Iterator& HashMap<TKey, TData, HashFunctor>::Iterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator doesn't have valid owner!");
		}
		if (ConstIterator(*this) == mOwner->end())
		{
			throw std::runtime_error("Iterator is already at the end, cannot be incremented!");
		}

		mPair++;

		if (mPair == (mOwner->mBuckets[mBucketIndex]).end())
		{
			mBucketIndex++;
			while ((mBucketIndex < mOwner->mBuckets.Size()) && mOwner->mBuckets[mBucketIndex].IsEmpty())
			{
				mBucketIndex++;
			}

			if (mBucketIndex < mOwner->mBuckets.Size())
			{
				mPair = mOwner->mBuckets[mBucketIndex].begin();
			}
			else
			{
				mPair = mOwner->mBuckets[mBucketIndex - 1].end();
			}
		}
		return *this;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline typename HashMap<TKey, TData, HashFunctor>::Iterator HashMap<TKey, TData, HashFunctor>::Iterator::operator++(int)
	{
		Iterator iter = *this;
		operator++();
		return iter;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline typename HashMap<TKey, TData, HashFunctor>::PairType& HashMap<TKey, TData, HashFunctor>::Iterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator doesn't have valid owner!");
		}
		return *mPair;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline typename HashMap<TKey, TData, HashFunctor>::PairType* HashMap<TKey, TData, HashFunctor>::Iterator::operator->() const
	{
		return &(operator*());
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline bool HashMap<TKey, TData, HashFunctor>::Iterator::operator==(const Iterator& rhs) const
	{
		return (mOwner == rhs.mOwner) && (mPair == rhs.mPair) && (mBucketIndex == rhs.mBucketIndex);
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline bool HashMap<TKey, TData, HashFunctor>::Iterator::operator!=(const Iterator& rhs) const
	{
		return!(*this == rhs);
	}

#pragma endregion

#pragma region ConstIterator

	template <typename TKey, typename TData, typename HashFunctor>
	inline HashMap < TKey, TData, HashFunctor>::ConstIterator::ConstIterator(const HashMap& owner, size_t currentIndex, typename const HashMap::ConstChainingIterator& currentPair) :
		mPair(currentPair), mBucketIndex(currentIndex), mOwner(&owner)
	{

	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline HashMap<TKey, TData, HashFunctor>::ConstIterator::ConstIterator(const Iterator& rhs) :
		mPair(rhs.mPair), mBucketIndex(rhs.mBucketIndex), mOwner(rhs.mOwner)
	{

	}
	
	template <typename TKey, typename TData, typename HashFunctor>
	inline typename HashMap<TKey, TData, HashFunctor>::ConstIterator& HashMap<TKey, TData, HashFunctor>::ConstIterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator doesn't have valid owner!");
		}
		if (*this == mOwner->end())
		{
			throw std::runtime_error("Iterator is already at the end, cannot be incremented!");
		}

		mPair++;

		if (mPair == (mOwner->mBuckets[mBucketIndex]).end())
		{
			mBucketIndex++;
			while ((mBucketIndex < mOwner->mBuckets.Size()) && mOwner->mBuckets[mBucketIndex].isEmpty())
			{
				mBucketIndex++;
			}

			if (mBucketIndex < mOwner->mBuckets.Size())
			{
				mPair = mOwner->mBuckets[mBucketIndex].begin();
			}
			else
			{
				mPair = mOwner->mBuckets[mBucketIndex - 1].end();
			}
		}
		return *this;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline typename HashMap<TKey, TData, HashFunctor>::ConstIterator HashMap<TKey, TData, HashFunctor>::ConstIterator::operator++(int)
	{
		Iterator iter = *this;
		operator++();
		return iter;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline typename const HashMap<TKey, TData, HashFunctor>::PairType& HashMap<TKey, TData, HashFunctor>::ConstIterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator doesn't have valid owner!");
		}
		return *mPair;
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline const typename HashMap<TKey, TData, HashFunctor>::PairType* HashMap<TKey, TData, HashFunctor>::ConstIterator::operator->() const
	{
		return &(operator*());
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline bool HashMap<TKey, TData, HashFunctor>::ConstIterator::operator==(const ConstIterator& rhs) const
	{
		return (mOwner == rhs.mOwner) && (mPair == rhs.mPair) && (mBucketIndex == rhs.mBucketIndex);
	}

	template <typename TKey, typename TData, typename HashFunctor>
	inline bool HashMap<TKey, TData, HashFunctor>::ConstIterator::operator!=(const ConstIterator& rhs) const
	{
		return!(*this == rhs);
	}

#pragma endregion
}