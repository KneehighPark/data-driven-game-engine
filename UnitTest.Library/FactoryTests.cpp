#include "pch.h"
#include "JsonTableParseHelper.h"
#include "CppUnitTest.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(FactoryTests)
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

		TEST_METHOD(Find)
		{
			FooFactory fooFactory;
			Assert::IsTrue(Factory<RTTI>::Find("Bar"s) == nullptr);
			Assert::IsTrue(Factory<RTTI>::Find("Foo"s) == &fooFactory);
		}

		TEST_METHOD(ClassName)
		{
			FooFactory fooFactory;
			Assert::IsFalse(fooFactory.ClassName() == "Bar"s);
			Assert::IsTrue(fooFactory.ClassName() == "Foo"s);
		}

		TEST_METHOD(Add)
		{
			Assert::IsNull(Factory<RTTI>::Find("Foo"s));
			FooFactory fooFactory;
			Assert::IsNotNull(Factory<RTTI>::Find("Foo"s));
		}

		TEST_METHOD(Remove)
		{
			Assert::IsNull(Factory<RTTI>::Find("Foo"s));
			FooFactory* fooFactory = new FooFactory();
			Assert::IsNotNull(Factory<RTTI>::Find("Foo"s));
			delete fooFactory;
			Assert::IsNull(Factory<RTTI>::Find("Foo"s));
		}

		TEST_METHOD(Size)
		{
			Assert::IsNull(Factory<RTTI>::Find("Foo"s));
			FooFactory fooFactory;
			Assert::IsNotNull(Factory<RTTI>::Find("Foo"s));
			Assert::AreEqual<size_t>(1, fooFactory.GetFactoryTableSize());
		}

		TEST_METHOD(Create)
		{
			auto expression = [&] {Factory<RTTI>::Create("Foo"s); };
			Assert::ExpectException<std::runtime_error>(expression);
			FooFactory fooFactory;
			RTTI* pointer = Factory<RTTI>::Create("Foo"s);
			Assert::IsNotNull(pointer);
			delete pointer;
		}

		TEST_METHOD(TestRTTI)
		{
			Foo a(100);
			RTTI* pointer = &a;

			Assert::IsTrue(pointer->Is("Foo"s));
			Assert::IsFalse(pointer->Is("Bar"s));
			Assert::IsTrue(pointer->Is(Foo::TypeIdClass()));
			Assert::IsTrue(pointer->Is(pointer->TypeIdInstance()));
			Assert::AreEqual(Foo::TypeIdClass(), pointer->TypeIdInstance());
		}

		
	private:
		static _CrtMemState s_start_mem_state;
	};
	_CrtMemState FactoryTests::s_start_mem_state;
}
