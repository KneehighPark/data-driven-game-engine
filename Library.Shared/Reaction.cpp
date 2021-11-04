#include "pch.h"
#include "Reaction.h"

namespace Library
{
	RTTI_DEFINITIONS(Reaction)

	Vector<Signature> Reaction::Signatures()
	{
		return Vector<Signature>();
	}

	void Reaction::Update(WorldState&)
	{
		//No implementation
	}

	Reaction::Reaction(RTTI::IdType type, const std::string& name) : ActionList(type, name)
	{
		
	}
}