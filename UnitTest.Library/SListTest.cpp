#include "pch.h"
#include "CppUnitTest.h"
#include "SList.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;

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
	TEST_CLASS(SListTests)
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
			SList<Foo> list;
			Assert::IsTrue(list.Size() == 0);
			Assert::IsTrue(list.IsEmpty());

			auto expression = [&list] {	list.Front(); };
			Assert::ExpectException<std::runtime_error>(expression);
			
			auto expression2 = [&list] { list.Back(); };
			Assert::ExpectException<std::runtime_error>(expression2);
		}

		TEST_METHOD(MoveAssignment)
		{
			SList<Foo> list;

			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);

			SList<Foo> copyList(std::move(list));
			Assert::AreEqual<size_t>(3, copyList.Size());
			Assert::AreEqual(a, copyList.Front());
			Assert::AreEqual(c, copyList.Back());
		}

		TEST_METHOD(MoveConstructor)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			SList<Foo> list;
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			SList<Foo> copyList = std::move(list);
			Assert::AreEqual<size_t>(3, copyList.Size());
			Assert::AreEqual(a, copyList.Front());
			Assert::AreEqual(c, copyList.Back());
		}

		TEST_METHOD(Front)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			SList<Foo> list;

			auto expression = [&list] {	list.Front(); };
			Assert::ExpectException<std::runtime_error>(expression);

			list.PushFront(a);
			list.PushFront(b);
			Assert::AreEqual(b, list.Front());
			list.PushFront(c);
			Assert::AreEqual(c, list.Front());
		}

		TEST_METHOD(ConstFront)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			SList<Foo> list;

			const SList<Foo>& constList = list;

			auto expression = [&constList] {constList.Front(); };
			Assert::ExpectException<std::runtime_error>(expression);

			list.PushFront(a);
			list.PushFront(b);
			Assert::AreEqual(b, constList.Front());
			list.PushFront(c);
			Assert::AreEqual(c, constList.Front());
		}

		TEST_METHOD(Back)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			SList<Foo> list;

			auto expression = [&list] { list.Back(); };
			Assert::ExpectException<std::runtime_error>(expression);

			list.PushFront(a);
			list.PushFront(b);
			Assert::AreEqual(a, list.Back());
			list.PushBack(c);
			Assert::AreEqual(c, list.Back());
		}

		TEST_METHOD(ConstBack)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			SList<Foo> list;

			const SList<Foo>& constList = list;

			auto expression = [&constList] { constList.Back(); };
			Assert::ExpectException<std::runtime_error>(expression);

			list.PushFront(a);
			list.PushFront(b);
			Assert::AreEqual(a, constList.Back());
			list.PushBack(c);
			Assert::AreEqual(c, constList.Back());
		}

		TEST_METHOD(PopBack)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);
			const Foo d(40);

			SList<Foo> list;
			
			list.PushBack(a);
			list.PopBack();
			Assert::IsTrue(list.IsEmpty());
			list.PushBack(b);
			list.PushBack(c);
			list.PushBack(d);
			list.PopBack();
			list.PopBack();
			Assert::AreEqual(b, list.Front());
			Assert::AreEqual(b, list.Back());
		}

		TEST_METHOD(PopFront)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			SList<Foo> list;

			list.PushFront(a);
			list.PopFront();
			Assert::IsTrue(list.IsEmpty());
			list.PushFront(b);
			list.PushFront(c);
			list.PopFront();
			Assert::AreEqual(b, list.Front());
			Assert::AreEqual(b, list.Back());
		}

		TEST_METHOD(IsEmpty)
		{
			const Foo a(10);

			SList<Foo> list;

			Assert::IsTrue(list.IsEmpty());
			list.PushBack(a);
			Assert::IsFalse(list.IsEmpty());
		}

		TEST_METHOD(Copy)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			SList<Foo> list;
			
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);

			SList<Foo> listCopy(list);

			Assert::AreEqual<size_t>(3, listCopy.Size());
			Assert::AreEqual(a, listCopy.Front());
			listCopy.PopFront();
			Assert::AreEqual(b, listCopy.Front());
			listCopy.PopFront();
			Assert::AreEqual(c, listCopy.Front());
		}

		TEST_METHOD(Clear)
		{
			const Foo a(10);

			SList<Foo> list;

			list.PushBack(a);
			Assert::IsTrue(!list.IsEmpty());
			list.Clear();
			Assert::IsTrue(list.IsEmpty());
		}

		TEST_METHOD(Size)
		{
			const Foo a(10);
			const Foo b(20);

			SList<Foo> list;

			Assert::AreEqual<size_t>(0, list.Size());
			list.PushFront(a);
			Assert::AreEqual<size_t>(1, list.Size());
			list.PushFront(b);
			Assert::AreEqual<size_t>(2, list.Size());
			list.PopFront();
			Assert::AreEqual<size_t>(1, list.Size());
			list.PopFront();
			Assert::AreEqual<size_t>(0, list.Size());
			list.PushBack(b);
			Assert::AreEqual<size_t>(1, list.Size());
			list.PopBack();
			Assert::AreEqual<size_t>(0, list.Size());
		}

		TEST_METHOD(PushFront)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			SList<Foo> list;

			list.PushFront(a);
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(a, list.Back());
			list.PushFront(b);
			Assert::AreEqual(b, list.Front());
			list.PushFront(c);
			Assert::AreEqual(c, list.Front());
		}

		TEST_METHOD(PushBack)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			SList<Foo> list;

			list.PushBack(a);
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(a, list.Back());
			list.PushBack(b);
			Assert::AreEqual(b, list.Back());
			list.PushBack(c);
			Assert::AreEqual(c, list.Back());
		}

		TEST_METHOD(IteratorBegin)
		{
			const Foo a(10);
			const Foo b(20);

			SList<Foo> list;

			list.PushFront(a);
			SList<Foo>::Iterator iter = list.begin();
			Assert::AreEqual(a, *iter);
			list.PushFront(b);
			SList<Foo>::Iterator iter2 = list.begin();
			Assert::AreEqual(b, *iter2);
		}

		TEST_METHOD(ConstIteratorBegin)
		{

		}

		TEST_METHOD(IteratorEnd)
		{
			
		}

		TEST_METHOD(ConstIteratorEnd)
		{

		}

		TEST_METHOD(CBegin)
		{

		}

		TEST_METHOD(CEnd)
		{

		}

		TEST_METHOD(InsertAfter)
		{
			const Foo a(10);
			const Foo b(20);

			SList<Foo> list;
			list.PushFront(a);

			SList<Foo>::Iterator iter = list.begin();
			list.InsertAfter(b, iter);
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(b, list.Back());
		}

		TEST_METHOD(Find)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			SList<Foo> list;
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);

			SList<Foo>::Iterator iter = list.Find(b);
			Assert::AreEqual(b, *iter);
		}

		TEST_METHOD(Remove)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			SList<Foo> list;

			list.Remove(a);
			Assert::IsTrue(list.IsEmpty());

			list.PushFront(a);
			list.Remove(a);
			Assert::IsTrue(list.IsEmpty());

			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			list.Remove(b);

			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(c, list.Back());
			Assert::AreEqual<size_t>(2, list.Size());
		}

		private:
			static _CrtMemState sStartMemState;
	};
	_CrtMemState SListTests::sStartMemState;
}