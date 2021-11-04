#include "pch.h"
#include "ActionDestroyAction.h"
#include "Entity.h"
#include "World.h"


namespace Library
{
	RTTI_DEFINITIONS(ActionDestroyAction)

	const std::string ActionDestroyAction::ActionInstanceKey = "ActionInstanceName";
	const std::string ActionDestroyAction::ActionsKey = "Actions";


	ActionDestroyAction::ActionDestroyAction() : ActionDestroyAction(std::string())
	{
	}

	ActionDestroyAction::ActionDestroyAction(const std::string& name) : Action(TypeIdClass(), name), mActionInstance(name)
	{
	}

	void ActionDestroyAction::Update(WorldState& worldState)
	{
		assert(worldState.World != nullptr);
		assert(worldState.Entity != nullptr);

		auto action = worldState.Entity->FindAction(mActionInstance);
		if (action != nullptr)
		{
			worldState.World->MarkForDelete(*action);
		}

		worldState.World->MarkForDelete(*this);
	}

	void ActionDestroyAction::SetActionName(const std::string& name)
	{
		mActionInstance = name;
	}

	gsl::owner<Scope*> ActionDestroyAction::Clone() const
	{
		return new ActionDestroyAction(*this);
	}

	Vector<Signature> ActionDestroyAction::Signatures()
	{
		Vector<Signature> signatures = Action::Signatures();
		signatures.PushBack({ ActionInstanceKey, Datum::DatumType::STRING, 1, offsetof(ActionDestroyAction, mActionInstance) });
		return signatures;
	}


}