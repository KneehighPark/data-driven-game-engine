#pragma once

#include "RTTI.h"
#include "Vector.h"
#include <set>
#include <mutex>
#include <thread>
#include <future>

namespace Library
{
	//Forward declaration of EventSubscriber class
	class EventSubscriber;

	/// <summary>
	/// abstract base class for events to derive from
	/// </summary>
	class EventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI)

	public:
		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~EventPublisher() = default;

		/// <summary>
		/// Notifies all subscribers of this event
		/// </summary>
		void Deliver() const;

	protected:

		using SubscriberList = std::set<EventSubscriber*>;

		/// <summary>
		/// Constructor that takes in a list of subscribers
		/// </summary>
		/// <param name="subscribers">subscribers</param>
		explicit EventPublisher(const SubscriberList& subscribers, std::mutex& mutex);

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="rhs">Const reference to EventPublisher</param>
		EventPublisher(const EventPublisher&) = default;

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="rhs">R-value reference to EventPublisher</param>
		EventPublisher(EventPublisher&&) = default;

		/// <summary>
		/// Copy assignment operator
		/// </summary>
		/// <param name="rhs">Const reference to EventPublisher</param>
		/// <returns>Reference to EventPublisher</returns>
		EventPublisher& operator=(const EventPublisher&) = default;

		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="rhs">R-value reference to EventPublisher</param>
		/// <returns>Reference to EventPublisher</returns>
		EventPublisher& operator=(EventPublisher&&) = default;

	private:

		/// <summary>
		/// pointer to this event's list of subscribers
		/// </summary>
		const SubscriberList* mSubscribers;

		std::mutex* mMutex;

	};
}