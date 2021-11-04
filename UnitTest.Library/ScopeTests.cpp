#include "pch.h"
#include "CppUnitTest.h"
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<UnitTests::Foo>(const UnitTests::Foo& t)
	{
		RETURN_WIDE_STRING(t.Data());
	}

	template<>
	inline std::wstring ToString<Datum::DatumType>(const Datum::DatumType& t)
	{
		RETURN_WIDE_STRING(static_cast<int>(t));
	}

	template<>
	inline std::wstring ToString<Datum>(const Datum& temp)
	{
		RETURN_WIDE_STRING(&temp);
	}
}

namespace UnitTestLibraryDesktop
{
	class Bar : public Scope
	{
		RTTI_DECLARATIONS(Bar, Scope)

	public:
		gsl::owner<Scope*> Clone() const override
		{
			return new Bar(*this);
		}
	};

	RTTI_DEFINITIONS(Bar)

	TEST_CLASS(ScopeTests)
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

		TEST_METHOD(Constructor)
		{
			Bar* child = new Bar();

			Scope sc;
			sc.Adopt(*child, "Children");

			Scope copy(sc);
			Datum& copyOfChildren = copy["Children"];
			Assert::AreEqual(size_t(1), copyOfChildren.Size());
			Scope& copyOfChild = copyOfChildren.Get<Scope>();
			Bar* copyOfBar = copyOfChild.As<Bar>();
			Assert::IsNotNull(copyOfBar);

			//Edge case testing, size cannot be 0
			auto expression = [&] {Scope zero(0); };
			Assert::ExpectException<std::runtime_error>(expression);
		}

		TEST_METHOD(CopyConstructor)
		{
			Scope pokemon;
			Datum& badges = pokemon.Append("badges");
			badges = 8;

			Scope& inventory = pokemon.AppendScope("inventory");
			Datum& potions = inventory.Append("potions");
			potions = 10;

			Scope& species = pokemon.AppendScope("species");
			Datum& caught = species.Append("caught");
			caught = 151;

			Scope pokemon_copy(pokemon);
			Assert::IsTrue(pokemon_copy == pokemon);
		}

		TEST_METHOD(CopyAssignment)
		{
			Scope pokemon;
			Datum& badges = pokemon.Append("badges");
			badges = 8;

			Scope& inventory = pokemon.AppendScope("inventory");
			Datum& potions = inventory.Append("potions");
			potions = 10;

			Scope& species = pokemon.AppendScope("species");
			Datum& caught = species.Append("caught");
			caught = 151;

			Scope another_pokemon;
			Datum& gyms = another_pokemon.Append("gyms");
			gyms = 8;

			Assert::IsTrue(pokemon != another_pokemon);
			another_pokemon = pokemon;
			Assert::IsTrue(pokemon == another_pokemon);
		}

		TEST_METHOD(Find)
		{
			Scope pokemon; 
			Assert::IsNull(pokemon.Find(""));
			Assert::IsNull(pokemon.Find("moves"));
			Datum& badges = pokemon.Append("badges");
			Assert::IsTrue(pokemon.Find("badges") == &badges);
			Assert::IsNull(pokemon.Find("moves"));
		}

		TEST_METHOD(ConstFind)
		{
			const Scope pokemon;
			Assert::IsNull(pokemon.Find(""));
			Assert::IsNull(pokemon.Find("moves"));
			Scope another_pokemon;
			const Datum& badges = another_pokemon.Append("badges");
			const Scope* another_another_pokemon = const_cast<Scope*>(&another_pokemon);
			Assert::IsTrue(another_another_pokemon->Find("badges") == &badges);
			Assert::IsNull(another_another_pokemon->Find("moves"));
		}

		TEST_METHOD(ToString)
		{
			Scope sc;
			Assert::AreEqual("Scope"s, sc.ToString());
			RTTI* pointer;
			pointer = &sc;
			Assert::AreEqual("Scope"s, pointer->ToString());
		}

		TEST_METHOD(MoveConstructor)
		{
			Scope pokemon;
			Datum& badges = pokemon.Append("badges");
			badges = 8;

			Scope& inventory = pokemon.AppendScope("inventory");
			Datum& potions = inventory.Append("potions");
			potions = 10;

			Scope& species = pokemon.AppendScope("species");
			Datum& caught = species.Append("caught");
			caught = 151;

			Scope another_pokemon(std::move(pokemon));
			Assert::IsNull(another_pokemon.GetParent());
		}

		TEST_METHOD(MoveAssignment)
		{
			Scope pokemon;
			Datum& badges = pokemon.Append("badges");
			badges = 8;

			Scope& inventory = pokemon.AppendScope("inventory");
			Datum& potions = inventory.Append("potions");
			potions = 10;

			Scope& species = pokemon.AppendScope("species");
			Datum& caught = species.Append("caught");
			caught = 151;

			Scope another_pokemon;
			another_pokemon = std::move(pokemon);
			Assert::IsNull(another_pokemon.GetParent());
		}

