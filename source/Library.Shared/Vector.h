#pragma once
#include <cstdlib>
#include <cstddef>
#include <cstring>
#include <exception>
#include <functional>
#include <initializer_list>

namespace Library
{
	
	/// <summary>
	/// Default reserve strategy
	/// </summary>
	class IncrementStrategy final
	{
	public:
		size_t operator()(size_t size, size_t capacity) const;
	};

	template <typename T>

	/// <summary>
	/// Templated vector class
	/// </summary>
	class Vector final
	{
	private: 

		size_t mSize{ 0 };
		size_t mCapacity{ 0 };
		T* mData{ nullptr };

	public:

		using value_type = T;
		void push_back(const value_type& value) { EmplaceBack(value); }
		void push_back(value_type&& value) { EmplaceBack(std::move(value)); }

		/// <summary>
		/// Iterator class
		/// </summary>
		class Iterator final
		{
			friend Vector;
			friend class ConstIterator;

		public: 

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = T;
			using pointer = T*;
			using iterator_category = std::forward_iterator_tag;

			/// <summary>
			/// Default constructor
			/// </summary>
			Iterator() = default;

			/// <summary>
			/// Copy constructor
			/// </summary>
			/// <param name="rhs">Const reference to iterator being copied from</param>
			Iterator(const Iterator& rhs) = default;

			/// <summary>
			/// Move constructor 
			/// </summary>
			/// <param name="rhs">R value reference to iterator being copied</param>
			Iterator(Iterator&& rhs) = default;

			/// <summary>
			/// Assignment operator
			/// </summary>
			/// <param name="rhs">Const reference to iterator being copied</param>
			/// <returns>Reference to iterator</returns>
			Iterator& operator=(const Iterator& rhs) = default;

			/// <summary>
			/// Move assignment operator
			/// </summary>
			/// <param name="rhs">R value reference to iterator being copied</param>
			/// <returns></returns>
			Iterator& operator=(Iterator&& rhs) = default;

			/// <summary>
			/// Equality operator 
			/// </summary>
			/// <param name="rhs">Const reference to iterator being compared to</param>
			/// <returns>True if owners and current index of both iterators are the same, otherwise, false</returns>
			bool operator==(const Iterator& rhs) const;

			/// <summary>
			/// Not equal operator
			/// </summary>
			/// <param name="rhs">Const reference to iterator being compared to</param>
			/// <returns>Compliment of equality operator</returns>
			bool operator!=(const Iterator& rhs) const;

			/// <summary>
			/// Prefix increment operator
			/// </summary>
			/// <returns>Reference to incremented iterator</returns>
			Iterator& operator++();

			/// <summary>
			/// Postfix increment operator
			/// </summary>
			/// <param name=""></param>
			/// <returns>Reference to incremented iterator</returns>
			Iterator operator++(int);

			/// <summary>
			/// Dereference operator
			/// </summary>
			/// <returns>Const reference to data</returns>
			T& operator*() const;

		private:
			/// <summary>
			/// Constructor for iterator
			/// </summary>
			/// <param name="index">Index</param>
			/// <param name="owner">Owner</param>
			Iterator(size_t index, Vector& owner);

			size_t mIndex{ 0 };
			Vector* mOwner{ nullptr };
		};

		class ConstIterator final
		{
			friend Vector;
			friend class ConstIterator;

		public:

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = T;
			using pointer = T*;
			using iterator_category = std::forward_iterator_tag;

			/// <summary>
			/// Default constructor
			/// </summary>
			ConstIterator() = default;

			/// <summary>
			/// Conversion of Iterator to ConstIterator
			/// </summary>
			/// <param name="rhs">ConstIterator</param>
			ConstIterator(const Iterator& rhs);

			/// <summary>
			/// Copy constructor
			/// </summary>
			/// <param name="rhs">Const reference to iterator being copied from</param>
			ConstIterator(const ConstIterator& rhs) = default;

			/// <summary>
			/// Move constructor 
			/// </summary>
			/// <param name="rhs">R value reference to iterator being copied</param>
			ConstIterator(ConstIterator&& rhs) = default;

