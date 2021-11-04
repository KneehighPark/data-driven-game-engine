#include "pch.h"
#include "Vector.h"
#include <cassert>

namespace Library
{
#pragma region IncrementStrategy

	inline size_t IncrementStrategy::operator()(size_t size, size_t capacity) const
	{
		if (size == 0 && capacity == 0)
		{
			return 5;
		}
		return capacity * 2;
	}

#pragma endregion



#pragma region Vector

	template <typename T>
	inline Vector<T>::Vector(size_t capacity)
	{
		Reserve(capacity);
	}

	template <typename T>
	inline Vector<T>::Vector(std::initializer_list<T> list) : mCapacity(0), mSize(0), mData(nullptr)
	{
		for (const auto& value : list)
		{
			PushBack(value);
		}
	}

	template <typename T>
	inline Vector<T>::Vector(const Vector& rhs) : mSize(rhs.mSize), mCapacity(rhs.mCapacity)
	{
		mData = static_cast<T*>(malloc(rhs.mCapacity * sizeof(T)));

		for (size_t index = 0; index < rhs.mSize; ++index)
		{
			new (mData + index)T(rhs.mData[index]);
		}
	}

	template <typename T>
	inline Vector<T>::~Vector()
	{
		Wipe();
	}

	template <typename T>
	inline Vector<T>::Vector(Vector&& rhs) : mSize(rhs.mSize), mCapacity(rhs.mCapacity), mData(rhs.mData)
	{
		rhs.mSize = 0;
		rhs.mCapacity = 0;
		rhs.mData = nullptr;
	}

	template <typename T>
	inline Vector<T>& Vector<T>::operator=(Vector&& rhs)
	{
		if (this != &rhs)
		{
			Wipe(); 

			mSize = rhs.mSize;
			mCapacity = rhs.mCapacity;
			mData = rhs.mData;

			rhs.mSize = 0;
			rhs.mCapacity = 0;
			rhs.mData = nullptr;
		}
		return *this;
	}

	template <typename T>
	inline Vector<T>& Vector<T>::operator=(const Vector& rhs)
	{
		if (this != &rhs)
		{
			Wipe();

			mData = static_cast<T*>(malloc(rhs.mCapacity * sizeof(T)));

			for (size_t index = 0; index < rhs.mSize; ++index)
			{
				new (mData + index)T(rhs.mData[index]);
			}
			mSize = rhs.mSize;
			mCapacity = rhs.mCapacity;
		}
		return *this;
	}

	template <typename T>
	inline T& Vector<T>::operator[](size_t index)
	{
		if (index >= mSize)
		{
			throw std::runtime_error("Index is out of bounds");
		}
		return mData[index];
	}

	template <typename T>
	inline const T& Vector<T>::operator[](size_t index) const
	{
		if (index >= mSize)
		{
			throw std::runtime_error("Index is out of bounds");
		}
		return mData[index];
	}

	template <typename T>
	inline T& Vector<T>::At(size_t index)
	{
		return operator[](index);
	}

	template <typename T>
	inline const T& Vector<T>::At(size_t index) const
	{
		return operator[](index);
	}

	template <typename T>
	inline bool Vector<T>::IsEmpty() const
	{
		return (mSize == 0);
	}

	template <typename T>
	inline T& Vector<T>::Front()
	{
		if (IsEmpty())
		{
			throw std::runtime_error("Vector is empty.");
		}
		return mData[0];
	}

	template <typename T>
	inline const T& Vector<T>::Front() const
	{
		if (IsEmpty())
		{
			throw std::runtime_error("Vector is empty.");
		}
		return mData[0];
	}

	template <typename T>
	inline T& Vector<T>::Back()
	{
		if (IsEmpty())
		{
			throw std::runtime_error("Vector is empty.");
		}
		return mData[mSize-1];
	}

	template <typename T>
	inline const T& Vector<T>::Back() const
	{
		if (IsEmpty())
		{
			throw std::runtime_error("Vector is empty.");
		}
		return mData[mSize - 1];
	}

