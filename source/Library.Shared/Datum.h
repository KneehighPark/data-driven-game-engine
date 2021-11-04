#pragma once

#include "RTTI.h"
#pragma warning(push)
#pragma warning(disable:4201)
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#pragma warning(pop)
#include <cstdint>
#include <string>
#include <sstream>
#include <tuple>
#include "HashMap.h"


namespace Library
{
	/// <summary>
	/// Forward declaration of Scope
	/// </summary>
	class Scope;

	/// <summary>
	/// Runtime polymorphic vector
	/// </summary>
	class Datum final
	{

		friend class Attributed;

	public: 

		/// <summary>
		/// Data types supported by Datum
		/// </summary>
		enum class DatumType
		{
			UNKNOWN,
			INTEGER,
			FLOAT,
			VECTOR4,
			MATRIX4X4, 
			TABLE,
			STRING,
			POINTER
		};

		static const int8_t DatumTypeSizes[];

		/// <summary>
		/// Default constructor -- initialized empty, no memory allocated
		/// </summary>
		Datum();

		/// <summary>
		/// Destructor
		/// </summary>
		~Datum();

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="rhs">Const R-Value reference that will be copied</param>
		Datum(const Datum& rhs);

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="rhs">R-Value reference that will be moved</param>
		Datum(Datum&& rhs);

		/// <summary>
		/// Copy assignment operator
		/// </summary>
		/// <param name="rhs">Const reference to Datum that will be copied</param>
		/// <returns>Reference to current Datum</returns>
		Datum& operator= (const Datum& rhs);

		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="rhs">R-value reference to Datum that will be moved</param>
		/// <returns>Reference to current Datum</returns>
		Datum& operator= (Datum&& rhs);

		/// <summary>
		/// Assignment operator overloads for each supported DatumType
		/// </summary>
		/// <param name="rhs">Reference to supported DatumType</param>
		/// <returns>Reference to modified datum</returns>
		Datum& operator=(const std::int32_t& rhs);
		Datum& operator=(const std::float_t& rhs);
		Datum& operator=(const glm::vec4& rhs);
		Datum& operator=(const glm::mat4x4& rhs);
		Datum& operator=(const std::string & rhs);
		Datum& operator=(RTTI* rhs);
		Datum& operator=(Scope& rhs);

		/// <summary>
		/// Returns DatumType of Datum
		/// </summary>
		/// <returns>DatumTpe</returns>
		DatumType Type() const;

		/// <summary>
		/// Sets DatumType
		/// </summary>
		/// <param name="type">Type that Datum is to be set to</param>
		void SetType(DatumType type);

		/// <summary>
		/// Returns Datum's size
		/// </summary>
		/// <returns>Datum's size</returns>
		size_t Size() const;
		
		/// <summary>
		/// Returns Datum's capacity
		/// </summary>
		/// <returns>Datum's capacity</returns>
		size_t Capacity() const;

		/// <summary>
		/// Set's number of values for Datum, reserves memory if needed (supports shrinking and growing)
		/// </summary>
		/// <param name="size">New size for Datum</param>
		void Resize(size_t newSize);

		/// <summary>
		/// Allocates/reallocates elements based on new capacity
		/// </summary>
		/// <param name="newCapacity">New capacity for Datum</param>
		void Reserve(size_t newCapacity);

		/// <summary>
		/// Clears Datum, does not alter capacity
		/// </summary>
		void Clear();

		/// <summary>
		/// Retrieves storage type of Datum
		/// </summary>
		/// <returns>True if external, false if internal</returns>
		bool IsExternal() const;

		/// <summary>
		/// Assigns internal values array to the given array, sets size and capacity
		/// </summary>
		/// <param name="type">Datum type</param>
		/// <param name="arraySize">Array size</param>
		void SetStorage(const DatumType& type, size_t arraySize);

		/// <summary>
		/// Assigns internal values array to the given array, sets size and capacity
		/// </summary>
		/// <param name="array">Given array</param>
		/// <param name="arraySize">Given size</param>
		void SetStorage(std::int32_t* array, size_t arraySize);
		void SetStorage(std::float_t* array, size_t arraySize);
		void SetStorage(glm::vec4* array, size_t arraySize);
		void SetStorage(glm::mat4x4* array, size_t arraySize);
		void SetStorage(std::string* array, size_t arraySize);
		void SetStorage(RTTI** array, size_t arraySize);

