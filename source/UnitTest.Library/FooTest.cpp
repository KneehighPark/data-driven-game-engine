#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;
//using namespace FieaGameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<UnitTests::Foo>(const UnitTests::Foo& t)
	{
		RETURN_WIDE_STRING(t.Data());
	}

	template<>
	inline std::wstring ToString<UnitTests::Foo>(const UnitTests::Foo* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<UnitTests::Foo>(UnitTests::Foo* t)
	{
		RETURN_WIDE_STRING(t);
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(FooTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(Constructor)
		{
			Foo a;
			Assert::AreEqual(0, a.Data());

			Foo b(10);
			Assert::AreEqual(10, b.Data());
		}

		TEST_METHOD(CopySemantics)
		{
			Foo a(10);
			Assert::AreEqual(10, a.Data());

			Foo b(a);
			Assert::AreEqual(a, b);

			Foo c;
			c = a;
			Assert::AreEqual(c, a);
		}

		TEST_METHOD(EqualityOperators)
		{
			Foo a(10);
			Foo b(a);
			Assert::AreEqual(a, b);

			Foo c;
			Assert::IsTrue(c != a); // Assert::AreNotEqual does not invoke operator!=
		}

		TEST_METHOD(SetData)
		{
			Foo a;
			Assert::AreEqual(0, a.Data());

			const int data = 10;
			a.SetData(data);
			Assert::AreEqual(data, a.Data());
		}

		TEST_METHOD(MoveSemantics)
		{
			const int data = 10;
			Foo a(data);
			Assert::AreEqual(data, a.Data());

			Foo b(std::move(a));
			Assert::AreEqual(data, b.Data());

			Foo c;
			Assert::AreEqual(0, c.Data());
			c = std::move(b);
			Assert::AreEqual(data, c.Data());
		}

		TEST_METHOD(RTTI_TEST)
		{
			//Entity a;
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState FooTest::sStartMemState;
}