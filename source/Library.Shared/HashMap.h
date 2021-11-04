#pragma once
#include "Vector.h"
#include "SList.h"
#include "HashFunctions.h"
#include <initializer_list>

namespace Library
{
	template <typename TKey, typename TData, typename HashFunctor = HashFunctions<TKey>>

	/// <summary>
	/// Hashmap class
	/// </summary>
	class HashMap final
	{

	public: 
		using PairType = std::pair<TKey, TData>;

	private:

		using ChainType = SList<PairType>;
		using BucketType = Vector<ChainType>;
		using ChainingIterator = typename ChainType::Iterator;
		using ConstChainingIterator = typename ChainType::ConstIterator;

	public:

		/// <summary>
		/// Iterator for hashmap
		/// </summary>
		class Iterator final
		{
			friend HashMap;
			friend class ConstIterator;

		public:

			/// <summary>
			/// Default constructor
			/// </summary>
			Iterator() = default;

			/// <summary>
			/// Default copy constructor
			/// </summary>
			/// <param name="rhs">Const reference to copied iterator</param>
			Iterator(const Iterator& rhs) = default;

			/// <summary>
			///Default move constructor
			///</summary>
			/// <param name="rhs">R-value reference to iterator which will be moved.</param>
			Iterator(Iterator&& rhs) = default;

			/// <summary>Default 
			/// Copy assignment operator.
			/// </summary>
			/// <param name="rhs">Const reference to passed Iterator.</param>
			/// <returns>Reference to an Iterator.</returns>
			Iterator& operator=(const Iterator& rhs) = default;

			/// <summary>
			/// Move assignment operator
			/// </summary>
			/// <param name="rhs">R-value reference to iterator that is being moved.</param>
			/// <returns>Reference to an Iterator.</returns>
			Iterator& operator=(Iterator && rhs) = default;

			/// <summary>
			/// Default destructor for HashMap Iterator
			/// </summary>
			~Iterator() = default;

			/// <summary>
			/// Prefix Increment operator
			/// </summary>
			/// <returns>Reference to an Iterator.</returns>
			Iterator& operator++();

			/// <summary>
			/// Postfix Increment operator
			/// </summary>
			/// <returns>Iterator before increment operation.</returns>
			Iterator operator++(int);

			/// <summary>
			/// Dereference operator
			/// </summary>
			/// <returns>Reference to key/value PairType pointed to by current Iterator.</returns>
			PairType& operator*() const;

			/// <summary>
			/// Dereference operator
			/// </summary>
			/// <returns>Pointer to key/value PairType pointed to by current Iterator.</returns>
			PairType* operator->() const;

			/// <summary>
			/// Comparison operator
			/// </summary>
			/// <param name="rhs">Const reference to passed Iterator.</param>
			/// <returns>Boolean value indicating whether two iterators are equal or not.</returns>
			bool operator==(const Iterator & rhs) const;

			/// <summary>
			/// Not equal operator
			/// </summary>
			/// <param name="rhs">Const reference to passed Iterator</param>
			/// <returns>Boolean value indicating whether two iterators aren't equal or not.</returns>
			bool operator!=(const Iterator & t_ths) const;

		private:
			Iterator(HashMap& owner, size_t currentIndex, const ChainingIterator& currentPair);
			size_t mBucketIndex = 0;
			ChainingIterator mPair;
			HashMap* mOwner = nullptr;

		};

		/// <summary>
		/// ConstIterator for hashmap
		/// </summary>
		class ConstIterator final
		{
			friend HashMap;
			friend class Iterator;

		public:

			/// <summary>
			/// Copy constructor for converting Iterator to ConstIterator
			/// </summary>
			/// <param name="rhs">Const reference to iterator that is to be copied</param>
			ConstIterator(const Iterator& rhs);

			/// <summary>
			/// Default constructor
			/// </summary>
			ConstIterator() = default;

