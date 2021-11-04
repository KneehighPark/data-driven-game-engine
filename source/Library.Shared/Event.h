#pragma once

#include "EventPublisher.h"
#include "Vector.h"
#include <mutex>

namespace Library
{
	/// <summary>
	/// Templated Event Class
	/// </summary>
	template <typename T>
	class Event final : public EventPublisher
	{

		RTTI_DECLARATIONS(Event<T>, EventPublisher)

	public:

		/// <summary>
		/// Parameterized constructor with message
		/// </summary>
		/// <param name="message">L-value reference to message</param>
		explicit Event(const T& message = T());

		/// <summary>
		/// Parameterized constructor with message
		/// </summary>
		/// <param name="message">R-value reference to message</param>
		explicit Event(T&& message);

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="rhs">Const reference to Event</param>
		Event(const Event&) = default;

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="rhs">R-value reference to Event</param>
		Event(Event&&) = default;

		/// <summary>
		/// Copy assignment operator
		/// </summary>
		/// <param name="rhs">Const reference to Event</param>
		/// <returns>Reference to Event</returns>
		Event& operator=(const Event&) = default;

		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="rhs">R-value reference to Event</param>
		/// <returns>Reference to Event</returns>
		Event& operator=(Event&&) = default;

		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~Event() = default;

		/// <summary>
		/// Given the address of an EventSubscriber, add it to the list of subscribers for this event type.
		/// </summary>
		/// <param name="subscriber">subscriber</param>
		static void Subscribe(EventSubscriber& subscriber);

		/// <summary>
		/// Given the address of an EventSubscriber, remove it from the list of subscribers for this event type.
		/// </summary>
		/// <param name="subscriber">subscriber</param>
		static void Unsubscribe(EventSubscriber& subscriber);

		/// <summary>
		/// Unsubscribe all subscribers to this event type.
		/// </summary>
		static void UnsubscribeAll();

		/// <summary>
		/// Getter method for the message
		/// </summary>
		/// <returns>Const reference to the  message</returns>
		const T& Message() const;

	private:
		/// <summary>
		/// Static list of subscribers
		/// </summary>
		static SubscriberList sSubscribers;

		/// <summary>
		/// Message that is to be delivered
		/// </summary>
		T mMessage;

		/// <summary>
		/// Mutex for the static list of subscribers
		/// </summary>
		static std::mutex mMutex;
	};
}

#include "Event.inl"