		/// <summary>
		/// Comparison operator for two Datums
		/// </summary>
		/// <param name="rhs">Const reference to passed datum</param>
		/// <returns>True if equal, false if not</returns>
		bool operator==(const Datum& rhs) const;

		/// <summary>
		/// Comparison operator for Datum types
		/// </summary>
		/// <param name="rhs">Const reference to datum type</param>
		/// <returns>True if equal, false if not</returns>
		bool operator==(const int32_t& rhs) const;
		bool operator==(const float_t& rhs) const;
		bool operator==(const glm::vec4& rhs) const;
		bool operator==(const glm::mat4x4& rhs) const;
		bool operator==(const std::string& rhs) const;
		bool operator==(const RTTI* rhs) const;

		/// <summary>
		/// Not equal operator for two Datums
		/// </summary>
		/// <param name="rhs">Const reference to passed datum</param>
		/// <returns>False if equal, true if not</returns>
		bool operator!=(const Datum& rhs) const;

		/// <summary>
		/// Not equal operator for Datum types
		/// </summary>
		/// <param name="rhs">Const reference to datum type</param>
		/// <returns>False if equal, true if not</returns>
		bool operator!=(const int32_t& rhs) const;
		bool operator!=(const float_t& rhs) const;
		bool operator!=(const glm::vec4& rhs) const;
		bool operator!=(const glm::mat4x4& rhs) const;
		bool operator!=(const std::string& rhs) const;
		bool operator!=(const RTTI* rhs) const;

		/// <summary>
		/// Assign the appropriate element in the values array 
		/// </summary>
		/// <param name="type">Given Datum type</param>
		/// <param name="index">Given index</param>
		void Set(const DatumType& type, size_t index = 0);

		/// <summary>
		/// Assign the approriate element in the values array
		/// </summary>
		/// <param name="value">Given value</param>
		/// <param name="index">Given index</param>
		void Set(const int32_t& value, size_t index = 0);
		void Set(const float_t& value, size_t index = 0);
		void Set(const glm::vec4& value, size_t index = 0);
		void Set(const glm::mat4x4& value, size_t index = 0);
		void Set(const std::string& value, size_t index = 0);
		void Set(RTTI* value, size_t index = 0);
		void Set(Scope& value, size_t index = 0);

		/// <summary>
		/// Returns value from array at given index
		/// </summary>
		/// <param name="index">Given index</param>
		/// <returns>Type T reference</returns>
		template <typename T> T& Get(size_t index = 0);

		/// <summary>
		/// Returns value from array at given index
		/// </summary>
		/// <param name="index">Given index</param>
		/// <returns>Reference to appropriate value</returns>
		template <> int32_t& Get(size_t index);
		template <> float_t& Get(size_t index);
		template <> glm::vec4& Get(size_t index);
		template <> glm::mat4x4& Get(size_t index);
		template <> std::string& Get(size_t index);
		template <> RTTI*& Get(size_t index);
		template <> Scope& Get<Scope>(size_t index);

		/// <summary>
		/// Const version of Get
		/// </summary>
		/// <param name="index">Given index</param>
		/// <returns>Const Type T Reference</returns>
		template <typename T> const T& Get(size_t index = 0) const;

		/// <summary>
		/// Const versions of Get
		/// </summary>
		/// <param name="index">Given index</param>
		/// <returns>Const reference to appropriate value</returns>
		template <> const int32_t& Get(size_t index) const;
		template <> const float_t& Get(size_t index) const;
		template <> const glm::vec4& Get(size_t index) const;
		template <> const glm::mat4x4& Get(size_t index) const;
		template <> const std::string& Get(size_t index) const;
		template <> RTTI* const& Get(size_t index) const;
		template <> const Scope& Get<Scope>(size_t index) const;

		/// <summary>
		/// Assigns element in values array to given value via provided string
		/// </summary>
		/// <param name="string">Provided string</param>
		/// <param name="index">Provided index</param>
		void SetFromString(const std::string& string, size_t index = 0);

