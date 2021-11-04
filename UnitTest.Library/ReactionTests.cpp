#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace std;
using namespace std::string_literals;
using namespace UnitTests;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<ActionEvent>(ActionEvent* t)
	{
		RETURN_WIDE_STRING(t->ToString().c_str());
	}

	template<>
	std::wstring ToString<EventMessageAttributed>(EventMessageAttributed* t)
	{
		RETURN_WIDE_STRING(t->ToString().c_str());
	}

	template<>
	std::wstring ToString<ReactionAttributed>(ReactionAttributed* t)
	{
		RETURN_WIDE_STRING(t->ToString().c_str());
	}
}

namespace UnitTests
{
	TEST_CLASS(ReactionTests)
	{
		TEST_CLASS_INITIALIZE(InitializeClass)
		{
			TypeRegistry::RegisterType(Entity::TypeIdClass(), Entity::Signatures());
			TypeRegistry::RegisterType(Sector::TypeIdClass(), Sector::Signatures());
			TypeRegistry::RegisterType(World::TypeIdClass(), World::Signatures());
			TypeRegistry::RegisterType(Action::TypeIdClass(), Action::Signatures());
			TypeRegistry::RegisterType(ActionList::TypeIdClass(), ActionList::Signatures());
			TypeRegistry::RegisterType(ActionListIf::TypeIdClass(), ActionListIf::Signatures());
			TypeRegistry::RegisterType(ActionIncrement::TypeIdClass(), ActionIncrement::Signatures());
			TypeRegistry::RegisterType(ActionCreateAction::TypeIdClass(), ActionCreateAction::Signatures());
			TypeRegistry::RegisterType(ActionDestroyAction::TypeIdClass(), ActionDestroyAction::Signatures());
			TypeRegistry::RegisterType(Reaction::TypeIdClass(), Reaction::Signatures());
			TypeRegistry::RegisterType(EventMessageAttributed::TypeIdClass(), EventMessageAttributed::Signatures());
			TypeRegistry::RegisterType(ReactionAttributed::TypeIdClass(), ReactionAttributed::Signatures());
			TypeRegistry::RegisterType(ActionEvent::TypeIdClass(), ActionEvent::Signatures());
		}

		TEST_CLASS_CLEANUP(CleanupClass)
		{
			TypeRegistry::Clear();
		}


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

		TEST_METHOD(ReactionAttributedRTTI)
		{
			ReactionAttributed reactionAttributed;
			RTTI* rtti = &reactionAttributed;
			Assert::IsFalse(rtti->Is("Bar"s));
			Assert::IsTrue(rtti->Is("Reaction"s));
			Assert::IsTrue(rtti->Is("ActionList"s));
			Assert::IsTrue(rtti->Is("Attributed"s));
			Assert::IsTrue(rtti->Is("Scope"s));
			Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));
			Assert::IsTrue(rtti->Is(ReactionAttributed::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(ReactionAttributed::TypeIdClass(), rtti->TypeIdInstance());

			Bar* b = rtti->As<Bar>();
			Assert::IsNull(b);

			ReactionAttributed* reactionAttributedPointer = rtti->As<ReactionAttributed>();
			Assert::IsNotNull(reactionAttributedPointer);
			Assert::AreEqual(&reactionAttributed, reactionAttributedPointer);
			RTTI* r = rtti->QueryInterface(Bar::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(ReactionAttributed::TypeIdClass());
			Assert::IsNotNull(r);
		}

		TEST_METHOD(ActionEventRTTI)
		{
			ActionEvent actionEvent;
			RTTI* rtti = &actionEvent;
			Assert::IsFalse(rtti->Is("Bar"s));
			Assert::IsTrue(rtti->Is("ActionEvent"s));
			Assert::IsTrue(rtti->Is("Action"s));
			Assert::IsTrue(rtti->Is("Attributed"s));
			Assert::IsTrue(rtti->Is("Scope"s));
			Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));
			Assert::IsTrue(rtti->Is(ActionEvent::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(ActionEvent::TypeIdClass(), rtti->TypeIdInstance());

			Bar* b = rtti->As<Bar>();
			Assert::IsNull(b);

			ActionEvent* actionEventPointer = rtti->As<ActionEvent>();
			Assert::IsNotNull(actionEventPointer);
			Assert::AreEqual(&actionEvent, actionEventPointer);
			RTTI* r = rtti->QueryInterface(Bar::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(ActionEvent::TypeIdClass());
			Assert::IsNotNull(r);
		}

		TEST_METHOD(EventMessageAttributedRTTI)
		{
			{
				EventMessageAttributed eventMessageAttributed;
				RTTI* rtti = &eventMessageAttributed;
				Assert::IsFalse(rtti->Is("Bar"s));
				Assert::IsTrue(rtti->Is("EventMessageAttributed"s));
				Assert::IsTrue(rtti->Is("Attributed"s));
				Assert::IsTrue(rtti->Is("Scope"s));
				Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));
				Assert::IsTrue(rtti->Is(EventMessageAttributed::TypeIdClass()));
				Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
				Assert::AreEqual(EventMessageAttributed::TypeIdClass(), rtti->TypeIdInstance());

				Bar* b = rtti->As<Bar>();
				Assert::IsNull(b);

				EventMessageAttributed* eventMessageAttributedPointer = rtti->As<EventMessageAttributed>();
				Assert::IsNotNull(eventMessageAttributedPointer);
				Assert::AreEqual(&eventMessageAttributed, eventMessageAttributedPointer);
				RTTI* r = rtti->QueryInterface(Bar::TypeIdClass());
				Assert::IsNull(r);

				r = rtti->QueryInterface(EventMessageAttributed::TypeIdClass());
				Assert::IsNotNull(r);
			}
		}

		TEST_METHOD(CreateReaction)
		{
			ReactionAttributedFactory reactionAttributedFactory;
			World testWorld("TestWorld");
			ReactionAttributed* reaction = testWorld.CreateReaction("ReactionAttributed")->As<ReactionAttributed>();
			Assert::IsTrue(reaction == &testWorld["Reactions"][0]);
		}

		TEST_METHOD(ReactionAttributedConstructor)
		{
			ReactionAttributed reactionAttributed;
			Assert::IsTrue(reactionAttributed.IsPrescribedAttribute("this"s));
			Assert::IsTrue(reactionAttributed.IsPrescribedAttribute("Subtype"s));
		}

		TEST_METHOD(ActionEventConstructor)
		{
			ActionEvent actionEvent;
			Assert::IsTrue(actionEvent.IsPrescribedAttribute("this"s));
			Assert::IsTrue(actionEvent.IsPrescribedAttribute("Subtype"s));
			Assert::IsTrue(actionEvent.IsPrescribedAttribute("Delay"s));
		}

		TEST_METHOD(EventMessageAttributedConstructor)
		{
			EventMessageAttributed eventMessageAttributed;
			Assert::IsTrue(eventMessageAttributed.IsPrescribedAttribute("this"s));
			Assert::IsTrue(eventMessageAttributed.IsPrescribedAttribute("Subtype"s));
		}

		TEST_METHOD(CloneReactionAttributed)
		{
			ReactionAttributedFactory reactionAttributedFactory;
			World testWorld("TestWorld");
			ReactionAttributed* reaction = testWorld.CreateReaction("ReactionAttributed")->As<ReactionAttributed>();
			reaction->SetSubtype("TestInteger"s);

			auto clone = reaction->Clone();
			ReactionAttributed* reactionAttributedClone = clone->As<ReactionAttributed>();
			Assert::IsNotNull(reactionAttributedClone);
			Assert::AreNotSame(reaction, reactionAttributedClone);
			Assert::AreEqual("TestInteger"s, reactionAttributedClone->Subtype());
			delete clone;
		}

		TEST_METHOD(CloneActionEvent)
		{
			ReactionAttributedFactory reactionAttributedFactory;
			ActionEventFactory actionEventFactory;
			EntityFactory entityFactory;
			World testWorld("TestWorld");
			Entity* testEntity = testWorld.CreateSector("TestSector"s)->CreateEntity("Entity"s, "TestEntity"s);
			ActionEvent* testAction = testEntity->CreateAction("ActionEvent", "TestAction")->As<ActionEvent>();

			testAction->AppendAuxiliaryAttribute("Number") = 100;
			testAction->SetSubtype("Integer"s);

			auto clone = testAction->Clone();
			ActionEvent* actionEventClone = clone->As<ActionEvent>();
			Assert::IsNotNull(actionEventClone);
			Assert::AreNotSame(testAction, actionEventClone);
			Assert::AreEqual("Integer"s, actionEventClone->Subtype());
			Assert::AreEqual(100, (*actionEventClone)["Number"].Get<int32_t>());
			delete clone;
		}

		TEST_METHOD(CloneEventMessageAttributed)
		{
			ReactionAttributedFactory reactionAttributedFactory;
			ActionEventFactory actionEventFactory;
			EntityFactory entityFactory;
			World testWorld("TestWorld");
			EventMessageAttributed eventMessageAttributed;

			eventMessageAttributed.SetSubtype("TestInteger"s);
			eventMessageAttributed.SetWorld(testWorld);

			auto clone = eventMessageAttributed.Clone();
			EventMessageAttributed* eventMessageAttributedClone = clone->As<EventMessageAttributed>();
			Assert::IsNotNull(eventMessageAttributedClone);
			Assert::AreNotSame(&eventMessageAttributed, eventMessageAttributedClone);
			Assert::AreEqual("TestInteger"s, eventMessageAttributedClone->Subtype());
			Assert::IsTrue(&testWorld == eventMessageAttributedClone->GetWorld());
			delete clone;
		}

		TEST_METHOD(FileParsing)
		{
			GameTime gameTime;
			gameTime.SetCurrentTime(std::chrono::steady_clock::now());
			WorldState worldState;
			worldState.SetGameTime(gameTime);
			World world;

			ReactionAttributedFactory reactionAttributedFactory;
			ActionEventFactory actionEventFactory;
			EntityFactory entityFactory;
			SectorFactory sectorFactory;
			ActionIncrementFactory actionIncrementFactory;

			const std::string filename = "Content\\World.json";
			std::ifstream jsonfile(filename);
			Assert::IsFalse(!jsonfile.good());
			jsonfile.close();

			JsonTableParseHelper::SharedData sharedData(world);
			JsonTableParseHelper parseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(parseHelper);
			parseMaster.Initialize();
			parseMaster.ParseFromFile(filename);

			Assert::AreEqual("World"s, world.Name());

			Sector* sector1 = (&(world["Sectors"].Get<Scope>(0)))->As<Sector>();
			Assert::AreEqual("Sector1"s, sector1->Name());
			Sector* sector2 = (&(world["Sectors"].Get<Scope>(1)))->As<Sector>();
			Assert::AreEqual("Sector2"s, sector2->Name());

			Entity* entity1 = (&((*sector1)["Entities"].Get<Scope>(0)))->As<Entity>();
			Assert::AreEqual("Entity1"s, entity1->Name());
			Entity* entity2 = (&((*sector1)["Entities"].Get<Scope>(1)))->As<Entity>();
			Assert::AreEqual("Entity2"s, entity2->Name());
			Entity* entity3 = (&((*sector2)["Entities"].Get<Scope>(0)))->As<Entity>();
			Assert::AreEqual("Entity3"s, entity3->Name());

			ActionIncrement* actionIncrement1 = (&((*entity2)["Actions"].Get<Scope>(0)))->As<ActionIncrement>();
			Assert::AreEqual("Number"s, (*actionIncrement1)["Target"].Get<std::string>(0));
			Assert::AreEqual(10, (*actionIncrement1)["Number"].Get<int32_t>(0));

			ReactionAttributed* reactionAttributed = (&((*entity2)["Actions"].Get<Scope>(1)))->As<ReactionAttributed>();
			Assert::AreEqual("integer"s, (*reactionAttributed)["SubType"].Get<std::string>(0));

			world.Update(worldState);
			ActionIncrement* actionIncrement2 = (&((*entity3)["Actions"].Get<Scope>(0)))->As<ActionIncrement>();
			Assert::AreEqual("Number"s, (*actionIncrement2)["Target"].Get<std::string>(0));
			Assert::AreEqual(21, (*actionIncrement2)["Number"].Get<int32_t>(0));
			world.Update(worldState);
			Assert::AreEqual(22, (*actionIncrement2)["Number"].Get<int32_t>(0));
		}


	private:
		static _CrtMemState s_start_mem_state;
	};
	_CrtMemState ReactionTests::s_start_mem_state;
}