	template <typename T>
	inline size_t Vector<T>::Size() const
	{
		return mSize;
	}

	template <typename T>
	inline size_t Vector<T>::Capacity() const
	{
		return mCapacity;
	}

	template <typename T>
	inline void Vector<T>::PopBack()
	{
		if (!IsEmpty())
		{
			mData[--mSize].~T();
		}
	}

	template <typename T>
	inline void Vector<T>::Clear()
	{
		while (mSize > 0)
		{
			PopBack();
		}
	}

	template <typename T>
	inline void Vector<T>::Wipe()
	{
		Clear();
		if (mCapacity != 0)
		{
			free(mData);
		}
		mData = nullptr;
		mCapacity = 0;
	}

	template <typename T>
	inline void Vector<T>::ShrinkToFit()
	{
		if (mCapacity > mSize)
		{
			T* prevData = mData;
			mData = reinterpret_cast<T*>(malloc(mSize * sizeof(T)));
			memcpy(mData, prevData, mSize * sizeof(T));
			mCapacity = mSize;

			free(prevData);
			prevData = nullptr;
		}
	}

	template <typename T>
	inline typename Vector<T>::Iterator Vector<T>::Find(const T& data)
	{
		Iterator iter = begin();

		for (; iter != end(); iter++)
		{
			if (*iter == data)
			{
				break;
			}
		}
		return iter;
	}

	template <typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::Find(const T& data) const
	{
		ConstIterator iter = begin();

		for (; iter != end(); ++iter)
		{
			if (*iter == data)
			{
				break;
			}
		}
		return iter;
	}

	template <typename T>
	inline typename Vector<T>::Iterator Vector<T>::PushBack(const T& data)
	{
		if (mSize == mCapacity)
		{
			IncrementStrategy strat; 
			size_t newCapacity = strat.operator()(mSize, mCapacity);

			if (newCapacity < mCapacity)
			{
				throw std::runtime_error("Increment strategy fail! New capacity is less than current capacity!");
			}
			Reserve(newCapacity);
		}
		new (mData + mSize)T(data);
		mSize++;
		return Iterator(size_t(mSize - 1), *this);
	}

	template <typename T>
	inline typename Vector<T>::Iterator Vector<T>::begin() 
	{
		return Iterator(size_t(0), *this);
	}

	template <typename T>
	inline typename Vector<T>::Iterator Vector<T>::end()
	{
		return Iterator(mSize, *this);
	}

	template <typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::begin() const
	{
		return ConstIterator(0, *this);
	}

	template <typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::end() const
	{
		return ConstIterator(mSize, *this);
	}

	template <typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cbegin() const
	{
		return ConstIterator(0, *this);
	}

	template <typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cend() const
	{
		return ConstIterator(mSize, *this);
	}

	template <typename T>
	inline void Vector<T>::Resize(size_t newSize)
	{
		if (newSize < mSize)
		{
			for (size_t i = newSize; i < mSize; ++i)
			{
				mData[i].~T();
			}
		}
		if (newSize > mCapacity)
		{
			mData = reinterpret_cast<T*>(realloc(mData, sizeof(T) * newSize));

			for (size_t i = mSize; i < newSize; i++)
			{
				new (mData + i)T();
			}
		}
		mSize = newSize;
		mCapacity = newSize;
	}

	template <typename T>
	inline void Vector<T>::Reserve(size_t newCapacity)
	{	
		if (newCapacity > mCapacity)
		{
			mData = reinterpret_cast<T*>(realloc(mData, newCapacity * sizeof(T)));
			mCapacity = newCapacity;
		}
	}

	template <typename T>
	bool Vector<T>::Remove(const T& data)
	{
		Iterator iter = Find(data);

		if (iter != end())
		{
			Iterator iter2 = iter;
			return Remove(iter, ++iter2);
		}
		return false;
	}

	template <typename T>
	bool Vector<T>::Remove(const Iterator& iter)
	{
		Iterator next = iter;

		if (iter.mOwner == this)
		{
			++next;
		}

		size_t lastSize = mSize;
		Remove(iter, next);
		return (lastSize > mSize);
	}