		/// <summary>
		/// Provides string representing element in the values array at provided index
		/// </summary>
		/// <param name="index">Provided index</param>
		/// <returns>Element represented as a string</returns>
		std::string ToString(size_t index = 0);

		/// <summary>
		/// Appends item to the end of the array
		/// </summary>
		/// <param name="item">Const reference to item that is to be appended</param>
		void PushBack(const int32_t& item);
		void PushBack(const float_t& item);
		void PushBack(const glm::vec4& item);
		void PushBack(const glm::mat4x4& item);
		void PushBack(const std::string& item);
		void PushBack(RTTI* item);
		void PushBack(Scope& item);

		/// <summary>
		/// Removes last element in the Datum's values array
		/// </summary>
		void PopBack();

		/// <summary>
		/// Retrieves first element from the values array
		/// </summary>
		/// <returns>Reference of Type T</returns>
		template <typename T> T& Front();

		/// <summary>
		/// Const version of Find()
		/// </summary>
		/// <returns>Const reference of Type T</returns>
		template <typename T> const T& Front() const;

		/// <summary>
		/// Retrives last element from the values array
		/// </summary>
		/// <returns>Reference of Type T</returns>
		template <typename T> T& Back();

		/// <summary>
		/// Const version of Back()
		/// </summary>
		/// <returns>Const reference of Type T</returns>
		template <typename T> const T& Back() const;

		/// <summary>
		/// Removes given item from Datum. If item is not found, nothing happens.
		/// </summary>
		/// <param name="item">Item to be removed</param>
		/// <returns>True if item is removed, false if not</returns>
		bool Remove(const int32_t& item);
		bool Remove(const float_t& item);
		bool Remove(const glm::vec4& item);
		bool Remove(const glm::mat4x4& item);
		bool Remove(const std::string& item);
		bool Remove(const RTTI& item);
		bool Remove(const Scope& item);

		/// <summary>
		/// Removes item at provided index from Datum. If item is not found, nothing happens.
		/// </summary>
		/// <param name="index">Provided index</param>
		void RemoveAt(size_t index);

		/// <summary>
		/// Finds given item in the array
		/// </summary>
		/// <param name="item">Item to be found</param>
		/// <returns>Index of element if found, size of array if not</returns>
		size_t Find(const int32_t& item);
		size_t Find(const float_t& item);
		size_t Find(const glm::vec4& item);
		size_t Find(const glm::mat4x4& item);
		size_t Find(const std::string& item);
		size_t Find(const RTTI& item);
		size_t Find(const Scope& item);

		const size_t Find(const int32_t& item) const;
		const size_t Find(const float_t& item) const;
		const size_t Find(const glm::vec4& item) const;
		const size_t Find(const glm::mat4x4& item) const;
		const size_t Find(const std::string& item) const;
		const size_t Find(const RTTI& item) const;
		const size_t Find(const Scope& item) const;

		/// <summary>
		/// Access operator for nested scopes
		/// </summary>
		/// <param name="index">Index of Scope</param>
		/// <returns>Reference to Scope</returns>
		Scope& operator[](size_t index);

		/// <summary>
		/// Const version of access operator for nested scopes
		/// </summary>
		/// <param name="index">Index of Scope</param>
		/// <returns>Const reference to Scope</returns>
		const Scope& operator[](size_t index) const;

		const static HashMap<std::string, Datum::DatumType> StringDatumTypeHashMap;

	private:

		void SetStorage(void* array, size_t arraySize);

		/// <summary>
		/// Union for storing pointer
		/// </summary>
		union Data
		{
			std::int32_t* mInt = nullptr;
			std::float_t* mFloat;
			glm::vec4* mVec4;
			glm::mat4x4* mMat4x4;
			std::string* mString;
			RTTI** mRTTI;
			Scope** mScope;
			void* vp;
			uint8_t* bytes;
		};

		Data mData;
		DatumType mType = DatumType::UNKNOWN;

		size_t mSize = 0;
		size_t mCapacity = 0;
		bool mIsExternal = false;
	};
}