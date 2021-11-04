#include "pch.h"
#include "Action.h"
#include "Entity.h"

namespace Library
{
	RTTI_DEFINITIONS(Action)

	const std::string Action::ActionName = "Name";

	const std::string& Action::Name() const
	{
		return mActionName;
	}

	void Action::SetName(const std::string& name)
	{
		mActionName = name;
	}

	Entity* Action::GetEntity() const
	{
		assert(GetParent()->Is(Entity::TypeIdClass()));
		return static_cast<Entity*>(GetParent());
	}

	void Action::SetEntity(Entity* entity)
	{
		if (entity != nullptr)
		{
			entity->Adopt(*this, "Actions");
		}
	}

	Vector<Signature> Action::Signatures()
	{
		return Vector<Signature>
		{
			{ActionName, Datum::DatumType::STRING, 1, offsetof(Action, mActionName)}
		};
	}

	Action::Action(const RTTI::IdType typeID, const std::string& name) : Attributed(typeID), mActionName(name)
	{

	}

}