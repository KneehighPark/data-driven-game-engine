#pragma once
#include "EventSubscriber.h"
#include "GameTime.h"
#include "EventQueue.h"

namespace UnitTests
{
	class EnqueueEventSubscriber final : public Library::EventSubscriber
	{
	public:

		EnqueueEventSubscriber(Library::EventQueue& eventQueue, Library::GameTime& gameTime); 
		~EnqueueEventSubscriber() = default;
		EnqueueEventSubscriber(const EnqueueEventSubscriber& rhs) = default;
		EnqueueEventSubscriber(EnqueueEventSubscriber&& rhs) = default;
		EnqueueEventSubscriber& operator=(const EnqueueEventSubscriber& rhs) = default;
		EnqueueEventSubscriber& operator=(EnqueueEventSubscriber&& rhs) = default;

		virtual void Notify(const Library::EventPublisher& event) override;

		size_t Data() const;
		void SetData(const size_t value);

	private:

		size_t mData;
		Library::EventQueue* mEventQueue;
		Library::GameTime* mGameTime;
	};
}