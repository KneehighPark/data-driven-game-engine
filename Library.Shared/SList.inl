#include "pch.h"
#include "SList.h"

namespace Library
{
#pragma region Node
	template <typename T>
	inline SList<T>::Node::Node(const T& data, Node* next) : Data(data), Next(next)
	{
	}
#pragma endregion Node

	template<typename T>
	SList<T>::SList(const SList& rhs)
	{
		Node* front = rhs.mFront;

		while (front != nullptr)
		{
			PushBack(front->Data);
			front = front->Next;
		}
	}

	template<typename T>
	SList<T>::~SList()
	{
		Clear();
	}

	template<typename T>
	SList<T>::SList(SList&& rhs) : mSize(rhs.mSize), mFront(rhs.mFront), mBack(rhs.mBack)
	{
		rhs.mSize = 0;
		rhs.mFront = nullptr;
		rhs.mBack = nullptr;
	}

	template<typename T>
	SList<T>& SList<T>::operator=(const SList& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			Node* front = rhs.mFront;
			while (front != nullptr)
			{
				PushBack(front->Data);
				front = front->Next;
			}
		}
		return *this;
	}

	template<typename T>
	inline SList<T>& SList<T>::operator=(SList&& rhs)
	{
		if (this != &rhs)
		{
			Clear();

			mSize = rhs.mSize;
			mFront = rhs.mFront;
			mBack = rhs.mBack;

			rhs.mSize = 0;
			rhs.mFront = nullptr;
			rhs.mBack = nullptr;
		}
	}

	template <typename T>
	inline size_t SList<T>::Size() const
	{
		return mSize;
	}

	template <typename T>
	inline bool SList<T>::IsEmpty() const
	{
		return mSize == 0;
	}

	template<typename T>
	void SList<T>::PushFront(const T& data)
	{
		mFront = new Node(data, mFront);
		if (IsEmpty()) 
		{
			mBack = mFront;
		}
		mSize++;
	}

	template<typename T>
	inline T& SList<T>::Front()
	{
		if (IsEmpty()) 
		{
			throw std::runtime_error("List is empty.");
		}
		return mFront->Data;
	}

	template<typename T>
	inline const T& SList<T>::Front() const
	{
		if (IsEmpty())
		{
			throw std::runtime_error("List is empty.");
		}
		return mFront->Data;
	}

	template<typename T>
	void SList<T>::PopFront()
	{
		if (!IsEmpty())
		{
			Node* node = mFront->Next;
			delete mFront;
			mFront = node;

			mSize--;
			if (mSize <= 1) 
			{
				mBack = mFront;
			}
		}
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::PushBack(const T& data)
	{
		Node* node = new Node(data, nullptr);
		if (IsEmpty())
		{
			mFront = node;
		}
		else
		{
			mBack->Next = node;
		}
		mBack = node;
		mSize++;
		return Iterator(mBack, *this);
	}

	template<typename T>
	inline T& SList<T>::Back()
	{
		if (IsEmpty())
		{
			throw std::runtime_error("List is empty.");
		}
		return mBack->Data;
	}

	template<typename T>
	inline const T& SList<T>::Back() const
	{
		if (IsEmpty())
		{
			throw std::runtime_error("List is empty.");
		}
		return mBack->Data;
	}

	template<typename T>
	void SList<T>::Clear()
	{
		while (mSize > 0)
		{
			PopFront();
		}
	}

	template<typename T>
	void SList<T>::PopBack()
	{
		if (mSize == 1)
		{
			PopFront();
		}
		else if (mSize > 1)
		{
			Node* penultimate = mFront;
			Node* end = mFront->Next;
			while (end->Next != nullptr)
			{
				penultimate = end;
				end = end->Next;
			}
			delete end;
			penultimate->Next = nullptr;
			mSize--;
			mBack = penultimate;
		}
			
		if (mSize <= 1)
		{
			mBack = mFront;
		}
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::begin()
	{
		return(Iterator (mFront, *this));
	}

	template <typename T>
	typename SList<T>::ConstIterator SList<T>::begin() const
	{
		return(ConstIterator (mFront, *this));
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::end()
	{
		return(Iterator(nullptr, *this));
	}

	template <typename T>
	typename SList<T>::ConstIterator SList<T>::end() const
	{
		return(ConstIterator(nullptr, *this));
	}

	template <typename T>
	typename SList<T>::ConstIterator SList<T>::cbegin() const
	{
		return(ConstIterator(mFront, *this));
	}

	template <typename T>
	typename SList<T>::ConstIterator SList<T>::cend() const
	{
		return(ConstIterator(nullptr, *this));
	}

	template <typename T>
	typename void SList<T>::InsertAfter(const T& data, const Iterator& iter)
	{
		if (this != iter.mOwner)
		{
			throw std::runtime_error("Iterator does not belong to this list");
		}
		else
		{
			if (iter == end())
			{
				PushBack(data);
			}
			else
			{
				Node* insert = new Node(data, iter.mCurrent->Next);
				if (mBack == iter.mCurrent)
				{
					mBack = insert;
				}
				iter.mCurrent->Next = insert;
				++mSize;
			}
		}
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::Find(const T& data) 
	{
		Iterator iter = begin();
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
	typename SList<T>::ConstIterator SList<T>::Find(const T& data) const
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
	typename bool SList<T>::Remove(const T& data)
	{
		return Remove(Find(data));
	}

	template <typename T>
	typename bool SList<T>::Remove(const Iterator& it)
	{
		if (it.mOwner != this)
		{
			throw std::runtime_error("Invalid iterator. Owned by different container.");
		}

		bool found = false;
		if (it != end())
		{
			if (it.mCurrent == mBack)
			{
				PopBack();
			}
			else
			{
				Node* next = it.mCurrent->Next;
				// Cannot used operator= when using SList within HashMap because HashMap stores std::pair with a const key.
				it.mCurrent->Data.~T();
				new(&it.mCurrent->Data)T(std::move(next->Data));
				it.mCurrent->Next = next->Next;
				delete next;

				if (it.mCurrent->Next == nullptr)
				{
					mBack = it.mCurrent;
				}
				--mSize;
			}

			found = true;
		}

		return found;
	}
		

#pragma region Iterator 

	template <typename T>
	SList<T>::Iterator::Iterator(Node* node, const SList<T>& list) : mCurrent(node), mOwner(&list)
	{

	}

	template <typename T>
	bool SList<T>::Iterator::operator==(const Iterator& rhs) const
	{
		return (mCurrent == rhs.mCurrent) && (mOwner == rhs.mOwner);
	}
	
	template <typename T>
	bool SList<T>::Iterator::operator!=(const Iterator& rhs) const
	{
		return !(*this == rhs);
	}

	template <typename T>
	typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator does not have a valid owner");
		}
		mCurrent = mCurrent->Next;
		return *this;
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		Iterator iter = *this;
		operator++();
		return iter;
	}

	template<typename T>
	T& SList<T>::Iterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator does not have a valid owner");
		}
		return mCurrent->Data;
	}
#pragma endregion

#pragma region ConstIterator


	template <typename T>
	SList<T>::ConstIterator::ConstIterator(Node* node, const SList<T>& list) : mCurrent(node), mOwner(&list)
	{

	}



	template <typename T>
	SList<T>::ConstIterator::ConstIterator(const Iterator& rhs) : mCurrent(rhs.mCurrent), mOwner(rhs.mOwner)
	{

	}

	template <typename T>
	bool SList<T>::ConstIterator::operator==(const ConstIterator& rhs) const
	{
		return (mCurrent == rhs.mCurrent) && (mOwner == rhs.mOwner);
	}

	template <typename T>
	bool SList<T>::ConstIterator::operator!=(const ConstIterator& rhs) const
	{
		return !(*this == rhs);
	}

	template <typename T>
	typename SList<T>::ConstIterator& SList<T>::ConstIterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator does not have a valid owner");
		}
		mCurrent = mCurrent->Next;
		return *this;
	}

	template <typename T>
	typename SList<T>::ConstIterator& SList<T>::ConstIterator::operator++(int)
	{
		ConstIterator iter = *this;
		operator++();
		return iter;
	}

	template<typename T>
	const T& SList<T>::ConstIterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Iterator does not have a valid owner");
		}
		return mCurrent->Data;
	}


#pragma endregion

}