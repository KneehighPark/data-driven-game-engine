#pragma once
#include "EventPublisher.h"

namespace Library
{
	/// <summary>
	/// EventSubscriber abstract base class
	/// </summary>
	class EventSubscriber
	{
	public:
		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~EventSubscriber() = default;

		/// <summary>
		/// Called by all events that this subscriber is subscribed to 
		/// </summary>
		/// <param name="">event</param>
		virtual void Notify(const EventPublisher& event) = 0;

	protected:
		/// <summary>
		/// Constructor
		/// </summary>
		EventSubscriber() = default;

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="rhs">Const reference to EventSubscriber</param>
		EventSubscriber(const EventSubscriber& rhs) = default;

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="rhs">R-value reference to EventSubscriber</param>
		EventSubscriber(EventSubscriber&& rhs) = default;

		/// <summary>
		/// Copy assignment operator
		/// </summary>
		/// <param name="rhs">Const reference to EventSubscriber</param>
		/// <returns>Reference to EventSubscriber</returns>
		EventSubscriber& operator=(const EventSubscriber& rhs) = default;

		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="rhs">R-value reference to EventSubscriber</param>
		/// <returns>Reference to EventSubscriber</returns>
		EventSubscriber& operator=(EventSubscriber&& rhs) = default;
	};
}