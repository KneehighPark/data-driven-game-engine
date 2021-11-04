#include "pch.h"
#include "Avatar.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<Entity>(const Entity& t_temp)
	{
		RETURN_WIDE_STRING(t_temp.ToString().c_str());
	}
	template<>
	std::wstring ToString<Sector>(const Sector& t_temp)
	{
		RETURN_WIDE_STRING(t_temp.ToString().c_str());
	}
	template<>
	std::wstring ToString<World>(const World& t_temp)
	{
		RETURN_WIDE_STRING(t_temp.ToString().c_str());
	}

	template<>
	std::wstring ToString<Library::World>(Library::World* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	std::wstring ToString<Library::Sector>(Library::Sector* t)
	{
		RETURN_WIDE_STRING(t);
	}
}

namespace UnitTests
{
	TEST_CLASS(EntitySectorWorldTests)
	{

		TEST_CLASS_INITIALIZE(InitializeClass)
		{
			TypeRegistry::RegisterType(Entity::TypeIdClass(), Entity::Signatures());
			TypeRegistry::RegisterType(Sector::TypeIdClass(), Sector::Signatures());
			TypeRegistry::RegisterType(World::TypeIdClass(), World::Signatures());
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
			{
				string name = "TestEntity";
				Entity entity(name);
				Assert::IsTrue(entity.IsPrescribedAttribute("Name"s));
				Assert::AreEqual(name, entity.Name());
			}
			{
				string name = "TestSector";
				Sector sector(name);
				Assert::IsTrue(sector.IsPrescribedAttribute("Name"s));
				Assert::AreEqual(name, sector.Name());
			}
			{
				string name = "TestWorld";
				World world(name);
				Assert::IsTrue(world.IsPrescribedAttribute("Name"s));
				Assert::AreEqual(name, world.Name());
			}
		}

		TEST_METHOD(Names)
		{
			{
				Entity entity("");
				string name = "TestEntity";
				Assert::IsTrue(entity.Name().empty());
				entity.SetName(name);
				Assert::AreEqual(name, entity.Name());
			}
			{
				Sector sector("");
				string name = "TestSector";
				Assert::IsTrue(sector.Name().empty());
				sector.SetName(name);
				Assert::AreEqual(name, sector.Name());
			}
			{
				World world("");
				string name = "TestWorld";
				Assert::IsTrue(world.Name().empty());
				world.SetName(name);
				Assert::AreEqual(name, world.Name());
			}
		}
		TEST_METHOD(Clones)
		{
			{
				Entity entity("TestEntity");
				auto clone = entity.Clone();
				Entity* entityClone = clone->As<Entity>();
				Assert::IsNotNull(entityClone);
				Assert::AreNotSame(entity, *entityClone);
				delete clone;
			}
			{
				Sector sector("TestSector");
				auto clone = sector.Clone();
				Sector* sectorClone = clone->As<Sector>();
				Assert::IsNotNull(sectorClone);
				Assert::AreNotSame(sector, *sectorClone);
				delete clone;
			}
			{
				World world("TestWorld");
				auto clone = world.Clone();
				World* worldClone = clone->As<World>();
				Assert::IsNotNull(worldClone);
				Assert::AreNotSame(world, *worldClone);
				delete clone;
			}
		}

		TEST_METHOD(SectorGetSet)
		{
			EntityFactory factory;
			Sector sector("TestSector");
			Sector sector2("TestSector2");
			Entity* entity = sector.CreateEntity("Entity", "MyAwesomeEntity");
			Assert::IsTrue(&sector == entity->GetSector());
			entity->SetSector(&sector2);
			Assert::AreEqual(&sector2, entity->GetSector());
		}

		TEST_METHOD(WorldGetSet)
		{
			World world("World");
			World world2("World2");
			Sector* sector = world.CreateSector("TestSector");
			Assert::AreEqual(&world, sector->GetWorld());
			sector->SetWorld(&world2);
			Assert::AreEqual(&world2, sector->GetWorld());
		}

		TEST_METHOD(UpdateTest)
		{
			{
				AvatarFactory avatarFactory;

				World world("TestWorld");
				Sector* shaansSector = world.CreateSector("Shaan's Sector");
				Assert::IsNotNull(shaansSector);

				Entity* entity = shaansSector->CreateEntity("Avatar", "Test Avatar");
				Assert::IsNotNull(entity);
				Avatar* avatar = entity->As<Avatar>();
				Assert::IsNotNull(avatar);

				GameTime gameTime;
				WorldState worldState;
				worldState.SetGameTime(gameTime);
				
				Assert::AreEqual(0, avatar->Health);
				world.Update(worldState);
				Assert::AreEqual(1, avatar->Health);
			}

			{
				Sector sector("TestSector");
				GameTime gameTime;
				WorldState worldState;
				worldState.SetGameTime(gameTime);
				sector.Update(worldState);
			}

			{
				Entity entity("TestEntity");
				GameTime gameTime;
				WorldState worldState;
				worldState.SetGameTime(gameTime);
				entity.Update(worldState);
			}
		}

	private:
		static _CrtMemState s_start_mem_state;
	};
	_CrtMemState EntitySectorWorldTests::s_start_mem_state;
}
