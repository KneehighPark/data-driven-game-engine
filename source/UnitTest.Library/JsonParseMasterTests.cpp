#include "pch.h"
#include <json/json.h>
#include "JsonParseHelper.h"
#include "CppUnitTest.h"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<Library::JsonParseHelper::SharedData>(Library::JsonParseHelper::SharedData* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	std::wstring ToString<Library::JsonParseHelper>(Library::JsonParseHelper* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	std::wstring ToString<Library::IJsonParseHelper>(Library::IJsonParseHelper* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	std::wstring ToString<Library::JsonParseMaster>(Library::JsonParseMaster* t)
	{
		RETURN_WIDE_STRING(t);
	}
}

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(JsonParseMasterTests)
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

		TEST_METHOD(JsonParseMasterConstructor)
		{
			JsonParseHelper::SharedData sharedData; 
			JsonParseHelper parseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(parseHelper);
			Assert::AreEqual(&sharedData, parseMaster.GetSharedData()->As<JsonParseHelper::SharedData>());
		}

		TEST_METHOD(MoveSemantics)
		{
			JsonParseHelper::SharedData sharedData;
			JsonParseHelper parseHelper;
			{
				// Move Constructor 
				JsonParseMaster parseMaster(sharedData);
				parseMaster.AddHelper(parseHelper);
				JsonParseMaster anotherParseMaster(std::move(parseMaster));
				Assert::IsFalse(anotherParseMaster.IsClone());
				Assert::AreEqual<size_t>(1, anotherParseMaster.GetParseHelpers().Size());
				Assert::AreEqual(&sharedData, anotherParseMaster.GetSharedData()->As<JsonParseHelper::SharedData>());
				Assert::AreEqual(&anotherParseMaster, sharedData.GetJsonParseMaster());
			}
			{
				// Move Assignment 
				JsonParseMaster parseMaster(sharedData);
				parseMaster.AddHelper(parseHelper);
				JsonParseHelper::SharedData another_sharedData;
				JsonParseMaster anotherParseMaster(another_sharedData);
				anotherParseMaster = std::move(parseMaster);
				Assert::IsFalse(anotherParseMaster.IsClone());
				Assert::AreEqual<size_t>(1, anotherParseMaster.GetParseHelpers().Size());
				Assert::AreEqual(&sharedData, anotherParseMaster.GetSharedData()->As<JsonParseHelper::SharedData>());
				Assert::AreEqual(&anotherParseMaster, sharedData.GetJsonParseMaster());

			}
		}

		TEST_METHOD(JsonParseMasterClone)
		{
			JsonParseHelper::SharedData sharedData;
			JsonParseHelper parseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(parseHelper);
			Assert::IsFalse(parseMaster.IsClone());
			auto clone = parseMaster.Clone();
			Assert::AreNotEqual(&parseMaster, clone);
			Assert::IsTrue(clone->IsClone());
			delete clone;
		}

		TEST_METHOD(JsonParseHelperRTTI)
		{
			JsonParseHelper helper;
			RTTI* rtti = &helper;
			Assert::IsTrue(rtti->Is("IJsonParseHelper"));
			Assert::IsTrue(rtti->Is(JsonParseHelper::TypeIdClass()));
			Assert::IsTrue(rtti->TypeIdInstance());

			IJsonParseHelper* ijson = rtti->As<IJsonParseHelper>();
			Assert::IsNotNull(ijson);
			Assert::AreEqual(static_cast<IJsonParseHelper*>(&helper), ijson);

			JsonParseHelper* helperRTTI = rtti->As<JsonParseHelper>();
			Assert::IsNotNull(helperRTTI);
			Assert::AreEqual(&helper, helperRTTI);

			RTTI* rttiPointer = rtti->QueryInterface(Attributed::TypeIdClass());
			Assert::IsTrue(rttiPointer == nullptr);
			rttiPointer = rtti->QueryInterface(JsonParseHelper::TypeIdClass());
			Assert::IsTrue(rttiPointer != nullptr);
		}

		TEST_METHOD(SharedDataRTTI)
		{
			JsonParseHelper::SharedData sharedData;
			RTTI* rtti = &sharedData;
			Assert::IsTrue(rtti->Is("SharedData"));
			Assert::IsTrue(rtti->Is(JsonParseHelper::SharedData::TypeIdClass()));
			Assert::IsTrue(rtti->TypeIdInstance());

			JsonParseHelper::SharedData* sharedDataRTTI = rtti->As<JsonParseHelper::SharedData>();
			Assert::IsNotNull(sharedDataRTTI);
			Assert::AreEqual(&sharedData, sharedDataRTTI);

			RTTI* rttiPointer = rtti->QueryInterface(Attributed::TypeIdClass());
			Assert::IsTrue(rttiPointer == nullptr);
			rttiPointer = rtti->QueryInterface(JsonParseHelper::SharedData::TypeIdClass());
			Assert::IsTrue(rttiPointer != nullptr);
		}

		TEST_METHOD(ParseString)
		{
			const string input = R"({ "integer": { "name": "Health", "value": 100 } })";
			JsonParseHelper::SharedData sharedData;
			JsonParseHelper parseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(parseHelper);
			parseMaster.Parse(input);
			Assert::AreEqual<size_t>(3, parseHelper.startHandlers);
			Assert::AreEqual<size_t>(3, parseHelper.endHandlers);
			Assert::AreEqual<size_t>(0, sharedData.Depth());
			Assert::AreEqual<size_t>(2, sharedData.maxDepth);
		}

		TEST_METHOD(ParseArray)
		{
			const string input = R"({"integer": {"numbers": [100, 200, 300] } })";
			JsonParseHelper::SharedData sharedData;
			JsonParseHelper parseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(parseHelper);
			parseMaster.Parse(input);
			Assert::AreEqual<size_t>(4, parseHelper.startHandlers);
			Assert::AreEqual<size_t>(4, parseHelper.endHandlers);
			Assert::AreEqual<size_t>(0, sharedData.Depth());
			Assert::AreEqual<size_t>(2, sharedData.maxDepth);
		}

		TEST_METHOD(ParseStream)
		{
			stringstream input;
			input << R"({ "integer": { "name": "Health", "value": 100 } })"s;
			JsonParseHelper::SharedData sharedData;
			JsonParseHelper parseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(parseHelper);
			parseMaster.Parse(input);
			Assert::AreEqual<size_t>(3, parseHelper.startHandlers);
			Assert::AreEqual<size_t>(3, parseHelper.endHandlers);
			Assert::AreEqual<size_t>(0, sharedData.Depth());
			Assert::AreEqual<size_t>(2, sharedData.maxDepth);
		}

		/*TEST_METHOD(ParseFile)
		{
			const std::string name = R"(Content\TestData.json)";
			const std::string string = R"({ "integer": { "name": "Health", "value": 100 } })";
			std::ifstream jsonFile(name);
			Assert::IsFalse(jsonFile.bad());
			jsonFile.close();
			JsonParseHelper::SharedData sharedData;
			JsonParseHelper parseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(parseHelper);
			parseMaster.ParseFromFile(name);
			Assert::AreEqual(name, parseMaster.GetFileName());
			Assert::AreEqual<size_t>(3, parseHelper.startHandlers);
			Assert::AreEqual<size_t>(3, parseHelper.endHandlers);
			Assert::AreEqual<size_t>(0, sharedData.Depth());
			Assert::AreEqual<size_t>(2, sharedData.maxDepth);
		}*/

		TEST_METHOD(AddRemoveHelpers)
		{
			JsonParseHelper::SharedData sharedData;
			JsonParseHelper parseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(parseHelper);
			Assert::AreEqual<size_t>(1, parseMaster.GetParseHelpers().Size());
			parseMaster.RemoveHelper(parseHelper);
			Assert::AreEqual<size_t>(0, parseMaster.GetParseHelpers().Size());
		}

		
	private:
		static _CrtMemState s_start_mem_state;
	};
	_CrtMemState JsonParseMasterTests::s_start_mem_state;
}