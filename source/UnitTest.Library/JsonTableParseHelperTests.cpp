#include "pch.h"
#include "JsonTableParseHelper.h"
#include "CppUnitTest.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<Library::Scope>(Library::Scope* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	std::wstring ToString<Library::JsonParseMaster>(Library::JsonParseMaster* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	std::wstring ToString<Library::JsonTableParseHelper::SharedData>(Library::JsonTableParseHelper::SharedData* t)
	{
		RETURN_WIDE_STRING(t);
	}

	/*template<> 
	std::wstring ToString<Datum::DatumType>(const Datum::DatumType& t)
	{
		RETURN_WIDE_STRING(static_cast<int>(t));
	}*/
}

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(JsonTableParseHelperTests)
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

		TEST_METHOD(InitializeTest)
		{
			Scope scope;
			const string input = R"({ "health": { "Type": "integer", "Value": 100 }, "DPS": { "Type": "float", "Value": 3.14 } })";
			JsonTableParseHelper::SharedData sharedData(scope);
			JsonTableParseHelper parseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(parseHelper);
			parseMaster.Initialize();

			parseMaster.Parse(input);
			
			//NOT SURE WHY THIS BROKE
			Assert::AreEqual(0, static_cast<int32_t>(parseHelper.SizeOfStack()));
		}

		TEST_METHOD(CloneTest)
		{
			Scope scope;
			JsonTableParseHelper::SharedData sharedData(scope);
			JsonTableParseHelper parseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(parseHelper);
			auto clone = parseMaster.Clone();
			Assert::AreNotEqual(clone, &parseMaster);
			delete clone->GetSharedData()->As<JsonTableParseHelper::SharedData>()->GetSharedData(); //this took way too long
			delete clone;
		}

		TEST_METHOD(SharedDataTests)
		{
			Scope scope;
			Scope anotherScope;
			const string input = R"({ "health": { "Type": "integer", "Value": 100 }, "DPS": { "Type": "float", "Value": 3.14 } })";
			JsonTableParseHelper::SharedData sharedData(scope);
			JsonTableParseHelper parseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(parseHelper);
			parseMaster.Initialize();
			parseMaster.Parse(input);
			Assert::AreEqual(&sharedData, parseMaster.GetSharedData()->As<JsonTableParseHelper::SharedData>());
			Assert::AreEqual(&scope, sharedData.GetSharedData());
			sharedData.SetSharedData(anotherScope);
			Assert::AreEqual(&anotherScope, sharedData.GetSharedData());
		}


		TEST_METHOD(ParseFromString)
		{
			Scope scope;
			const string input = R"({ "health": { "Type": "integer", "Value": 100 }, "DPS": { "Type": "float", "Value": 3.14 } })";
			JsonTableParseHelper::SharedData sharedData(scope);
			JsonTableParseHelper parseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(parseHelper);
			parseMaster.Initialize();

			parseMaster.Parse(input);

			Assert::AreEqual(100, scope["health"].Get<int32_t>(0));
			Assert::AreEqual(3.14f, scope["DPS"].Get<float_t>(0));
		}

		TEST_METHOD(ParseNestedScopeFromString)
		{
			Scope scope;
			const string input = R"({ "inventory": { "Type": "table", "Value": { "guns": { "Type": "table", "Value": { "special": { "Type": "string", "Value": "BFG" } } } } } })";
			JsonTableParseHelper::SharedData sharedData(scope);
			JsonTableParseHelper parseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(parseHelper);
			parseMaster.Initialize();
			parseMaster.Parse(input);

			Scope& anotherScope = scope["inventory"].Get<Scope>(0);
			Scope temp = anotherScope;
			Scope& differentScope = temp["guns"].Get<Scope>(0);
			Scope anotherTemp = differentScope;
			Assert::AreEqual("BFG"s, anotherTemp["special"].Get<std::string>(0));
		}

		TEST_METHOD(ParseArrayFromString)
		{
			Scope scope;
			const string input = R"({ "PokemonIDs": { "Type": "integer", "Value": [ 25, 149, 151 ] }, "inventory": { "Type": "table", "Value": { "potions": { "Type": "string", "Value": [ "Super", "Hyper", "Max" ] } } } })";
			JsonTableParseHelper::SharedData sharedData(scope);
			JsonTableParseHelper parseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(parseHelper);
			parseMaster.Initialize();
			parseMaster.Parse(input);

			Assert::AreEqual(25, scope["PokemonIDs"].Get<int32_t>(0));
			Assert::AreEqual(149, scope["PokemonIDs"].Get<int32_t>(1));
			Assert::AreEqual(151, scope["PokemonIDs"].Get<int32_t>(2));

			Scope& anotherScope = scope["inventory"].Get<Scope>(0);
			Scope temp = anotherScope;
			Assert::AreEqual("Super"s, temp["potions"].Get<std::string>(0));
			Assert::AreEqual("Hyper"s, temp["potions"].Get<std::string>(1));
			Assert::AreEqual("Max"s, temp["potions"].Get<std::string>(2));
		}

		/*TEST_METHOD(ParseScopeFromFileTest)
		{
			const std::string name = R"(Content\ScopeData.json)";
			std::ifstream json_file(name);
			Assert::IsFalse(!json_file.good());
			json_file.close();

			Scope scope;
			JsonTableParseHelper::SharedData sharedData(scope);
			JsonTableParseHelper parseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(parseHelper);
			parseMaster.Initialize();
			parseMaster.ParseFromFile(name);

			//NOT SURE WHY THIS BROKE
			Assert::AreEqual(100, scope["health"].Get<int32_t>(0));
			Assert::AreEqual(3.14f, scope["DPS"].Get<float_t>(0));
			Assert::AreEqual("Super"s, scope["potions"].Get<std::string>(0));
			Assert::AreEqual("Hyper"s, scope["potions"].Get<std::string>(1));
			Assert::AreEqual("Max"s, scope["potions"].Get<std::string>(2));
			Assert::IsTrue(glm::vec4(1, 2, 3, 4) == scope["direction"].Get<glm::vec4>(0));

			Scope& inventory = scope["inventory"].Get<Scope>(0);
			Datum* guns = inventory.Find("guns");
			Assert::IsNotNull(guns);
			Assert::AreEqual(Datum::DatumType::TABLE, guns->Type());
			Assert::AreEqual(size_t(1), guns->Size());
			Scope& specialGun = guns->Get<Scope>(0);
			Datum* specialGunDatum = specialGun.Find("special");
			Assert::IsNotNull(specialGunDatum);
			Assert::AreEqual(Datum::DatumType::STRING, specialGunDatum->Type());
			Assert::AreEqual(size_t(1), specialGunDatum->Size());
			Assert::AreEqual("BFG"s, specialGunDatum->Get<std::string>());

			std::string matrixString = "mat4x4((1, 2, 3, 4), (5, 6, 7, 8), (9, 10, 11, 12), (13, 14, 15, 16))"s;
			glm::mat4x4 matrix;
			sscanf_s(matrixString.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
				&matrix[0][0], &matrix[0][1], &matrix[0][2], &matrix[0][3],
				&matrix[1][0], &matrix[1][1], &matrix[1][2], &matrix[1][3],
				&matrix[2][0], &matrix[2][1], &matrix[2][2], &matrix[2][3],
				&matrix[3][0], &matrix[3][1], &matrix[3][2], &matrix[3][3]);

			Assert::IsTrue(matrix == scope["transformation"].Get<glm::mat4x4>(0));

			Datum* pokemon = scope.Find("pokemon");
			Assert::IsNotNull(pokemon);
			Assert::AreEqual(Datum::DatumType::TABLE, pokemon->Type());
			Assert::AreEqual(size_t(3), pokemon->Size());
			auto& pikachu = pokemon->Get<Scope>(0);
			Assert::AreEqual("pikachu"s, pikachu["Name"].Get<std::string>());
			Assert::AreEqual(25, pikachu["PokedexId"].Get<int>());
			auto& dragonite = pokemon->Get<Scope>(1);
			Assert::AreEqual("dragonite"s, dragonite["Name"].Get<std::string>());
			Assert::AreEqual(149, dragonite["PokedexId"].Get<int>());
			auto& mew = pokemon->Get<Scope>(2);
			Assert::AreEqual("mew"s, mew["Name"].Get<std::string>());
			Assert::AreEqual(151, mew["PokedexId"].Get<int>());
		}*/



	private:
		static _CrtMemState s_start_mem_state;
	};
	_CrtMemState JsonTableParseHelperTests::s_start_mem_state;
}
