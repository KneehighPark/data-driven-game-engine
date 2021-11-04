#pragma once

namespace Library
{
	template <typename T>

	/// <summary>
	/// Custom implementation of a singly-linked list
	/// </summary>
	class SList
	{
	private:
		struct Node final
		{
			T Data;
			Node* Next;

			Node(const T& data, Node* next = nullptr);
		};

	public:
		/// <summary>
		/// Default constructor for singly-linked list (user declared, compiler provided)
		/// </summary>
		SList() = default;			

		/// <summary>
		/// Copy constructor for singly-linked list
		/// </summary>
		/// <param name="rhs">List to be copied</param>
		SList(const SList& rhs);	

		/// <summary>
		/// Move constructor for singly-linked list
		/// </summary>
		/// <param name="rhs">List to be copied</param>
		SList(SList&& rhs);

		/// <summary>
		/// Copy assignment operator for singly-linked list
		/// </summary>
		/// <param name="rhs">List to be copied</param>
		SList& operator=(const SList& rhs);
		
		/// <summary>
		/// Move assignment operator for singly-linked list
		/// </summary>
		/// <param name="rhs">List to be moved</param>
		SList& operator=(SList&& rhs);			

		/// <summary>
		/// Deconstructor
		/// </summary>
		~SList();
		
		/// <summary>
		/// Returns size of singly-linked list
		/// </summary>
		/// <returns>Size of singly-linked list</returns>
		size_t Size() const;
		
		/// <summary>
		/// Returns whether or not the singly-linked list is empty or not
		/// </summary>
		/// <returns>Boolean representing whether the list is empty or not </returns>
		bool IsEmpty() const;

		/// <summary>
		/// Returns front/head node of singly-linked list
		/// </summary>
		/// <returns>Front/head node</returns>
		/// <exception cref="List is empty">Throws exception if list is empty</exception>
		T& Front();
		
		/// <summary>
		/// Returns tail/end node of singly-linked list
		/// </summary>
		/// <returns></returns>
		/// <exception cref="List is empty">Throws exception if list is empty</exception>
		T& Back();
		
		/// <summary>
		/// Returns front/head node of singly-linked list
		/// </summary>
		/// <returns>Front/head node</returns>
		/// <exception cref="List is empty">Throws exception if list is empty</exception>
		const T& Front() const;
		
		/// <summary>
		/// Returns tail/end node of singly-linked list
		/// </summary>
		/// <returns>Back/tail node</returns>
		/// <exception cref="List is empty">Throws exception if list is empty</exception>
		const T& Back() const;

		
		
		/// <summary>
		/// Adds new node to the front of the list
		/// </summary>
		/// <param name="data">Data to be added</param>
		void PushFront(const T& data);
		
		/// <summary>
		/// Removes front/head node from the list
		/// </summary>
		void PopFront();
		
		/// <summary>
		/// Removes tail/end node from the list
		/// </summary>
		void PopBack();
		
		/// <summary>
		/// Clears the list by removing each node
		/// </summary>
		void Clear();

		/// <summary>
		/// Iterator class used to traverse SList
		/// </summary>
		class Iterator final
		{
			friend SList;
			friend class ConstIterator;

		public:
			/// <summary>
			///  Constructor for iterator (without owner) 
			/// </summary>
			Iterator() = default;

			/// <summary>
			/// Copy constructor for iterator
			/// </summary>
			/// <param name="rhs">Const reference to iterator being copied</param>
			Iterator(const Iterator& rhs) = default;

			/// <summary>
			/// Copy assignment operator for iterator
			/// </summary>
			/// <param name="rhs">Const reference of iterator to be copied</param>
			/// <returns>Reference to current iterator</returns>
			Iterator& operator=(const Iterator& rhs) = default;

			/// <summary>
			/// Move assignment operator for iterator
			/// </summary>
			/// <param name="rhs">R value reference of iterator to be moved</param>
			/// <returns>Reference to current iterator</returns>
			Iterator& operator= (Iterator&& rhs) = default;

			/// <summary>
			/// Move constructor for iterator
			/// </summary>
			/// <param name="rhs">R value reference of iterator to be moved</param>
			Iterator(Iterator&& rhs) = default;

			/// <summary>
			/// Compares two iterators for equality
			/// </summary>
			/// <param name="rhs">Const reference to iterator being compared to</param>
			/// <returns>True if owner and current node of both iterators are the same</returns>
			bool operator== (const Iterator& rhs) const;

			/// <summary>
			/// Complement of the equal equal operator
			/// </summary>
			/// <param name="rhs">Const reference to iterator being compared to</param>
			/// <returns>True if owner and current node of both iterators are not the same</returns>
			bool operator!= (const Iterator& rhs) const;

			/// <summary>
			/// Prefix increment operator
			/// </summary>
			/// <returns>Iterator pointing to the next node</returns>
			/// <exception cref="Null pointer exception">Iterator points to a null pointer</exception>
			/// <exception cref="Iterator does not have a valid owner">Iterator does not belong to a valid SList</exception>
			Iterator& operator++();

			/// <summary>
			/// Postfix increment operator
			/// </summary>
			/// <param name=""></param>
			/// <returns>Iterator pointing to the same node</returns>
			Iterator operator++(int);

			/// <summary>
			/// Dereference operator 
			/// </summary>
			/// <returns>Returns element in list at iterator's current position</returns>
			/// <exception cref="Null pointer exception">Iterator points to a null pointer</exception>
			/// <exception cref="Iterator does not have a valid owner">Iterator does not belong to a valid SList</exception>
			T& operator*() const;

