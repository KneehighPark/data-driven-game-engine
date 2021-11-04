#include "pch.h"
#include "ReactionAttributed.h"
#include "EventMessageAttributed.h"
#include "Event.h"
#include "World.h"

namespace Library
{
	RTTI_DEFINITIONS(ReactionAttributed)

	const std::string ReactionAttributed::SubtypeKey = "Subtype";
	const std::size_t ReactionAttributed::SubtypeIndex = 3;

	Vector<Signature> ReactionAttributed::Signatures()
	{
		return {{ SubtypeKey, Datum::DatumType::STRING, 1, offsetof(ReactionAttributed, mSubtype) }};
	}

	ReactionAttributed::ReactionAttributed(const std::string& subtype, const std::string& name) : Reaction(TypeIdClass(), name), mSubtype(subtype)
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::ReactionAttributed(const ReactionAttributed& rhs) : Reaction(rhs), mSubtype(rhs.mSubtype)
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::ReactionAttributed(ReactionAttributed&& rhs) : Reaction(std::move(rhs)), mSubtype(std::move(mSubtype)) 
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::~ReactionAttributed()
	{
		Event<EventMessageAttributed>::Unsubscribe(*this);
	}

	gsl::owner<ReactionAttributed*> ReactionAttributed::Clone() const
	{
		return new ReactionAttributed(*this);
	}

	std::string& ReactionAttributed::Subtype()
	{
		return mSubtype;
	}

	const std::string& ReactionAttributed::Subtype() const
	{
		return mSubtype;
	}

	void ReactionAttributed::SetSubtype(const std::string& subtype)
	{
		mSubtype = subtype;
	}

	void ReactionAttributed::Notify(const EventPublisher& event)
	{
		assert(event.Is(Event<EventMessageAttributed>::TypeIdClass()));
		const Event<EventMessageAttributed>* actualEvent = static_cast<const Event<EventMessageAttributed>*>(&event);
		const EventMessageAttributed& payload = actualEvent->Message();
		
		auto attributes = payload.GetAttributes();
		for (auto& attribute : attributes)
		{
			if (!payload.IsPrescribedAttribute(attribute->first))
			{
				auto& datum = Append(attribute->first);
				datum = attribute->second;
			}
		}
		if (payload.Subtype() == mSubtype)
		{
			Update(payload.GetWorld()->GetWorldState());
		}
	}
}