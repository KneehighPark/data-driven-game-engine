#include "pch.h"
#include "ActionList.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionList)

	const std::string ActionList::ActionsKey = "Actions";
	const std::size_t ActionList::ActionsIndex = 2;

	ActionList::ActionList() : ActionList(std::string())
	{
	}

	ActionList::ActionList(const std::string& name) : Action(TypeIdClass(), name)
	{
	}

	ActionList::ActionList(const RTTI::IdType typeID, const std::string& name) : Action(typeID, name)
	{
	}

	gsl::owner<Scope*> ActionList::Clone() const
	{
		return new ActionList(*this);;
	}

	Datum& ActionList::Actions()
	{
		return (*this)[ActionsIndex];
	}

	Vector<Signature> ActionList::Signatures()
	{
		Vector<Signature> signatures = Action::Signatures();
		signatures.PushBack({ ActionsKey, Datum::DatumType::TABLE, 0, 0 });
		return signatures;
	}

	void ActionList::Update(WorldState& worldState)
	{
		Datum& actions = Actions();
		for (size_t i = 0; i < actions.Size(); ++i)
		{
			Scope& scope = actions[i];
			assert(scope.Is(Action::TypeIdClass()));
			Action* action = static_cast<Action*>(&scope);
			worldState.Action = action;
			action->Update(worldState);
			worldState.Action = nullptr;
		}
	}
}