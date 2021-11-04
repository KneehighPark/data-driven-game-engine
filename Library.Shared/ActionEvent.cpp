#include "pch.h"
#include "ActionEvent.h"
#include "EventMessageAttributed.h"
#include "Event.h"
#include "World.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionEvent)

	const std::string ActionEvent::SubtypeKey = "Subtype";
	const std::string ActionEvent::DelayKey = "Delay";

	Vector<Signature> ActionEvent::Signatures()
	{
		return 
		{
			{ SubtypeKey, Datum::DatumType::STRING, 1, offsetof(ActionEvent, mSubtype) },
			{ DelayKey, Datum::DatumType::INTEGER, 1, offsetof(ActionEvent, mDelay) }
		};
	}

	ActionEvent::ActionEvent(const std::string& subtype, const std::string& name) : Action(TypeIdClass(), name), mSubtype(subtype)
	{
	}

	std::string& ActionEvent::Subtype()
	{
		return mSubtype;
	}

	const std::string& ActionEvent::Subtype() const
	{
		return mSubtype;
	}

	void ActionEvent::SetSubtype(const std::string& subtype)
	{
		mSubtype = subtype;
	}

	std::size_t ActionEvent::GetDelay() const
	{
		return mDelay;
	}

	void ActionEvent::SetDelay(const std::size_t delay)
	{
		mDelay = delay;
	}

	gsl::owner<ActionEvent*> ActionEvent::Clone() const
	{
		return new ActionEvent(*this);
	}

	void ActionEvent::Update(WorldState& worldState)
	{
		worldState.Action = this;
		EventMessageAttributed message(*worldState.World, mSubtype);

		auto attributes = GetAttributes();
		for (auto& attribute : attributes)
		{
			if (!IsPrescribedAttribute(attribute->first))
			{
				message.AppendAuxiliaryAttribute(attribute->first) = attribute->second;
			}
		}
		std::shared_ptr<Event<EventMessageAttributed>> event_ptr = std::make_shared<Event<EventMessageAttributed>>(message);
		worldState.World->GetEventQueue().EnqueueEvent(event_ptr, worldState.GetGameTime(), Milliseconds(mDelay));
	}
}