	template <typename T>
	bool Vector<T>::Remove(const Iterator& beginning, const Iterator& ending)
	{
		if ((beginning.mOwner != ending.mOwner) || (beginning.mOwner != this))
		{
			throw std::runtime_error("Iterators are invalid");
		}

		if ((beginning.mIndex > ending.mIndex))
		{
			throw std::runtime_error("Iterators are invalid");
		}

		size_t startingIndex = beginning.mIndex;
		size_t endingIndex = ending.mIndex;

		for (Iterator iter = beginning; iter != ending; ++iter)
		{
			(*iter).~T();
		}

		T* dest = mData + startingIndex;
		T* src = mData + endingIndex;

		memmove_s(dest, sizeof(T) * (mSize - endingIndex), src, sizeof(T) * (mSize - endingIndex));
		mSize -= (endingIndex - startingIndex);

		return true;
	}

	template <typename T>
	template <typename... Args>
	typename Vector<T>::Iterator Vector<T>::EmplaceBack(Args&&... args)
	{
		if (mSize == mCapacity)
		{
			IncrementStrategy strat;
			size_t newCapacity = strat.operator()(mSize, mCapacity);

			if (newCapacity < mCapacity)
			{
				throw std::runtime_error("Increment strategy fail! New capacity is less than current capacity!");
			}
			Reserve(newCapacity);
		}
		new (mData + mSize)T(std::forward<Args>(args)...);
		++mSize;
		return Iterator(mSize - 1, *this);
	}


#pragma endregion
#pragma region Iterator

	template <typename T>
	inline Vector<T>::Iterator::Iterator(size_t index, Vector& owner) : mIndex(index), mOwner(&owner)
	{

	}

	template <typename T>
	inline bool Vector<T>::Iterator::operator==(const Iterator& rhs) const
	{
		return (mIndex == rhs.mIndex) && (mOwner == rhs.mOwner);
	}
	
	template <typename T>
	inline bool Vector<T>::Iterator::operator!=(const Iterator& rhs) const
	{
		return !(*this == rhs);
	}
	
	template <typename T>
	inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator does not have a valid owner");
		}

		if (mIndex >= mOwner->Size())
		{
			throw std::runtime_error("Index is out of bounds");
		}

		mIndex++;
		return *this;
	}

	template <typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
	{
		Iterator iter = *this;
		operator++();
		return iter;
	}

	template <typename T>
	inline T& Vector<T>::Iterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator does not have a valid owner");
		}

		return mOwner->operator[](mIndex);
	}

#pragma endregion

#pragma region ConstIterator

	template <typename T>
	inline Vector<T>::ConstIterator::ConstIterator(const Iterator& rhs) : mIndex(rhs.mIndex), mOwner(rhs.mOwner)
	{

	}

	template <typename T>
	inline Vector<T>::ConstIterator::ConstIterator(size_t index, const Vector& owner) : mIndex(index), mOwner(&owner)
	{

	}

	template <typename T>
	inline bool Vector<T>::ConstIterator::operator==(const ConstIterator& rhs) const
	{
		return (mIndex == rhs.mIndex) && (mOwner == rhs.mOwner);
	}

	template <typename T>
	inline bool Vector<T>::ConstIterator::operator!=(const ConstIterator& rhs) const
	{
		return !(*this == rhs);
	}

	template <typename T>
	inline typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator does not have a valid owner");
		}

		if (mOwner->Size() == 0)
		{
			throw std::runtime_error("Vector doesn't have any elements");
		}

		if (mIndex >= mOwner->Size())
		{
			throw std::runtime_error("Index is out of bounds");
		}

		mIndex++;
		return *this;
	}

	template <typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator++(int)
	{
		ConstIterator iter = *this;
		operator++();
		return iter;
	}

	template <typename T>
	inline const T& Vector<T>::ConstIterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator does not have a valid owner");
		}

		return mOwner->operator[](mIndex);
	}

#pragma endregion

}