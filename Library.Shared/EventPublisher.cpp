#include "pch.h"
#include "EventPublisher.h"
#include "EventSubscriber.h"

namespace Library
{
	RTTI_DEFINITIONS(EventPublisher)

	EventPublisher::EventPublisher(const SubscriberList& subscribers, std::mutex& mutex) : mSubscribers(&subscribers), mMutex(&mutex)
	{
	}

	void EventPublisher::Deliver() const
	{
		std::vector<std::future<void>> futures;
		{
			std::lock_guard<std::mutex> lock(*mMutex);
			for (auto subscriber : *mSubscribers)
			{
				futures.emplace_back(std::async(std::launch::async, [subscriber, this]() {subscriber->Notify(*this); }));
			}
		}

		for (auto& future : futures)
		{
			try
			{
				future.get();
			}
			catch (...)
			{
				throw;
			}
		}
	}
}