#include "pch.h"
#include "EnqueueEventSubscriber.h"
#include "Foo.h"
#include "Event.h"

using namespace Library;

namespace UnitTests
{
	EnqueueEventSubscriber::EnqueueEventSubscriber(EventQueue& eventQueue, GameTime& gameTime) : mData(0), mEventQueue(&eventQueue), mGameTime(&gameTime)
	{

	}

	size_t EnqueueEventSubscriber::Data() const
	{
		return mData;
	}

	void EnqueueEventSubscriber::SetData(const size_t value)
	{
		mData = value;
	}

	void EnqueueEventSubscriber::Notify(const EventPublisher& rhs_event)
	{
		auto foo = rhs_event.As<Event<Foo>>();

		if (foo == nullptr)
		{
			throw std::exception("Unexpected type.");
		}

		Foo f(10);
		std::shared_ptr<Event<Foo>> event = std::make_shared<Event<Foo>>(f);
		mEventQueue->EnqueueEvent(event, *mGameTime, std::chrono::milliseconds(0));
		mData = foo->Message().Data();
	}


}