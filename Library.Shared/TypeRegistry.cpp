#include "pch.h"
#include "TypeRegistry.h"

namespace Library
{
	HashMap<size_t, Vector<Signature>> TypeRegistry::type_hashmap;

	void TypeRegistry::RegisterType(const RTTI::IdType typeID, const Vector<Signature>& signatures)
	{
		type_hashmap.Insert(std::make_pair(typeID, signatures));
	}

	void TypeRegistry::UnregisterType(const RTTI::IdType typeID)
	{
		type_hashmap.Remove(typeID);
	}

	Vector<Signature>& TypeRegistry::GetSignatures(const RTTI::IdType typeID)
	{
		return type_hashmap.At(typeID);
	}

	void TypeRegistry::Clear()
	{
		type_hashmap.Clear();
	}
}