		TEST_METHOD(Equals)
		{
			Scope pokemon;
			Datum& badges = pokemon.Append("badges");
			badges = 8;

			Scope another_pokemon;
			Datum& another_badges = another_pokemon.Append("badges");
			another_badges = 8;

			Scope not_pokemon;
			Datum& fake_badges = not_pokemon.Append("fake_badges");
			fake_badges = 10;

			Assert::IsTrue(pokemon.Equals(&another_pokemon));
			Assert::IsFalse(pokemon.Equals(&not_pokemon));
		}

		TEST_METHOD(BracketOperatorIndex)
		{
			Scope pokemon;
			Datum& badges = pokemon.Append("badges");
			badges = 8;
			Datum& money = pokemon.Append("money");
			money = 1982.93f;

			Assert::AreEqual(pokemon[0].Type(), Datum::DatumType::INTEGER);
			Assert::AreEqual(pokemon[1].Type(), Datum::DatumType::FLOAT);

			const Scope const_pokemon = pokemon;
			Assert::AreEqual(const_pokemon[0].Type(), Datum::DatumType::INTEGER);
			Assert::AreEqual(const_pokemon[1].Type(), Datum::DatumType::FLOAT);

			auto expression = [&] {pokemon[99]; };
			Assert::ExpectException<std::runtime_error>(expression);

			auto expression2 = [&] {const_pokemon[99]; };
			Assert::ExpectException<std::runtime_error>(expression2);
		}

		TEST_METHOD(Search)
		{
			Scope pokemon;
			Datum& badges = pokemon.Append("badges");
			Scope& inventory = pokemon.AppendScope("inventory");
			Scope& pokeballs = inventory.AppendScope("pokeballs");
			Scope& special = pokeballs.AppendScope("master");

			Scope* sp;
			Assert::IsTrue(&badges == pokemon.Search("badges", &sp));
			Assert::IsTrue(&pokemon == sp);
			Datum* dp = pokemon.Search("garbage", &sp);
			Assert::IsNull(dp);

			special.Search("pokeballs"s, &sp);
			Assert::IsTrue(&inventory == sp);
		}

		TEST_METHOD(Ancestor)
		{
			Scope pokemon;
			Scope& inventory = pokemon.AppendScope("inventory");
			Scope& pokeballs = inventory.AppendScope("pokeballs");
			Scope& special = pokeballs.AppendScope("master");
			Assert::IsFalse(special.IsAncestorOf(inventory));
			Assert::IsTrue(special.IsDescendentOf(inventory));
			Assert::IsTrue(pokeballs.IsAncestorOf(special));
			Assert::IsFalse(pokeballs.IsDescendentOf(special));
			Assert::IsTrue(pokeballs.IsDescendentOf(pokemon));
		}

		TEST_METHOD(At)
		{
			Scope pokemon;
			Scope& inventory = pokemon.AppendScope("inventory");
			inventory.Append("radio");
			inventory.Append("phone");
			Scope& consumables = inventory.AppendScope("consumables");
			Scope& potions = consumables.AppendScope("potions");
			potions.Append("super");
			Datum& hyper = potions.Append("hyper");
			Datum& datum = potions.At("hyper");

			auto expression = [&] {potions.At("max"); };
			Assert::ExpectException<std::runtime_error>(expression);
			Assert::AreEqual(hyper, datum);

			const Scope const_potions = potions;
			auto expression2 = [&] {const_potions.At("max");};
			Assert::ExpectException<std::runtime_error>(expression2);
		}

		TEST_METHOD(Adopt)
		{
			//Testing edge cases
			Scope pokemon;
			Scope& inventory = pokemon.AppendScope("inventory");
			auto expression = [&] {inventory.Adopt(inventory, "inventory"); };
			Assert::ExpectException<std::runtime_error>(expression);
			auto expression2 = [&] {pokemon.Adopt(inventory, "inventory"); };
			Assert::ExpectException<std::runtime_error>(expression2);
		}

		TEST_METHOD(Append)
		{
			//Testing edge casses
			Scope pokemon;
			auto expression = [&] {pokemon.Append(""); };
			Assert::ExpectException<std::runtime_error>(expression);
		}

		TEST_METHOD(Size)
		{
			Scope pokemon;
			Assert::AreEqual<size_t>(0, pokemon.Size());
			
			Datum& badges = pokemon.Append("badges");
			badges = 8;
			Assert::AreEqual<size_t>(1, pokemon.Size());

			Scope& inventory = pokemon.AppendScope("inventory");
			Datum& potions = inventory.Append("potions");
			potions = 10;
			Assert::AreEqual<size_t>(2, pokemon.Size());
		}
		

	private:
		static _CrtMemState s_start_mem_state;
	};
	_CrtMemState ScopeTests::s_start_mem_state;
}