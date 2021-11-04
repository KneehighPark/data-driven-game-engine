#pragma once
#include "pch.h"
#include "Avatar.h"

using namespace Library;

namespace UnitTests
{
	RTTI_DEFINITIONS(Avatar)

	void Avatar::Update(WorldState& /*worldState*/)
	{
		Health++;
	}

	Vector<Signature> Avatar::Signatures()
	{
		return Vector<Signature>
		{
			{"Health", Datum::DatumType::INTEGER, 1, offsetof(Avatar, Health)}
		};
	}
}