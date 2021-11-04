#pragma once
#include "EventSubscriber.h"
#include "EventPublisher.h"

namespace UnitTests
{
	class SubscriberBar final : public Library::EventSubscriber
	{
	public:
		SubscriberBar();
		~SubscriberBar() = default;
		SubscriberBar(const SubscriberBar& rhs) = default;
		SubscriberBar(SubscriberBar&& rhs) = default;
		SubscriberBar& operator=(const SubscriberBar& rhs) = default;
		SubscriberBar& operator=(SubscriberBar&& rhs) = default;

		size_t Data() const;
		virtual void Notify(const Library::EventPublisher& t_event) override;
		void Subscribe();
		void SetData(const size_t value);


	private:
		size_t mData;
	};


}