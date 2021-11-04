#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace std;
using namespace std::string_literals;
using namespace UnitTests;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<Foo>(const Foo& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}
}

namespace UnitTests
{
	TEST_CLASS(EventTests)
	{
	public:
		
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&s_start_mem_state);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState end_mem_state, diff_mem_state;
			_CrtMemCheckpoint(&end_mem_state);
			if (_CrtMemDifference(&diff_mem_state, &s_start_mem_state, &end_mem_state))
			{
				_CrtMemDumpStatistics(&diff_mem_state);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(EventConstructor)
		{
			Event<Foo> event(Foo(10));
			Assert::AreEqual(event.Message(), Foo(10));

			Foo foo;
			Event<Foo> event2(foo);
			Assert::AreEqual(event2.Message(), foo);
		}

		
		TEST_METHOD(RTTITests)
		{
			Foo foo;
			Event<Foo> event(foo);
			RTTI* rtti = &event;

			Assert::IsFalse(rtti->Is("Bar"s));
			Assert::IsTrue(rtti->Is("EventPublisher"s));
			Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));
			Assert::IsTrue(rtti->Is(EventPublisher::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));

			Bar* b = rtti->As<Bar>();
			Assert::IsNull(b);


			EventPublisher* ep = rtti->As<EventPublisher>();
			Assert::IsNotNull(ep);

			RTTI* r = rtti->QueryInterface(Bar::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(EventPublisher::TypeIdClass());
			Assert::IsNotNull(r);
		}

		TEST_METHOD(SubscribeUnsubscribe)
		{
			Foo foo(20);
			SubscriberFoo sFoo1, sFoo2;
			Event<Foo> event(foo);

			Event<Foo>::Subscribe(sFoo1);
			Assert::AreEqual(10_z, sFoo1.Data());
			Assert::AreEqual(10_z, sFoo2.Data());
			
			event.Deliver();
			Assert::AreEqual(20_z, sFoo1.Data());
			Assert::AreEqual(10_z, sFoo2.Data());
			
			Event<Foo>::Unsubscribe(sFoo1);
			Assert::AreEqual(20_z, sFoo1.Data());
			Event<Foo>::Subscribe(sFoo2);
			
			event.Deliver();
			Assert::AreEqual(20_z, sFoo1.Data());
			Assert::AreEqual(20_z, sFoo2.Data());
			Event<Foo>::Unsubscribe(sFoo2);
		}

		TEST_METHOD(Unsubscribe)
		{
			Foo foo(20);
			SubscriberFoo sFoo1, sFoo2;
			Event<Foo>::Subscribe(sFoo1);
			Event<Foo>::Subscribe(sFoo2);
			Event<Foo> event(foo);

			Assert::AreEqual(10_z, sFoo1.Data());
			Assert::AreEqual(10_z, sFoo2.Data());

			event.Deliver();
			Assert::AreEqual(20_z, sFoo1.Data());
			Assert::AreEqual(20_z, sFoo2.Data());

			Event<Foo>::UnsubscribeAll();
			
			Assert::AreEqual(20_z, sFoo1.Data());
			Assert::AreEqual(20_z, sFoo2.Data());
		}

		TEST_METHOD(EventMessage)
		{
			Foo foo(20);
			SubscriberFoo subFoo;
			Event<Foo> event(foo);
			Assert::AreEqual(event.Message(), foo);
		}