			/// <summary>
			/// Default copy constructor
			/// </summary>
			/// <param name="rhs">Const reference to copied ConstIterator</param>
			ConstIterator(const ConstIterator& rhs) = default;

			/// <summary>
			///Default move constructor
			///</summary>
			/// <param name="rhs">R-value reference to ConstIterator which will be moved.</param>
			ConstIterator(ConstIterator&& rhs) = default;

			/// <summary>Default 
			/// Copy assignment operator.
			/// </summary>
			/// <param name="rhs">Const reference to passed ConstIterator.</param>
			/// <returns>Reference to an ConstIterator.</returns>
			ConstIterator& operator=(const ConstIterator & rhs) = default;

			/// <summary>
			/// Move assignment operator
			/// </summary>
			/// <param name="rhs">R-value reference to ConstIterator that is being moved.</param>
			/// <returns>Reference to an ConstIterator.</returns>
			ConstIterator& operator=(ConstIterator && rhs) = default;

			/// <summary>
			/// Default destructor for ConstIterator
			/// </summary>
			~ConstIterator() = default;

			/// <summary>
			/// Prefix Increment operator
			/// </summary>
			/// <returns>Reference to an ConstIterator.</returns>
			ConstIterator& operator++();

			/// <summary>
			/// Postfix Increment operator
			/// </summary>
			/// <returns>ConstIterator before increment operation.</returns>
			ConstIterator operator++(int);

			/// <summary>
			/// Dereference operator
			/// </summary>
			/// <returns>Reference to key/value PairType pointed to by current ConstIterator.</returns>
			const PairType& operator*() const;

			/// <summary>
			/// Dereference operator
			/// </summary>
			/// <returns>Pointer to key/value PairType pointed to by current ConstIterator.</returns>
			const PairType* operator->() const;

			/// <summary>
			/// Comparison operator
			/// </summary>
			/// <param name="rhs">Const reference to passed ConstIterator.</param>
			/// <returns>Boolean value indicating whether two iterators are equal or not.</returns>
			bool operator==(const ConstIterator & rhs) const;

			/// <summary>
			/// Not equal operator
			/// </summary>
			/// <param name="rhs">Const reference to passed ConstIterator</param>
			/// <returns>Boolean value indicating whether two iterators aren't equal or not.</returns>
			bool operator!=(const ConstIterator & t_ths) const;

		private:
			ConstIterator(const HashMap& owner, size_t currentIndex, const ConstChainingIterator& currentPair);
			size_t mBucketIndex = 0;
			ConstChainingIterator mPair;
			const HashMap* mOwner = nullptr;
		};

		/// <summary>
		/// Parameterized constructor
		/// </summary>
		/// <param name="numberOfBuckets">Size of the hash table array (number of buckets)</param>
		explicit HashMap(size_t numberOfBuckets = DEFAULT_NUM_BUCKETS);

		/// <summary>
		/// Initializer list constructor
		/// </summary>
		/// <param name="list">Initializer arguments</param>
		HashMap(std::initializer_list<PairType> list);

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="rhs">Const reference of hashmap to be copied</param>
		HashMap(const HashMap& rhs) = default;
		
		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="rhs">R value reference of hashmap to be copied</param>
		HashMap(HashMap&& rhs) = default;

		/// <summary>
		/// Copy assignment operator
		/// </summary>
		/// <param name="rhs">Const reference of hashmap to be copied</param>
		/// <returns>Reference of hashmap</returns>
		HashMap& operator= (const HashMap& rhs) = default;
		
		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="rhs">R value reference of hashmap to be copied</param>
		/// <returns>Reference of hashmap</returns>
		HashMap& operator= (HashMap&& rhs) = default;

		/// <summary>
		/// Destructor 
		/// </summary>
		~HashMap() = default;

		/// <summary>
		/// Finds whether a given key exists in the hashmap
		/// </summary>
		/// <param name="key">Const reference to a key</param>
		/// <returns>Iterator to an element found or iterator to the end</returns>
		Iterator Find(const TKey& key);

