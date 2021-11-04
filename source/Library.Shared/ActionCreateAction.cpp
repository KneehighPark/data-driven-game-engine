#include "pch.h"
#include "ActionCreateAction.h"
#include "ActionList.h"
#include "Entity.h"
#include "Sector.h"
#include "World.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionCreateAction)

	const std::string ActionCreateAction::PrototypeNameKey = "Prototype";
	const std::string ActionCreateAction::ActionNameKey = "ActionName";

	ActionCreateAction::ActionCreateAction() : ActionCreateAction(std::string())
	{
	}

	ActionCreateAction::ActionCreateAction(const std::string& name) : Action(TypeIdClass(), name)
	{
	}

	void ActionCreateAction::SetClassName(const std::string& className)
	{
		mClassName = className;
	}

	void ActionCreateAction::SetInstanceName(const std::string& instanceName)
	{
		mInstanceName = instanceName;
	}

	gsl::owner<Scope*> ActionCreateAction::Clone() const
	{
		return new ActionCreateAction(*this);
	}

	void ActionCreateAction::Update(WorldState& worldState)
	{
#ifdef NDEBUG
		worldState;
#else 
		assert(worldState.World != nullptr); assert(worldState.Entity != nullptr); 
#endif

		Action* action = Factory<Action>::Create(mClassName);
		action->SetName(mInstanceName);
		if (GetParent()->Is(ActionList::TypeIdClass()) 
			|| GetParent()->Is(Entity::TypeIdClass()) 
			|| GetParent()->Is(Sector::TypeIdClass()) 
			|| GetParent()->Is(World::TypeIdClass())) 
		{ 
			GetParent()->Adopt(*action, "Actions"); 
		}
	}

	Vector<Signature> ActionCreateAction::Signatures()
	{
		Vector<Signature> signatures = Action::Signatures();
		signatures.PushBack({ PrototypeNameKey, Datum::DatumType::STRING, 1, offsetof(ActionCreateAction, mClassName) });
		signatures.PushBack({ ActionNameKey, Datum::DatumType::STRING, 1, offsetof(ActionCreateAction, mInstanceName) });
		return signatures;
	}
}