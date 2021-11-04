#pragma once

#include "EventPublisher.h"
#include "GameTime.h"

using TimePoint = std::chrono::high_resolution_clock::time_point;
using Milliseconds = std::chrono::milliseconds;

namespace Library
{
	/// <summary>
	/// Queue of events
	/// </summary>
	class EventQueue final
	{	
	private:
	
		

		/// <summary>
		/// Internal data type that represents an entry in the queue
		/// </summary>
		struct QueueEntry final
		{
			/// <summary>
			/// Constructor
			/// </summary>
			/// <param name="event">Event pointer</param>
			/// <param name="timeEnqueued">Time an event was enqueued</param>
			/// <param name="delay">Amount of time after enqueued that event expires</param>
			QueueEntry(std::shared_ptr<EventPublisher>&& event, const TimePoint& timeEnqueued, const Milliseconds& delay);

			/// <summary>
			///  Takes the current time and returns true if the event has expired (time enqueued + delay)
			/// </summary>
			/// <param name="currentTime">Current time</param>
			/// <returns>True if expired, false if not</returns>
			bool IsExpired(const TimePoint& currentTime) const;

			/// <summary>
			/// event
			/// </summary>
			std::shared_ptr<EventPublisher> Event;

			/// <summary>
			/// time enqueued
			/// </summary>
			TimePoint TimeEnqueued;

			/// <summary>
			/// delay
			/// </summary>
			Milliseconds Delay;
		};

	public:

		/// <summary>
		/// Constructor
		/// </summary>
		EventQueue() = default;

		/// <summary>
		/// Copy constructor (deleted)
		/// </summary>
		/// <param name=""></param>
		EventQueue(const EventQueue&) = delete;

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="rhs">R-value reference to EventQueue</param>
		EventQueue(EventQueue&& rhs) = default;

		/// <summary>
		/// Copy assignment operator (deleted)
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		EventQueue& operator=(const EventQueue&) = delete;

		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="">R-value reference to EventQueue</param>
		/// <returns>Reference to EventQueue</returns>
		EventQueue& operator=(EventQueue&&) = default;

		/// <summary>
		/// destructor
		/// </summary>
		~EventQueue() = default;

		/// <summary>
		/// Enqueues an event
		/// </summary>
		/// <param name="event">Address of an event publisher</param>
		/// <param name="gameTime">Current game time</param>
		/// <param name="delay">Delay</param>
		void EnqueueEvent(std::shared_ptr<EventPublisher> event, const GameTime& gameTime, const Milliseconds& delay = Milliseconds());

		/// <summary>
		/// Updates the delay of a pending event
		/// </summary>
		/// <param name="event">Address of an event publisher</param>
		/// <param name="delay">Delay</param>
		void UpdateEvent(const std::shared_ptr<EventPublisher>& event, const Milliseconds& delay);

		/// <summary>
		/// Removes an enqueued event without firing it off
		/// </summary>
		/// <param name="event">Address of an event publisher</param>
		bool RemoveEvent(const std::shared_ptr<EventPublisher>& event);

		/// <summary>
		/// Publishes any queued events that have expired
		/// </summary>
		/// <param name="gameTime">Game Time</param>
		void Update(const GameTime& gameTime);

		/// <summary>
		/// Clears any pending events
		/// </summary>
		void Clear();

		/// <summary>
		/// Shrinks EventQueue
		/// </summary>
		void ShrinkToFit();

		/// <summary>
		/// Determines whether the queue is empty or not 
		/// </summary>
		/// <returns>True if empty, false if not</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Retrieves the number of events in the queue
		/// </summary>
		/// <returns>Number of enqueued events</returns>
		size_t Size() const;

	private:

		/// <summary>
		/// Pending events
		/// </summary>
		Vector<QueueEntry> mPendingQueue;

		/// <summary>
		/// Internal list for expired events
		/// </summary>
		Vector<QueueEntry> mExpiredQueue;

		/// <summary>
		/// Finds an entry in a list
		/// </summary>
		/// <param name="list">List of entries</param>
		/// <param name="event">Event (Address of an event publisher)</param>
		/// <returns>An iterator at the found event</returns>
		Vector<QueueEntry>::Iterator Find(Vector<QueueEntry>& list, const std::shared_ptr<EventPublisher>& event);

		mutable std::mutex mMutex;

		Vector<std::future<void>> mNotifyList;
	};
}