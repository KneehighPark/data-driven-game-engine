#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace std;
using namespace std::string_literals;
using namespace UnitTests;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<ActionList>(const ActionList& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	std::wstring ToString<ActionListIf>(const ActionListIf& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	std::wstring ToString<ActionIncrement>(const ActionIncrement& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	std::wstring ToString<ActionCreateAction>(const ActionCreateAction& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	std::wstring ToString<ActionDestroyAction>(const ActionDestroyAction& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	std::wstring ToString<Entity>(Entity* t)
	{
		RETURN_WIDE_STRING(t->ToString().c_str());
	}

}

namespace UnitTests
{
	TEST_CLASS(ActionTests)
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

		TEST_METHOD(Constructors)
		{
			ActionIncrement actionIncrement;
			Assert::IsTrue(actionIncrement.IsPrescribedAttribute("Name"));

			ActionList actionList;
			Assert::IsTrue(actionList.IsPrescribedAttribute("Name"));

			ActionListIf actionListIf;
			Assert::IsTrue(actionListIf.IsPrescribedAttribute("Name"));
			Assert::IsTrue(actionListIf.IsPrescribedAttribute("Condition"));
			Assert::IsTrue(actionListIf.IsPrescribedAttribute("Then"));
			Assert::IsTrue(actionListIf.IsPrescribedAttribute("Else"));

			ActionCreateAction actionCreate;
			Assert::IsTrue(actionCreate.IsPrescribedAttribute("Name"));
			Assert::IsTrue(actionCreate.IsPrescribedAttribute("Prototype"));
			Assert::IsTrue(actionCreate.IsPrescribedAttribute("ActionName"));

			ActionDestroyAction actionDestroy;
			Assert::IsTrue(actionDestroy.IsPrescribedAttribute("Name"));
			Assert::IsTrue(actionDestroy.IsPrescribedAttribute("ActionInstanceName"));
		}

		TEST_METHOD(Names)
		{
			{
				ActionList test;
				Assert::IsTrue(test.Name().empty());
				test.SetName("Testing"s);
				Assert::AreEqual(test.Name(), "Testing"s);
			}

			{
				ActionListIf test;
				Assert::IsTrue(test.Name().empty());
				test.SetName("Testing"s);
				Assert::AreEqual(test.Name(), "Testing"s);
			}

			{
				ActionIncrement test;
				Assert::IsTrue(test.Name().empty());
				test.SetName("Testing"s);
				Assert::AreEqual(test.Name(), "Testing"s);
			}
		}

		TEST_METHOD(Clones)
		{
			{
				ActionList actionList("actionList");
				auto clone = actionList.Clone();
				ActionList* actionListClone = clone->As<ActionList>();
				Assert::IsNotNull(actionListClone);
				Assert::AreNotSame(actionList, *actionListClone);
				delete clone;
			}

			{
				ActionListIf actionListIf("actionListIf");
				auto clone = actionListIf.Clone();
				ActionListIf* actionListIfClone = clone->As<ActionListIf>();
				Assert::IsNotNull(actionListIfClone);
				Assert::AreNotSame(actionListIf, *actionListIfClone);
				delete clone;
			}

			{
				ActionIncrement actionIncrement;
				auto clone = actionIncrement.Clone();
				ActionIncrement* actionIncrementClone = clone->As<ActionIncrement>();
				Assert::IsNotNull(actionIncrementClone);
				Assert::AreNotSame(actionIncrement, *actionIncrementClone);
				delete clone;
			}

			{
				ActionCreateAction actionCreateAction("ACA");
				auto clone = actionCreateAction.Clone();
				ActionCreateAction* actionCreateActionClone = clone->As<ActionCreateAction>();
				Assert::IsNotNull(actionCreateActionClone);
				Assert::AreNotSame(actionCreateAction, *actionCreateActionClone);
				delete clone;
			}

			{
				ActionDestroyAction actionDestroyAction("ADA");
				auto clone = actionDestroyAction.Clone();
				ActionDestroyAction* actionDestroyActionClone = clone->As<ActionDestroyAction>();
				Assert::IsNotNull(actionDestroyActionClone);
				Assert::AreNotSame(actionDestroyAction, *actionDestroyActionClone);
				delete clone;
			}
		}

		TEST_METHOD(EntityGetSet)
		{
			EntityFactory entityFactory;
			World world("TestWorld");
			Entity* entity = world.CreateSector("TestSector")->CreateEntity("Entity", "TestEntity");
			ActionIncrement* actionIncrement = new ActionIncrement();
			actionIncrement->SetEntity(entity);
			Assert::IsNotNull(actionIncrement);
			Assert::AreEqual(entity, actionIncrement->GetEntity()->As<Entity>());
		}

		TEST_METHOD(Updates)
		{
			GameTime gameTime;
			WorldState worldState;
			worldState.SetGameTime(gameTime);
			
			{
				// ActionIncrement Update 
				ActionIncrement actionIncrement;
				actionIncrement["TestInteger"] = 10;
				actionIncrement["Target"].Set("TestInteger");
				Assert::AreEqual(actionIncrement["TestInteger"].Get<int32_t>(), 10);
				actionIncrement.Update(worldState);
				Assert::AreEqual(actionIncrement["TestInteger"].Get<int32_t>(), 11);
			}

			{
				// ActionList Update 
				ActionList actionList;
				ActionIncrement* firstActionIncrement = new ActionIncrement();
				ActionIncrement* secondActionIncrement = new ActionIncrement();

				(*firstActionIncrement)["TestInteger"] = 10;
				(*secondActionIncrement)["TestInteger"] = 20;
				(*firstActionIncrement)["Target"].Set("TestInteger");
				(*secondActionIncrement)["Target"].Set("TestInteger");
				actionList["TestInteger"] = 10;
				actionList.Adopt(*firstActionIncrement, "Actions");
				actionList.Adopt(*secondActionIncrement, "Actions");

				Assert::AreEqual((*firstActionIncrement)["TestInteger"].Get<int32_t>(), 10);
				Assert::AreEqual(actionList["TestInteger"].Get<int32_t>(), 10);
				actionList.Update(worldState);
				Assert::AreEqual(actionList["TestInteger"].Get<int32_t>(), 10);
				Assert::AreEqual((*firstActionIncrement)["TestInteger"].Get<int32_t>(), 11);
				Assert::AreEqual((*secondActionIncrement)["TestInteger"].Get<int32_t>(), 21);
				Assert::AreEqual<size_t>(actionList.Actions().Size(), 2);
			}

			
		}

		



	private:
		static _CrtMemState s_start_mem_state;
	};
	_CrtMemState ActionTests::s_start_mem_state;
}