			friend class SList<T>;

		private:

			/// <summary>
			/// Constructor for iterator
			/// </summary>
			/// <param name="node">Current node iterator points to</param>
			/// <param name="list">List that owns the iterator</param>
			Iterator(Node* node, const SList& list);

			Node* mCurrent{ nullptr };
			const SList<T>* mOwner{ nullptr };
		};

		/// <summary>
		/// ConstIterator class used to traverse an SList
		/// </summary>
		class ConstIterator final
		{
			friend SList;
			friend class Iterator;

		public:
			/// <summary>
			/// Constructor for const-iterator without owner
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
			/// <param name="rhs">ConstIterator</param>
			ConstIterator(const ConstIterator& rhs) = default;

			/// <summary>
			/// Copy assignment operator
			/// </summary>
			/// <param name="rhs">Const reference of const-iterator to be copied</param>
			/// <returns>Reference to current const-iterator</returns>
			ConstIterator& operator=(const ConstIterator& rhs) = default;

			/// <summary>
			/// Move assignment operator for iterator
			/// </summary>
			/// <param name="rhs">R value reference of iterator to be moved</param>
			/// <returns>Reference to current iterator</returns>
			ConstIterator& operator= (ConstIterator&& rhs) = default;

			/// <summary>
			/// Move constructor
			/// </summary>
			/// <param name="rhs">R value reference of const-iterator to be moved</param>
			ConstIterator(ConstIterator&& rhs) = default;

			/// <summary>
			/// Compares two iterators for equality
			/// </summary>
			/// <param name="rhs">Reference to iterator being compared</param>
			/// <returns>True if owner and current node of both iterators are the same</returns>
			bool operator== (const ConstIterator& rhs) const;

			/// <summary>
			/// Complement of equal equal operator
			/// </summary>
			/// <param name="rhs">Reference to iterator being compared</param>
			/// <returns>True if owner and current node of both iterators are not equal</returns>
			bool operator!= (const ConstIterator& rhs) const;

			/// <summary>
			/// Prefix increment operator
			/// </summary>
			/// <returns>ConstIterator pointing to the next node</returns>
			/// <exception cref="Null pointer exception">Iterator points to a null pointer</exception>
			/// <exception cref="Iterator does not have a valid owner">Iterator does not belong to a valid SList</exception>
			ConstIterator& operator++();

			/// <summary>
			/// Postfix increment operator
			/// </summary>
			/// <param name=""></param>
			/// <returns>ConstIterator pointing to the same node</returns>
			ConstIterator& operator++(int);

			/// <summary>
			/// Dereference operator
			/// </summary>
			/// <returns>Const T reference</returns>
			const T& operator*() const;

			friend class SList<T>;

		private:
			/// <summary>
			/// Constructor for ConstIterator
			/// </summary>
			/// <param name="node">Node that iterator points to</param>
			/// <param name="list">List that owns iterator</param>
			ConstIterator(Node* node, const SList& list);

			Node* mCurrent{ nullptr };
			const SList<T>* mOwner{ nullptr };
		};

		/// <summary>
		/// Returns an SList Iterator pointing to the head of the list
		/// </summary>
		/// <returns>SList Iterator</returns>
		Iterator begin();

		/// <summary>
		/// Returns an SList Iterator pointing past the end of the list
		/// </summary>
		/// <returns>SList Iterator</returns>
		Iterator end();

		/// <summary>
		/// Returns an SList ConstIterator pointing to the head of the list
		/// </summary>
		/// <returns>SList ConstIterator</returns>
		ConstIterator begin() const;

		/// <summary>
		/// Returns an SList ConstIterator pointing past the end of the list
		/// </summary>
		/// <returns></returns>
		ConstIterator end() const;

		/// <summary>
		/// Returns an SList ConstIterator pointing to the head of the list (const only)
		/// </summary>
		/// <returnsSList ConstIterator></returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Returns an SList ConstIterator pointing past the end of the list (const only)
		/// </summary>
		/// <returns>SList ConstIterator</returns>
		ConstIterator cend() const;

		/// <summary>
		/// Appends the given data after the data the iterator points to
		/// </summary>
		/// <param name="data">Data to be appended</param>
		/// <param name="iter">Iterator for the list</param>
		/// <exception cref="Iterator does not belong to the list">Iterator does not belong to the invoking singly-linked list</exception>
		void InsertAfter(const T& data, const Iterator& iter);

		/// <summary>
		/// Returns iterator pointing to given item/data
		/// </summary>
		/// <param name="data">Data/item in the list</param>
		/// <returns>Iterator pointing to the item/data</returns>
		Iterator Find(const T& data);

		/// <summary>
		/// Returns constiterator pointing ot given item/data
		/// </summary>
		/// <param name="data">Data/item in the list</param>
		/// <returns>ConstIterator pointing to the item/data</returns>
		ConstIterator Find(const T& data) const;

		/// <summary>
		/// Removes item/data and maintains list integrity
		/// </summary>
		/// <param name="data">Item/data to be removed</param>
		bool Remove(const T& data);
		
		bool Remove(const Iterator& it);

		/// <summary>
		/// Adds new node to the back of the list
		/// </summary>
		/// <param name="data">Data to be added</param>
		Iterator PushBack(const T& data);

		
	
	private:
		size_t mSize{ 0 };			//initializes to 0
		Node* mFront{ nullptr };
		Node* mBack{ nullptr };
	};
}

#include "SList.inl"