		TEST_METHOD(Queue)
		{
			GameTime time;
			time.SetCurrentTime(std::chrono::high_resolution_clock::now());
			EventQueue queue;

			Assert::IsTrue(queue.IsEmpty());
			Assert::AreEqual(0_z, queue.Size());

			{
				std::shared_ptr<Event<Foo>> event = std::make_shared<Event<Foo>>(Foo(5));
				queue.EnqueueEvent(event, time);

				Assert::IsFalse(queue.IsEmpty());
				Assert::AreEqual(1_z, queue.Size());

				SubscriberFoo subFoo;
				subFoo.Subscribe();
				Assert::AreEqual(10_z, subFoo.Data());
				queue.Update(time);
				Assert::AreEqual(10_z, subFoo.Data());

				Assert::IsFalse(queue.IsEmpty());
				Assert::AreEqual(1_z, queue.Size());

				time.SetCurrentTime(time.CurrentTime() + 1s);
				Assert::AreEqual(10_z, subFoo.Data());
				queue.Update(time);
				Assert::AreEqual(5_z, subFoo.Data());

				Assert::IsTrue(queue.IsEmpty());
				Assert::AreEqual(0_z, queue.Size());
				Event<Foo>::UnsubscribeAll();
			}

			{
				std::shared_ptr<Event<Foo>> event = std::make_shared<Event<Foo>>(Foo(5));
				queue.EnqueueEvent(event, time, 5s);
				Assert::AreEqual(1_z, queue.Size());

				SubscriberFoo subFoo;
				subFoo.Subscribe();
				Assert::AreEqual(10_z, subFoo.Data());
				queue.Update(time);
				Assert::AreEqual(10_z, subFoo.Data());

				time.SetCurrentTime(time.CurrentTime() + 5s);
				queue.EnqueueEvent(event, time, 1s);
				Assert::AreEqual(1_z, queue.Size());

				Assert::AreEqual(10_z, subFoo.Data());
				queue.Update(time);
				Assert::AreEqual(10_z, subFoo.Data());

				time.SetCurrentTime(time.CurrentTime() + 1001ms);

				Assert::AreEqual(10_z, subFoo.Data());
				queue.Update(time);
				Assert::AreEqual(5_z, subFoo.Data());

				Assert::AreEqual(0_z, queue.Size());
				Event<Foo>::UnsubscribeAll();
			}

			{
				std::shared_ptr<Event<Foo>> event1 = std::make_shared<Event<Foo>>(Foo(5));
				queue.EnqueueEvent(event1, time);

				std::shared_ptr<Event<Foo>> event2 = std::make_shared<Event<Foo>>(Foo(20));
				queue.EnqueueEvent(event2, time, 5s);

				Assert::IsFalse(queue.IsEmpty());
				Assert::AreEqual(2_z, queue.Size());

				SubscriberFoo subFoo;
				subFoo.Subscribe();
				Assert::AreEqual(10_z, subFoo.Data());
				time.SetCurrentTime(time.CurrentTime() + 1ms);
				queue.Update(time);
				Assert::AreEqual(5_z, subFoo.Data());

				Assert::IsFalse(queue.IsEmpty());
				Assert::AreEqual(1_z, queue.Size());

				time.SetCurrentTime(time.CurrentTime() + 6s);

				Assert::AreEqual(5_z, subFoo.Data());
				queue.Update(time);
				Assert::AreEqual(20_z, subFoo.Data());

				Assert::IsTrue(queue.IsEmpty());
				Assert::AreEqual(0_z, queue.Size());
				Event<Foo>::UnsubscribeAll();
			}
		}

		TEST_METHOD(UpdateEvent)
		{
			GameTime time;
			time.SetCurrentTime(std::chrono::high_resolution_clock::now());
			EventQueue queue;

			{
				std::shared_ptr<Event<Foo>> event = std::make_shared<Event<Foo>>(Foo(5));
				queue.EnqueueEvent(event, time, 5s);

				SubscriberFoo subFoo;
				subFoo.Subscribe();

				Assert::AreEqual(10_z, subFoo.Data());
				queue.Update(time);
				Assert::AreEqual(10_z, subFoo.Data());

				time.SetCurrentTime(time.CurrentTime() + 6s);

				queue.UpdateEvent(event, 8s);

				Assert::AreEqual(10_z, subFoo.Data());
				queue.Update(time);
				Assert::AreEqual(10_z, subFoo.Data());

				queue.UpdateEvent(event, 4s);

				Assert::AreEqual(10_z, subFoo.Data());
				queue.Update(time);
				Assert::AreEqual(5_z, subFoo.Data());
				Event<Foo>::UnsubscribeAll();
			}
		}

		TEST_METHOD(RemoveEvent)
		{
			GameTime time;
			time.SetCurrentTime(std::chrono::high_resolution_clock::now());
			EventQueue queue;

			{
				std::shared_ptr<Event<Foo>> event = std::make_shared<Event<Foo>>(Foo(5));
				queue.EnqueueEvent(event, time, 3s);

				SubscriberFoo subFoo;
				subFoo.Subscribe();

				Assert::AreEqual(10_z, subFoo.Data());
				queue.Update(time);
				Assert::AreEqual(10_z, subFoo.Data());

				time.SetCurrentTime(time.CurrentTime() + 3001ms);
				Assert::IsTrue(queue.RemoveEvent(event));

				Assert::AreEqual(10_z, subFoo.Data());
				queue.Update(time);
				Assert::AreEqual(10_z, subFoo.Data());

				std::shared_ptr<Event<Foo>> anotherEvent = std::make_shared<Event<Foo>>(Foo(20));
				Assert::IsFalse(queue.RemoveEvent(anotherEvent));
				Event<Foo>::UnsubscribeAll();
			}
		}

