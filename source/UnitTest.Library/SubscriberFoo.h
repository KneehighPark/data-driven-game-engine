#pragma once
#include "EventSubscriber.h"
#include "EventPublisher.h"

namespace UnitTests
{
	class SubscriberFoo final : public Library::EventSubscriber
	{
	public:
		SubscriberFoo();
		~SubscriberFoo() = default;
		SubscriberFoo(const SubscriberFoo& rhs) = default;
		SubscriberFoo(SubscriberFoo&& rhs) = default;
		SubscriberFoo& operator=(const SubscriberFoo& rhs) = default;
		SubscriberFoo& operator=(SubscriberFoo&& rhs) = default;

		size_t Data() const;
		virtual void Notify(const Library::EventPublisher& t_event) override;
		void Subscribe();
		void SetData(const size_t value);


	private:
		size_t mData;
	};


}