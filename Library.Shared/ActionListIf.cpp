#include "pch.h"
#include "ActionListIf.h"

namespace Library
{

	RTTI_DEFINITIONS(ActionListIf)

	const std::string ActionListIf::ConditionKey = "Condition";
	const std::string ActionListIf::ThenKey = "Then";
	const std::string ActionListIf::ElseKey = "Else";
	const std::size_t ActionListIf::ThenIndex = 3;
	const std::size_t ActionListIf::ElseIndex = 4;


	ActionListIf::ActionListIf() : ActionListIf(std::string())
	{
	}

	ActionListIf::ActionListIf(const std::string& name) : ActionList(TypeIdClass(), name)
	{
	}

	void ActionListIf::Update(WorldState& worldState) 
	{ 
		Datum& datum = mCondition ? operator[](ThenIndex) : operator[](ElseIndex); 
		
		for (size_t i = 0; i < datum.Size(); ++i) 
		{ 
			Scope* actionScope = &(datum.Get<Scope>(i)); 
			assert(actionScope->Is(Action::TypeIdClass())); 
			Action* action = static_cast<Action*>(actionScope); 
			worldState.Action = action; 
			action->Update(worldState); 
			worldState.Action = nullptr; 
		} 
	}

	void ActionListIf::SetCondition(const size_t condition)
	{
		mCondition = condition;
	}

	void ActionListIf::SetIfBlock(Action& ifAction)
	{
		Datum& datum = (*this)[ThenKey];
		if (datum.Size() == 0)
		{
			Adopt(ifAction, ThenKey);
		}
		else
		{
			datum.Set(&ifAction);
		}
	}

	void ActionListIf::SetElseBlock(Action& elseAction)
	{
		Datum& datum = (*this)[ElseKey];
		if (datum.Size() == 0)
		{
			Adopt(elseAction, ElseKey);
		}
		else
		{
			datum.Set(&elseAction);
		}
	}

	Vector<Signature> ActionListIf::Signatures()
	{
		Vector<Signature> signatures = ActionList::Signatures();
		signatures.PushBack({ ConditionKey, Datum::DatumType::INTEGER, 1, offsetof(ActionListIf, mCondition) });
		signatures.PushBack({ ThenKey, Datum::DatumType::TABLE, 0, 0 });
		signatures.PushBack({ ElseKey, Datum::DatumType::TABLE, 0, 0 });
		return signatures;
	}

	gsl::owner<Scope*> ActionListIf::Clone() const
	{
		return new ActionListIf(*this);
	}

}