			/// <summary>
			/// Assignment operator
			/// </summary>
			/// <param name="rhs">Const reference to iterator being copied</param>
			/// <returns>Reference to ConstIterator</returns>
			ConstIterator& operator=(const ConstIterator& rhs) = default;

			/// <summary>
			/// Move assignment operator
			/// </summary>
			/// <param name="rhs">R value reference to iterator being copied</param>
			/// <returns>Reference to ConstIterator</returns>
			ConstIterator& operator=(ConstIterator&& rhs) = default;

			/// <summary>
			/// Equality operator 
			/// </summary>
			/// <param name="rhs">Const reference to ConstIterator being compared to</param>
			/// <returns>True if owners and current index of both iterators are the same, otherwise, false</returns>
			bool operator==(const ConstIterator& rhs) const;

			/// <summary>
			/// Not equal operator
			/// </summary>
			/// <param name="rhs">Const reference to ConstIterator being compared to</param>
			/// <returns>Compliment of equality operator</returns>
			bool operator!=(const ConstIterator& rhs) const;

			/// <summary>
			/// Prefix increment operator
			/// </summary>
			/// <returns>Reference to incremented ConstIterator</returns>
			ConstIterator& operator++();

			/// <summary>
			/// Postfix increment operator
			/// </summary>
			/// <param name=""></param>
			/// <returns>Reference to incremented ConstIterator</returns>
			ConstIterator operator++(int);

			/// <summary>
			/// Dereference operator
			/// </summary>
			/// <returns>Const reference to data</returns>
			const T& operator*() const;

		private:

			/// <summary>
			/// Constructor for ConstIterator
			/// </summary>
			/// <param name="index">Index</param>
			/// <param name="owner">Owner</param>
			ConstIterator(size_t index, const Vector& owner);

			size_t mIndex{ 0 };
			const Vector<T>* mOwner{ nullptr };
		};

		
		/// <summary>
		/// Constructor that takes an initial capacity
		/// </summary>
		/// <param name="capacity">Initial capacity</param>
		explicit Vector(size_t capacity = 0);

		Vector(std::initializer_list<T> list);

		/// <summary>
		/// Deconstructor
		/// </summary>
		~Vector();

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="rhs">Reference to R-value vector that will be moved</param>
		Vector(Vector&& rhs);

		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="rhs">Reference to R-value vector that will be moved</param>
		/// <returns></returns>
		Vector& operator=(Vector&& rhs);

		/// <summary>
		/// Assignment operator
		/// </summary>
		/// <param name="rhs">Reference to vector that will be assigned</param>
		/// <returns>Const reference to the vector</returns>
		Vector& operator=(const Vector& rhs);

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="rhs">Const reference to the vector that's being copied</param>
		Vector(const Vector& rhs);


		/// <summary>
		/// Gets reference to element at provided index
		/// </summary>
		/// <param name="index">Index</param>
		/// <returns>Reference to element</returns>
		/// <exception cref="Index is out of bounds">Index provided is out of bounds</exception>
		T& operator[] (size_t index);

		/// <summary>
		/// Gets const reference to element at provided index
		/// </summary>
		/// <param name="index">Index</param>
		/// <returns>Const reference to element</returns>
		/// <exception cref="Index is out of bounds">Index provided is out of bounds</exception>

		const T& operator[] (size_t index) const;

		/// <summary>
		/// Get reference to element at provided index
		/// </summary>
		/// <param name="index">Index</param>
		/// <returns>Reference to element</returns> 
		T& At(size_t index); 

		/// <summary>
		/// Gets const reference to element at provided index
		/// </summary>
		/// <param name="index">Index</param>
		/// <returns>Const reference to element</returns>
		const T& At(size_t index) const; 

		/// <summary>
		/// Removes the vector's last element
		/// </summary>
		void PopBack();

		/// <summary>
		/// Places item at the end of the vector
		/// </summary>
		Iterator PushBack(const T& data);

		/// <summary>
		/// Empties the vector, resets capacity to 0
		/// </summary>
		void Wipe();

