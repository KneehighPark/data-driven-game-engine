#include "pch.h"
#include "CppUnitTest.h"
#include "Vector.h"
#include "Foo.h"
#include <gsl/gsl>
#include <glm/glm.hpp>

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
	TEST_CLASS(VectorTests)
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
			Vector<Foo> vector;
			Assert::AreEqual<size_t>(0, vector.Size());
			Assert::AreEqual<size_t>(0, vector.Capacity());
			Assert::IsTrue(vector.IsEmpty());

			auto expression = [&vector] {vector.Front(); };
			Assert::ExpectException<std::runtime_error>(expression);

			auto expression2 = [&vector] {vector.Back(); };
			Assert::ExpectException<std::runtime_error>(expression2);

			Vector<Foo> vector2(10);
			Assert::AreEqual<size_t>(0, vector2.Size());
			Assert::AreEqual<size_t>(10, vector2.Capacity());
			Assert::IsTrue(vector2.IsEmpty());
		}

		TEST_METHOD(MoveAssignment)
		{
			Vector<Foo> vector;

			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			vector.PushBack(a);
			vector.PushBack(b);
			vector.PushBack(c);

			Vector<Foo> copy(std::move(vector));
			Assert::AreEqual<size_t>(3, copy.Size());
			Assert::AreEqual(a, copy.Front());
			Assert::AreEqual(c, copy.Back());
		}

		TEST_METHOD(MoveConstructor)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			Vector<Foo> vector;

			vector.PushBack(a);
			vector.PushBack(b);
			vector.PushBack(c);

			Vector<Foo> copy;
			copy = std::move(vector);
			Assert::AreEqual<size_t>(3, copy.Size());
			Assert::AreEqual(a, copy.Front());
			Assert::AreEqual(c, copy.Back());
		}

		TEST_METHOD(CopyConstructor)
		{
			Vector<Foo> vector;

			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			vector.PushBack(a);
			vector.PushBack(b);
			vector.PushBack(c);

			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(c, vector.Back());
			Assert::AreEqual<size_t>(3, vector.Size());

			Vector<Foo> vector2(vector);
			Assert::AreEqual(a, vector2.Front());
			Assert::AreEqual(c, vector2.Back());
			Assert::AreEqual<size_t>(3, vector2.Size());
		}

		TEST_METHOD(AssignmentOperator)
		{
			Vector<Foo> vector;
			Vector<Foo> vector2;

			const Foo a(10);
			const Foo b(20);
			const Foo c(30);
			const Foo d(40);

			vector.PushBack(a);
			vector.PushBack(b);
			vector2.PushBack(c);
			vector2.PushBack(d);

			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(b, vector.Back());
			Assert::AreEqual<size_t>(2, vector.Size());

			vector = vector2;

			Assert::AreEqual(c, vector.Front());
			Assert::AreEqual(d, vector.Back());
			Assert::AreEqual<size_t>(2, vector.Size());
		}

		TEST_METHOD(Subscript)
		{
			Vector<Foo> vector;

			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			vector.PushBack(a);
			vector.PushBack(b);
			vector.PushBack(c);

			Assert::AreEqual(a, vector[0]);
			Assert::AreEqual(b, vector[1]);
			Assert::AreEqual(c, vector[2]);

			vector[1] = c;
			Assert::AreEqual(c, vector[1]);

			auto expression = [&] {Foo d = vector[3], UNREFERENCED_PARAMETER(d); };
			Assert::ExpectException<std::exception>(expression);
		}

		TEST_METHOD(At)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			Vector<Foo> list;

			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);

			auto expression = [&] {Foo d = list.At(3), UNREFERENCED_PARAMETER(d); };
			Assert::ExpectException<std::exception>(expression);

			Assert::AreEqual(a, list.At(0));
			Assert::AreEqual(b, list.At(1));
			Assert::AreEqual(c, list.At(2));

			const Vector<Foo>& constList = list;

			auto expression2 = [&] {Foo d = constList.At(3), UNREFERENCED_PARAMETER(d); };
			Assert::ExpectException<std::exception>(expression2);

			Assert::AreEqual(a, constList.At(0));
			Assert::AreEqual(b, constList.At(1));
			Assert::AreEqual(c, constList.At(2));
		}

		TEST_METHOD(PushBack)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			Vector<Foo> list;

			list.PushBack(a);
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(a, list.Back());
			list.PushBack(b);
			Assert::AreEqual(b, list.Back());
			list.PushBack(c);
			Assert::AreEqual(c, list.Back());
		}

		TEST_METHOD(PopBack)
		{
			Vector<Foo> vector;

			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			vector.PushBack(a);
			vector.PushBack(b);
			vector.PushBack(c);

			Assert::AreEqual<size_t>(3, vector.Size());
			vector.PopBack();
			Assert::AreEqual<size_t>(2, vector.Size());
			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(b, vector.Back());
			vector.PopBack();
			Assert::AreEqual<size_t>(1, vector.Size());
			vector.PopBack();
			Assert::IsTrue(vector.IsEmpty());
		}

		TEST_METHOD(DefaultReserve)
		{
			Vector<Foo> vector;

			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			vector.PushBack(a);
			vector.PushBack(b);
			vector.PushBack(c);

			Assert::AreEqual<size_t>(5, vector.Capacity());
			vector.Reserve(10);
			Assert::AreEqual<size_t>(10, vector.Capacity());
		}

		TEST_METHOD(Clear)
		{
			const Foo a(10);

			Vector<Foo> list;

			list.PushBack(a);
			Assert::IsTrue(!list.IsEmpty());
			list.Clear();
			Assert::IsTrue(list.IsEmpty());
		}

		TEST_METHOD(Front)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			Vector<Foo> list;

			auto expression = [&list] {	list.Front(); };
			Assert::ExpectException<std::runtime_error>(expression);

			list.PushBack(a);
			list.PushBack(b);
			Assert::AreEqual(a, list.Front());
			list.PushBack(c);
			Assert::AreEqual(a, list.Front());
		}

		TEST_METHOD(ConstFront)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			Vector<Foo> list;

			const Vector<Foo>& constList = list;

			auto expression = [&constList] {constList.Front(); };
			Assert::ExpectException<std::runtime_error>(expression);

			list.PushBack(a);
			list.PushBack(b);
			Assert::AreEqual(a, constList.Front());
			list.PushBack(c);
			Assert::AreEqual(a, constList.Front());
		}

		TEST_METHOD(Back)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			Vector<Foo> list;

			auto expression = [&list] { list.Back(); };
			Assert::ExpectException<std::runtime_error>(expression);

			list.PushBack(a);
			list.PushBack(b);
			Assert::AreEqual(b, list.Back());
			list.PushBack(c);
			Assert::AreEqual(c, list.Back());
		}

		TEST_METHOD(ConstBack)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			Vector<Foo> list;

			const Vector<Foo>& constList = list;

			auto expression = [&constList] { constList.Back(); };
			Assert::ExpectException<std::runtime_error>(expression);

			list.PushBack(a);
			list.PushBack(b);
			Assert::AreEqual(b, constList.Back());
			list.PushBack(c);
			Assert::AreEqual(c, constList.Back());
		}

		TEST_METHOD(IsEmpty)
		{
			const Foo a(10);

			Vector<Foo> list;

			Assert::IsTrue(list.IsEmpty());
			list.PushBack(a);
			Assert::IsFalse(list.IsEmpty());
		}

		TEST_METHOD(Begin)
		{
			const Foo a(10);

			Vector<Foo> vector; 

			vector.PushBack(a);
			Vector<Foo>::Iterator iter = vector.begin();
			Assert::AreEqual(a, *iter);

			//add for const
		}

		TEST_METHOD(End)
		{
			const Foo a(10);

			Vector<Foo> vector;

			//add for const
		}

		TEST_METHOD(CBegin)
		{

		}

		TEST_METHOD(CEnd)
		{

		}

		TEST_METHOD(Size)
		{
			const Foo a(10);

			Vector<Foo> list;

			Assert::IsTrue(list.IsEmpty());
			Assert::AreEqual<size_t>(0, list.Size());
			list.PushBack(a);
			Assert::AreEqual<size_t>(1, list.Size());
			list.PushBack(a);
			Assert::AreEqual<size_t>(2, list.Size());
			list.PopBack();
			Assert::AreEqual<size_t>(1, list.Size());
			list.PopBack();
			Assert::AreEqual<size_t>(0, list.Size());
			Assert::IsTrue(list.IsEmpty());
		}

		TEST_METHOD(Capacity)
		{
			const Foo a(10);

			Vector<Foo> list;

			Assert::IsTrue(list.IsEmpty());
			Assert::AreEqual<size_t>(0, list.Capacity());
			list.PushBack(a);
			Assert::AreEqual<size_t>(5, list.Capacity());
			list.Reserve(10);
			Assert::AreEqual<size_t>(10, list.Capacity());
			list.Reserve(list.Capacity() * 2);
			Assert::AreEqual<size_t>(20, list.Capacity());
		}

		TEST_METHOD(Find)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);
			const Foo d(40);

			Vector<Foo> list;

			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);

			//const Vector<Foo>& constList = list;

			Vector<Foo>::Iterator iter = list.Find(b);
			Assert::IsTrue(b == *iter);
			
			iter = list.Find(d);
			Assert::IsTrue(iter == list.end());

			//Vector<Foo>::ConstIterator constIter = list.Find(b);
			//Assert::IsTrue(b == *constIter);

			//constIter = constList.Find(d);
			//Assert::IsTrue(constIter == list.end());
		}

		TEST_METHOD(Remove)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);
			const Foo d(40);
			const Foo e(50);

			Vector<Foo> list;

			Assert::AreEqual(false, list.Remove(a));
			Assert::IsTrue(list.IsEmpty());

			list.PushBack(a);
			list.Remove(a);
			Assert::IsTrue(list.IsEmpty());

			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			list.PushBack(d);
			list.PushBack(e);

			list.Remove(c);

			Assert::AreEqual<size_t>(4, list.Size());
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(e, list.Back());
		}

		TEST_METHOD(ShrinkToFit)
		{
			Vector<Foo> vector(10);

			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			vector.PushBack(a);
			vector.PushBack(b);
			vector.PushBack(c);

			Assert::AreEqual<size_t>(3, vector.Size());
			Assert::AreEqual<size_t>(10, vector.Capacity());

			vector.ShrinkToFit();
			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(c, vector.Back());
			Assert::AreEqual<size_t>(3, vector.Size());
			Assert::AreEqual<size_t>(3, vector.Capacity());
		}

		TEST_METHOD(Resize)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);
			const Foo d(40);
			const Foo e(50);

			Vector<Foo> list;

			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			list.PushBack(d);

			Assert::AreEqual<size_t>(4, list.Size());
			Assert::AreEqual<size_t>(5, list.Capacity());
			Assert::AreEqual(a, list.Front());

			list.PushBack(e);
			Assert::AreEqual<size_t>(5, list.Size());
			Assert::AreEqual<size_t>(5, list.Capacity());
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(e, list.Back());

			list.Resize(4);
			Assert::AreEqual<size_t>(4, list.Size());
			Assert::AreEqual<size_t>(4, list.Capacity());
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(d, list.Back());

			list.Resize(8);
			Assert::AreEqual<size_t>(8, list.Size());
			Assert::AreEqual<size_t>(8, list.Capacity());
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(Foo(0), list.Back());
		}

	private:
		static _CrtMemState sStartMemState;
	};
	_CrtMemState VectorTests::sStartMemState;
}