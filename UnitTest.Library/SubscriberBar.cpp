#pragma once
#include "pch.h"
#include "SubscriberBar.h"
#include "Event.h"
#include "Foo.h"

using namespace Library;

namespace UnitTests
{

	SubscriberBar::SubscriberBar() : mData(10)
	{
	}

	size_t SubscriberBar::Data() const
	{
		return mData;
	}

	void SubscriberBar::SetData(size_t value)
	{
		mData = value;
	}

	void SubscriberBar::Notify(const EventPublisher& event)
	{
		auto actualEvent = event.As<Event<Foo>>();

		if (actualEvent == nullptr)
		{
			throw std::exception("Unexpected type.");
		}

		const Foo& message = actualEvent->Message();
		mData = message.Data();
	}

	void SubscriberBar::Subscribe()
	{
		Event<Foo>::Subscribe(*this);
	}

}