		/// <summary>
		/// Empties the vector
		/// </summary>
		void Clear();

		/// <summary>
		/// Shrinks vector so that capacity is equal to size
		/// </summary>
		void ShrinkToFit();

		/// <summary>
		/// Checks whether vector is empty or not
		/// </summary>
		/// <returns>True if vector is empty</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Gets the size of the vector (number of T items in the vector)		/// </summary>
		/// <returns>Size of the vector as unsigned int</returns>
		size_t Size() const;

		/// <summary>
		/// Gets the capacity of the vector (number of T items that can fit in the vector)
		/// </summary>
		/// <returns>Capacity of the vector as an unsigned int</returns>
		size_t Capacity() const;

		/// <summary>
		/// Gets an Iterator pointing to the vector's first element
		/// </summary>
		/// <returns>Iterator</returns>
		Iterator begin();

		/// <summary>
		/// Gets an Iterator pointing one past the vector's last element
		/// </summary>
		/// <returns>Iterator</returns>
		Iterator end();

		/// <summary>
		/// Gets a ConstIterator pointing to the vector's first element
		/// </summary>
		/// <returns>ConstIterator</returns>
		ConstIterator begin() const;


		/// <summary>
		/// Gets an ConstIterator pointing one past the vector's last element
		/// </summary>
		/// <returns>ConstIterator</returns>
		ConstIterator end() const;

		/// <summary>
		/// Gets a ConstIterator pointing to the vector's first element
		/// </summary>
		/// <returns>ConstIterator</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Gets a ConstIterator pointing one past the vector's last element
		/// </summary>
		/// <returns>ConstIterator</returns>
		ConstIterator cend() const;

		/// <summary>
		/// Gets first item in the vector
		/// </summary>
		/// <returns>Reference to first element in the vector</returns>
		/// <exception cref="Vector is empty">Thrown when vector is empty</exception>
		T& Front();

		/// <summary>
		/// Gets first item in the vector
		/// </summary>
		/// <returns>Const reference to first element in the vector</returns>
		/// <exception cref="Vector is empty">Thrown when vector is empty</exception>
		const T& Front() const;

		/// <summary>
		/// Gets last item in the vector
		/// </summary>
		/// <returns>Reference to last element in the vector</returns>
		/// <exception cref="Vector is empty">Thrown when vector is empty</exception>
		T& Back();

		/// <summary>
		/// Gets last item in the vector
		/// </summary>
		/// <returns>Const reference to last element in the vector</returns>
		/// <exception cref="Vector is empty">Thrown when vector is empty</exception>
		const T& Back() const;

		/// <summary>
		/// Finds first element in vector that matches argument 
		/// </summary>
		/// <param name="data">Const reference of data to be found</param>
		/// <returns>Iterator pointing to found element</returns>
		Iterator Find(const T& data);

		/// <summary>
		/// Finds first element in vector that matches argument
		/// </summary>
		/// <param name="data">Const reference of data to be found</param>
		/// <returns>ConstIterator pointing to found element</returns>
		ConstIterator Find(const T& data) const;

		/// <summary>
		/// Reserves memory
		/// </summary>
		/// <param name="newCapacity">New capacity to be reserved</param>
		void Reserve(size_t newCapacity);

		/// <summary>
		/// Resizes vector
		/// </summary>
		/// <param name="newSize">New size to be resized to</param>
		void Resize(size_t newSize);

		/// <summary>
		/// Removes element from vector
		/// </summary>
		/// <param name="data">Element to be removed</param>
		/// <returns>True if removed</returns>
		bool Remove(const T& data);

		/// <summary>
		/// Removes elements from vector
		/// </summary>
		/// <param name="beginning">Iterator pointing to first element to be removed</param>
		/// <param name="ending">Iterator pointing to last element to be removed</param>
		/// <returns>True if removed</returns>
		bool Remove(const Iterator& beginning, const Iterator& ending);

		bool Remove(const Iterator& iter);

		template <typename... Args>
		Iterator EmplaceBack(Args&&... args);
	};
}

#include "Vector.inl"