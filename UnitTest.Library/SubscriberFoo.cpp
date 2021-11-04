#pragma once
#include "pch.h"
#include "SubscriberFoo.h"
#include "Event.h"
#include "Foo.h"

using namespace Library;

namespace UnitTests
{

	SubscriberFoo::SubscriberFoo() : mData(10)
	{
	}

	size_t SubscriberFoo::Data() const
	{
		return mData;
	}

	void SubscriberFoo::SetData(size_t value)
	{
		mData = value;
	}

	void SubscriberFoo::Notify(const EventPublisher& event)
	{
		auto actualEvent = event.As<Event<Foo>>();

		if (actualEvent == nullptr)
		{
			throw std::exception("Unexpected type.");
		}

		const Foo& message = actualEvent->Message();
		mData = message.Data();
	}

	void SubscriberFoo::Subscribe()
	{
		Event<Foo>::Subscribe(*this);
	}

}