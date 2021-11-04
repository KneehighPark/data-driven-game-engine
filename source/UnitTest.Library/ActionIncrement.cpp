
#include "pch.h"
#include "ActionIncrement.h"

using namespace Library;

namespace UnitTests
{
	RTTI_DEFINITIONS(ActionIncrement)

	ActionIncrement::ActionIncrement() : Action(TypeIdClass(), std::string())
	{
	}

	void ActionIncrement::Update(WorldState& /*worldState*/)
	{
		mReference = Search(mTarget);

		if (mReference == nullptr)
		{
			throw std::runtime_error("Target was not found!");
		}

		if (mReference->Type() == Datum::DatumType::INTEGER)
		{
			mReference->Datum::Get<int32_t>()++;
		}
	}

	Vector<Signature> ActionIncrement::Signatures()
	{
		Vector<Signature> signatures = Action::Signatures();
		signatures.PushBack({ "Target", Datum::DatumType::STRING, 1, offsetof(ActionIncrement, mTarget) });
		return signatures;
	}

	gsl::owner<Scope*> ActionIncrement::Clone() const
	{
		return new ActionIncrement(*this);
	}

}