		TEST_METHOD(ClearTest)
		{
			GameTime time;
			time.SetCurrentTime(std::chrono::high_resolution_clock::now());
			EventQueue queue;

			{
				std::shared_ptr<Event<Foo>> event = std::make_shared<Event<Foo>>(Foo(5));
				queue.EnqueueEvent(event, time);

				SubscriberFoo subFoo;
				subFoo.Subscribe();

				Assert::AreEqual(10_z, subFoo.Data());
				time.SetCurrentTime(time.CurrentTime() + 10ms);
				queue.Clear();
				queue.Update(time);
				Assert::AreEqual(10_z, subFoo.Data());

				Event<Foo>::UnsubscribeAll();

			}
		}

		TEST_METHOD(ShrinkTest)
		{
			GameTime time;
			time.SetCurrentTime(std::chrono::high_resolution_clock::now());
			EventQueue queue;

			{
				Assert::AreEqual(0_z, queue.Size());
				std::shared_ptr<Event<Foo>> event = std::make_shared<Event<Foo>>(Foo(5));
				queue.EnqueueEvent(event, time);
				std::shared_ptr<Event<Foo>> event2 = std::make_shared<Event<Foo>>(Foo(5));
				queue.EnqueueEvent(event2, time);
				std::shared_ptr<Event<Foo>> event3 = std::make_shared<Event<Foo>>(Foo(5));
				queue.EnqueueEvent(event3, time, 10s);
				Assert::AreEqual(3_z, queue.Size());
				
				SubscriberFoo subFoo;
				subFoo.Subscribe();
				queue.Update(time);

				time.SetCurrentTime(time.CurrentTime() + 11s);
				queue.ShrinkToFit();
				Assert::AreEqual(3_z, queue.Size());
				
				Event<Foo>::UnsubscribeAll();
			}
		}

		TEST_METHOD(AsyncSubscribeUnsubscribe)
		{
			SubscriberFoo subFoos[2000];
			SubscriberBar subBars[1000];

			for (size_t i = 0; i < 1000; ++i)
			{
				subFoos[i].SetData(i);
				Event<Foo>::Subscribe(subFoos[i]);
				subBars[i].SetData(i);
				Event<Foo>::Subscribe(subBars[i]);
			}

			for (size_t i = 1000; i < 2000; ++i)
			{
				subFoos[i].SetData(i);
				Event<Foo>::Subscribe(subFoos[i]);
			}

			Foo f(10);
			Event<Foo> fooEvent(f);
			fooEvent.Deliver();

			for (size_t i = 0; i < 1000; ++i)
			{
				Assert::AreEqual(subBars[i].Data(), 10_z);
			}
			for (size_t i = 0; i < 2000; ++i)
			{
				Assert::AreEqual(subFoos[i].Data(), 10_z);
			}

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(AsyncEnqueueUpdate)
		{
			EventQueue eventQueue;
			GameTime gameTime;
			SubscriberFoo subFoos[25];
			Vector<shared_ptr<EnqueueEventSubscriber>> enqueueEventSubscribers;
			std::shared_ptr<EnqueueEventSubscriber> enqueueEventSubscribersPointer[25];

			for (size_t i = 0; i < 25; ++i)
			{
				EnqueueEventSubscriber subscriber(eventQueue, gameTime);
				enqueueEventSubscribersPointer[i] = std::make_shared<EnqueueEventSubscriber>(subscriber);
				enqueueEventSubscribersPointer[i]->SetData(i);
				enqueueEventSubscribers.PushBack(enqueueEventSubscribersPointer[i]);
			}

			for (size_t i = 0; i < 25; ++i)
			{
				subFoos[i].SetData(i);
				Event<Foo>::Subscribe(subFoos[i]);
				Event<Foo>::Subscribe(*enqueueEventSubscribers[i]);
			}

			for (size_t i = 0; i < 25; ++i)
			{
				Foo f(20);
				std::shared_ptr<Event<Foo>> event = std::make_shared<Event<Foo>>(f);
				eventQueue.EnqueueEvent(event, gameTime, std::chrono::milliseconds(0));
			}

			gameTime.SetCurrentTime(gameTime.CurrentTime() + Milliseconds(100));
			eventQueue.Update(gameTime);

			for (size_t i = 0; i < 25; ++i)
			{
				Assert::AreEqual(enqueueEventSubscribers[i]->Data(), 20_z);
			}

			gameTime.SetCurrentTime(gameTime.CurrentTime() + Milliseconds(100));
			eventQueue.Update(gameTime);

			for (size_t i = 0; i < 25; ++i)
			{
				Assert::AreEqual(enqueueEventSubscribers[i]->Data(), 10_z);
			}

			Event<Foo>::UnsubscribeAll();
		}

	private:
		static _CrtMemState s_start_mem_state;
	};
	_CrtMemState EventTests::s_start_mem_state;
}
