#include "pch.h"
#include "EventQueue.h"
#include <algorithm>

namespace Library
{
	EventQueue::QueueEntry::QueueEntry(std::shared_ptr<EventPublisher>&& event, const TimePoint& timeEnqueued, const Milliseconds& delay) :
		Event(std::move(event)), TimeEnqueued(timeEnqueued), Delay(delay)
	{
	}

	void EventQueue::Clear()
	{
		std::lock_guard<std::mutex> lock(mMutex);
		mPendingQueue.Clear();
	}

	void EventQueue::ShrinkToFit()
	{
		std::lock_guard<std::mutex> lock(mMutex);
		mPendingQueue.ShrinkToFit();
		mExpiredQueue.ShrinkToFit();
	}

	bool EventQueue::IsEmpty() const
	{
		std::lock_guard<std::mutex> lock(mMutex);
		return mPendingQueue.IsEmpty();
	}

	size_t EventQueue::Size() const
	{
		std::lock_guard<std::mutex> lock(mMutex);
		return mPendingQueue.Size();
	}

	bool EventQueue::QueueEntry::IsExpired(const TimePoint& currentTime) const
	{
		return (TimeEnqueued + Delay < currentTime);
	}

	void EventQueue::EnqueueEvent(std::shared_ptr<EventPublisher> event, const GameTime& gameTime, const Milliseconds& delay)
	{
		std::lock_guard<std::mutex> lock(mMutex);
		const auto& iter = Find(mPendingQueue, event);

		if (iter != mPendingQueue.end())
		{
			(*iter).TimeEnqueued = gameTime.CurrentTime();
			(*iter).Delay = delay;
		}
		else
		{
			mPendingQueue.EmplaceBack(std::move(event), gameTime.CurrentTime(), delay);
		}
	}
	
	void EventQueue::UpdateEvent(const std::shared_ptr<EventPublisher>& event, const Milliseconds& delay)
	{
		std::lock_guard<std::mutex> lock(mMutex);
		const auto& iter = Find(mPendingQueue, event);

		if (iter != mPendingQueue.end())
		{
			(*iter).Delay = delay;
		}
	}

	bool EventQueue::RemoveEvent(const std::shared_ptr<EventPublisher>& event)
	{
		std::lock_guard<std::mutex> lock(mMutex);
		const auto& it = Find(mPendingQueue, event);

		if (it != mPendingQueue.end())
		{
			mPendingQueue.Remove(it);
			return true;
		}

		return false;
	}

	void EventQueue::Update(const GameTime& gameTime)
	{
		{
			std::lock_guard<std::mutex> lock(mMutex);

			//First, we move all the expired events to the back, using std::partition
			auto expiredStart = std::partition(mPendingQueue.begin(), mPendingQueue.end(),
				[&gameTime](const QueueEntry& queued) { return !queued.IsExpired(gameTime.CurrentTime()); });

			//Second, we std::move the expired events from the pending queue to the expired queue
			std::move(expiredStart, mPendingQueue.end(), std::back_inserter(mExpiredQueue));
			mPendingQueue.Remove(expiredStart, mPendingQueue.end());
		}

		mNotifyList.Reserve(mExpiredQueue.Size());
		

		//Deliver each expired event
		for (const auto& entry : mExpiredQueue)
		{
			mNotifyList.EmplaceBack(std::async(std::launch::async, &EventPublisher::Deliver, entry.Event));
		}

		for (auto& job : mNotifyList)
		{
			try
			{
				job.get();
			}
			catch (...)
			{
				mNotifyList.Clear();
				throw;
			}
		}

		//Clear the expired queue and jobs queue
		mExpiredQueue.Clear();
		mNotifyList.Clear();
	}

	Vector<EventQueue::QueueEntry>::Iterator EventQueue::Find(Vector<QueueEntry>& list, const std::shared_ptr<EventPublisher>& event)
	{
		return std::find_if(list.begin(), list.end(),
			[&event](const QueueEntry& entry) { return entry.Event == event; });
	}
	
}