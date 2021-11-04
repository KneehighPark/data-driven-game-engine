#pragma once
#include "Datum.h"
#include "HashMap.h"

namespace Library
{
	/// <summary>
	/// Stores information of prescribed attributes
	/// </summary>
	struct Signature final
	{
		std::string name;
		Datum::DatumType type;
		size_t size;
		size_t offset;
	};

	/// <summary>
	/// Singleton registry that holds hashmap of all prescribed signatures
	/// </summary>
	class TypeRegistry
	{
	public:
		TypeRegistry() = delete;
		TypeRegistry(const TypeRegistry& rhs) = delete;
		TypeRegistry(TypeRegistry&& rhs) = delete;
		TypeRegistry& operator=(const TypeRegistry& rhs) = delete;
		TypeRegistry& operator=(TypeRegistry&& rhs) = delete;

		/// <summary>
		/// Static function to register prescribed attributes
		/// </summary>
		/// <param name="typeID">Const RTTI ID Type</param>
		/// <param name="signatures">Const reference to vector of signatures</param>
		static void RegisterType(const RTTI::IdType typeID, const Vector<Signature>& signatures);
		
		/// <summary>
		/// Static function to remove a vector of signatures, one that is associated with a specific RTTI TypeID
		/// </summary>
		/// <param name="typeID">RTTI ID Type</param>
		static void UnregisterType(const RTTI::IdType typeID);
		
		/// <summary>
		/// Static function to get all signatures stored in the hashmap
		/// </summary>
		/// <param name="typeID">Const RTTI ID Type</param>
		/// <returns>Vector of prescribed signatures</returns>
		static Vector<Signature>& GetSignatures(const RTTI::IdType typeID);
		
		/// <summary>
		/// Clears the registry (the hashmap)
		/// </summary>
		static void Clear();

	private:
		static HashMap<size_t, Vector<Signature>> type_hashmap;
	};
}