#pragma once
#include "SList.h"

namespace Library
{
	/// <summary>
	/// Implementation of a stack that uses an SList
	/// </summary>
	template <typename T>
	class Stack final
	{
	public:

		/// <summary>
		/// Default constructor for Stack
		/// </summary>
		Stack() = default;

		/// <summary>
		/// Destructor for Stack
		/// </summary>
		~Stack();

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="rhs">Const reference to Stack that is to be copied</param>
		Stack(const Stack& rhs);

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="rhs">R-value reference to a Stack that is to be moved</param>
		Stack(Stack&& rhs);

		/// <summary>
		/// Copy assignment operator
		/// </summary>
		/// <param name="rhs">Const reference to a Stack that is to be copied</param>
		/// <returns>Reference to a Stack</returns>
		Stack& operator=(const Stack& rhs);

		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="rhs">R-value reference to Stack that is to be moved</param>
		/// <returns></returns>
		Stack& operator=(Stack&& rhs);

		/// <summary>
		/// Pushes element to the top of the stack
		/// </summary>
		/// <param name="value"></param>
		void Push(const T& value);

		/// <summary>
		/// Removes element from the top of the stack
		/// </summary>
		void Pop();
		
		/// <summary>
		/// Gets reference to element on the top of the stack
		/// </summary>
		/// <returns>Reference to the element on the top of the stack</returns>
		T& Top();

		/// <summary>
		/// Const version of Top()
		/// </summary>
		/// <returns>Const reference to the element on the top of the stack</returns>
		const T& Top() const;

		/// <summary>
		/// Provides size of the stack (number of elements)
		/// </summary>
		/// <returns>Size of the stack</returns>
		size_t Size() const;

		/// <summary>
		/// Determines whether or not stack is empty
		/// </summary>
		/// <returns>True if empty, false if not</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Clears stack (removes all of its elements)
		/// </summary>
		void Clear();

	private:
		SList<T> mList;
	};
}

#include "Stack.inl"