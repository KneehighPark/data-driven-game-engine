#pragma once
#include "Entity.h"

namespace UnitTests
{
	class Avatar : public Library::Entity
	{
		RTTI_DECLARATIONS(Avatar, Entity)

	public:
		static Library::Vector<Library::Signature> Signatures();
		void Update(Library::WorldState& worldState);
		int Health;
	};

	CONCRETE_FACTORY(Avatar, Library::Scope)
}