		/// <summary>
		/// Const version of Find
		/// </summary>
		/// <param name="key">Const reference to a key</param>
		/// <returns>ConstIterator to an element or to the end</returns>
		ConstIterator Find(const TKey& key) const;

		/// <summary>
		/// Inserts element into the hashmap
		/// </summary>
		/// <param name="pair">Const reference to a pair type</param>
		/// <returns>Iterator to inserted element, or iterator to existing element</returns>
		Iterator Insert(const PairType& pair);
		Iterator Insert(const PairType& pair, bool& result);


		/// <summary>
		/// Index operator
		/// </summary>
		/// <param name="key">Const reference to a key</param>
		/// <returns>Reference to data</returns>
		TData& operator[](const TKey& key);

		/// <summary>
		/// Removes element from hashmap
		/// </summary>
		/// <param name="key">Key</param>
		void Remove(const TKey& key);

		/// <summary>
		/// Empties table, deletes any allocated memory
		/// </summary>
		void Clear();

		/// <summary>
		/// Rehashes the hashmap
		/// </summary>
		/// <param name="numberOfBuckets">New number of buckets to be used during rehash</param>
		void Rehash(size_t numberOfBuckets);

		/// <summary>
		/// Returns population of the table
		/// </summary>
		/// <returns>Population (size) of the table</returns>
		size_t Size() const;

		/// <summary>
		/// Provides ratio of used buckets to total buckets in hashmap
		/// </summary>
		/// <returns>Load factor (aforementioned ratio)</returns>
		float_t LoadFactor() const;

		/// <summary>
		/// Provides total number of buckets in the hashmap
		/// </summary>
		/// <returns>Total number of buckets</returns>
		size_t NumberOfBuckets() const;

		/// <summary>
		/// Determines if key is in hash map
		/// </summary>
		/// <param name="key">Key to be "found"</param>
		/// <returns>True if key is in hash map, false if not</returns>
		bool ContainsKey(const TKey& key);

		/// <summary>
		/// Determines if key is in hash map
		/// </summary>
		/// <param name="key">Key to be "found"</param>
		/// <param name="iter">Iterator pointing to found item (or end, if item is not found)</param>
		/// <returns>True if key is in hash map, false if not</returns>
		bool ContainsKey(const TKey & key, Iterator & iter);

		/// <summary>
		/// Returns a TData reference for the provided key
		/// </summary>
		/// <param name="key">Const TKey Reference</param>
		/// <returns>TData reference</returns>
		TData& At(const TKey& key);

		/// <summary>
		/// Const version of At
		/// </summary>
		/// <param name="key">Const TKey reference</param>
		/// <returns>Const TData Reference</returns>
		const TData& At(const TKey& key) const;

		/// <summary>
		/// Provides iterator to first element in hashmap
		/// </summary>
		/// <returns>Iterator to first element in hashmap</returns>
		Iterator begin();

		/// <summary>
		/// Const version of begin
		/// </summary>
		/// <returns>ConstIterator to first element in hashmap</returns>
		ConstIterator begin() const;

		/// <summary>
		/// Provides iterator pointing to end of hashmap
		/// </summary>
		/// <returns>Iterator to the end of the hashmap</returns>
		Iterator end();

		/// <summary>
		/// Const version of end
		/// </summary>
		/// <returns>ConstIterator to the end of the hashmap</returns>
		ConstIterator end() const;

		/// <summary>
		/// Provides ConstIterator of the first element in the hashmap
		/// </summary>
		/// <returns>ConstIterator of the first element in the hashmap</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Provides ConstIterator to the end of the hashmap
		/// </summary>
		/// <returns>ConstIterator to the end of the hashmap</returns>
		ConstIterator cend() const;

	private:
		Iterator Find(const TKey& key, size_t& index);

		BucketType mBuckets;
		size_t mSize;
		static const size_t DEFAULT_NUM_BUCKETS = 5;
		static const HashFunctor mHashFunction;
	};
}
#include "HashMap.inl"
#include "HashFunctions.h"