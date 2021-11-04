#include "pch.h"
#include "CppUnitTest.h"
#include <string>

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
	inline std::wstring ToString<Datum::DatumType>(const Datum::DatumType& t)
	{
		RETURN_WIDE_STRING(static_cast<int>(t));
	}
}


namespace UnitTestLibraryDesktop
{
	TEST_CLASS(DatumTests)
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

#pragma region ConstructorsTests

		TEST_METHOD(Constructor)
		{
			Datum d;
			Assert::AreEqual<size_t>(0, d.Size());
			Assert::AreEqual<size_t>(0, d.Capacity());
			Assert::AreEqual(Datum::DatumType::UNKNOWN, d.Type());
			Assert::IsFalse(d.IsExternal());
		}

		TEST_METHOD(CopyConstructor)
		{
			Datum d;
			d.PushBack(10);

			Datum copy(d);
			Assert::AreEqual<size_t>(1, copy.Size());
			Assert::AreEqual<size_t>(1, copy.Capacity());
			Assert::AreEqual(Datum::DatumType::INTEGER, copy.Type());
			Assert::IsFalse(copy.IsExternal());
		}

		TEST_METHOD(CopyAssignmentOperator)
		{
			Datum d;
			d.PushBack(10);

			Datum copy;
			copy = d;
			Assert::AreEqual<size_t>(1, copy.Size());
			Assert::AreEqual<size_t>(1, copy.Capacity());
			Assert::AreEqual(Datum::DatumType::INTEGER, copy.Type());
			Assert::IsFalse(copy.IsExternal());
		}

		TEST_METHOD(MoveConstructor)
		{
			Datum d;
			d.PushBack(10);
			Datum another = std::move(d);

			Assert::AreEqual<size_t>(1, another.Size());
			Assert::AreEqual<size_t>(1, another.Capacity());
			Assert::AreEqual(Datum::DatumType::INTEGER, another.Type());
			Assert::IsFalse(another.IsExternal());

			auto expression = [&] {d.Get<int32_t>(); };
			Assert::ExpectException<std::runtime_error>(expression);
			Assert::AreEqual<size_t>(0, d.Size());
			Assert::AreEqual<size_t>(0, d.Capacity());
			Assert::AreEqual(Datum::DatumType::UNKNOWN, d.Type());
		}

		TEST_METHOD(MoveAssignmentOperator)
		{
			Datum d;
			d.PushBack(10);
			Datum another;
			another = std::move(d);

			Assert::AreEqual<size_t>(1, another.Size());
			Assert::AreEqual<size_t>(1, another.Capacity());
			Assert::AreEqual(Datum::DatumType::INTEGER, another.Type());
			Assert::IsFalse(another.IsExternal());

			auto expression = [&] {d.Get<int32_t>(); };
			Assert::ExpectException<std::runtime_error>(expression);
			Assert::AreEqual<size_t>(0, d.Size());
			Assert::AreEqual<size_t>(0, d.Capacity());
			Assert::AreEqual(Datum::DatumType::UNKNOWN, d.Type());
		}
#pragma endregion

		TEST_METHOD(Assignment)
		{
			Datum datum;
			datum = 10;
			Assert::AreEqual(10, datum.Get<int32_t>());
			Assert::AreEqual<size_t>(1, datum.Size());

			auto expression = [&] {datum = 2.0f; };
			Assert::ExpectException<std::runtime_error>(expression);

			datum.Resize(3);
			datum.Set(20, 1);
			datum.Set(30, 2);
			Assert::AreEqual(10, datum.Get<int32_t>(0));
			Assert::AreEqual(20, datum.Get<int32_t>(1));
			Assert::AreEqual(30, datum.Get<int32_t>(2));

			//Internal to internal
			Datum internal;
			internal = datum;
			Assert::AreEqual<size_t>(3, internal.Size());
			Assert::AreEqual<size_t>(3, internal.Capacity());
			Assert::AreEqual(Datum::DatumType::INTEGER, internal.Type());
			Assert::IsFalse(internal.IsExternal());

			//External to internal
			Datum external;
			external = 10;

			int32_t integers[] = { 10, 20, 30, 40, 50 };
			external.SetStorage(integers, 5);
			Assert::AreEqual<size_t>(5, external.Size());
			Assert::AreEqual<size_t>(5, external.Capacity());
			Assert::IsTrue(external.IsExternal());

			datum = external;
			Assert::AreEqual<size_t>(5, datum.Size());
			Assert::AreEqual<size_t>(5, datum.Capacity());
			Assert::IsTrue(datum.IsExternal());
			Assert::AreEqual(10, datum.Get<int32_t>(0));
			Assert::AreEqual(20, datum.Get<int32_t>(1));
			Assert::AreEqual(30, datum.Get<int32_t>(2));
			Assert::AreEqual(40, datum.Get<int32_t>(3));
			Assert::AreEqual(50, datum.Get<int32_t>(4));

			//External to External

			Datum another_external;
			another_external = 10;
			int32_t more_integers[] = { 10, 20, 30, 40 };
			another_external.SetStorage(more_integers, 4);
			external = another_external;

			Assert::AreEqual<size_t>(4, external.Size());
			Assert::AreEqual<size_t>(4, external.Capacity());
			Assert::IsTrue(external.IsExternal());
			Assert::AreEqual(10, external.Get<int32_t>(0));
			Assert::AreEqual(20, external.Get<int32_t>(1));
			Assert::AreEqual(30, external.Get<int32_t>(2));
			Assert::AreEqual(40, external.Get<int32_t>(3));
		}

		TEST_METHOD(Front)
		{
			{
				Datum integers;
				auto expression = [&] { integers.Front<int32_t>(); };
				Assert::ExpectException<std::runtime_error>(expression);
				integers = 10;
				Assert::IsTrue(10 == integers.Front<int32_t>());
			}
			{
				Datum floats;
				auto expression = [&] { floats.Front<float_t>(); };
				Assert::ExpectException<std::runtime_error>(expression);
				floats = 1.23f;
				Assert::AreEqual(1.23f, floats.Front<float_t>());
			}
			{
				Datum vectors;
				auto expression = [&] { vectors.Front<glm::vec4>(); };
				Assert::ExpectException<std::runtime_error>(expression);
				vectors = glm::vec4(10);
				Assert::IsTrue(glm::vec4(10) == vectors.Front<glm::vec4>());
			}
			{
				Datum matrices;
				auto expression = [&] { matrices.Front<glm::mat4x4>(); };
				Assert::ExpectException<std::runtime_error>(expression);
				matrices = glm::mat4x4(10);
				Assert::IsTrue(glm::mat4x4(10) == matrices.Front<glm::mat4x4>());
			}
			{
				Datum strings;
				auto expression = [&] { strings.Front<std::string>(); };
				Assert::ExpectException<std::exception>(expression);
				strings = "Shaan";
				Assert::IsTrue("Shaan" == strings.Front<std::string>());
			}
			{
				Datum RTTIs;
				auto expression = [&] { RTTIs.Front<RTTI*>(); };
				Assert::ExpectException<std::exception>(expression);
				Foo a(10);
				RTTIs = &a;
				Assert::IsTrue(RTTIs.Front<RTTI*>()->Equals((&a)));
			}
		}

		TEST_METHOD(ConstFront)
		{
			{
				Datum temp;
				const Datum integers = temp;
				auto expression = [&] { integers.Front<int32_t>(); };
				Assert::ExpectException<std::runtime_error>(expression);
				temp = 10;
				const Datum another_integers = temp;
				Assert::IsTrue(10 == another_integers.Front<int32_t>());
			}
			{
				Datum temp;
				const Datum floats = temp;
				auto expression = [&] { floats.Front<float_t>(); };
				Assert::ExpectException<std::runtime_error>(expression);
				temp = 1.23f;
				const Datum another_floats = temp;
				Assert::AreEqual(1.23f, another_floats.Front<float_t>());
			}
			{
				Datum temp;
				const Datum vectors = temp;
				auto expression = [&] { vectors.Front<glm::vec4>(); };
				Assert::ExpectException<std::runtime_error>(expression);
				temp = glm::vec4(10);
				const Datum another_vectors = temp;
				Assert::IsTrue(glm::vec4(10) == another_vectors.Front<glm::vec4>());
			}
			{
				Datum temp;
				const Datum matrices = temp;
				auto expression = [&] { matrices.Front<glm::mat4x4>(); };
				Assert::ExpectException<std::runtime_error>(expression);
				temp = glm::mat4x4(10);
				const Datum another_matrices = temp;
				Assert::IsTrue(glm::mat4x4(10) == another_matrices.Front<glm::mat4x4>());
			}
			{
				Datum temp;
				const Datum strings = temp;
				auto expression = [&] { strings.Front<std::string>(); };
				Assert::ExpectException<std::exception>(expression);
				temp = "Shaan";
				const Datum another_strings = temp;
				Assert::IsTrue("Shaan" == another_strings.Front<std::string>());
			}
			{
				Datum temp;
				const Datum RTTIs = temp;
				auto expression = [&] { RTTIs.Front<RTTI*>(); };
				Assert::ExpectException<std::exception>(expression);
				Foo a(10);
				temp = &a;
				const Datum another_RTTIs = temp;
				//Assert::IsTrue(another_RTTIs.Front<RTTI*>()->Equals(reinterpret_cast<RTTI*>(&a)));
			}
		}

		TEST_METHOD(Back)
		{
			{
				Datum integers;
				auto expression = [&] { integers.Back<int32_t>(); };
				Assert::ExpectException<std::runtime_error>(expression);
				int32_t integer_array[] = { 10,20,30,40,50 };
				integers.SetStorage(integer_array, 5);
				Assert::IsTrue(50 == integers.Back<int32_t>());
			}
			{
				Datum floats;
				auto expression = [&] { floats.Back<float_t>(); };
				Assert::ExpectException<std::runtime_error>(expression);
				float_t float_array[] = { 1.23f, 2.34f, 3.45f };
				floats.SetStorage(float_array, 3);
				Assert::AreEqual(3.45f, floats.Back<float_t>());
			}
			{
				Datum vectors;
				auto expression = [&] { vectors.Back<glm::vec4>(); };
				Assert::ExpectException<std::runtime_error>(expression);
				glm::vec4 a(1);
				glm::vec4 b(2);
				glm::vec4 c(3);
				glm::vec4 vector_array[] = { a,b,c };
				vectors.SetStorage(vector_array, 3);
				Assert::IsTrue(glm::vec4(3) == vectors.Back<glm::vec4>());
			}
			{
				Datum matrices;
				auto expression = [&] { matrices.Back<glm::mat4>(); };
				Assert::ExpectException<std::runtime_error>(expression);
				glm::mat4 a(1);
				glm::mat4 b(2);
				glm::mat4 c(3);
				glm::mat4 matrix_array[] = { a,b,c };
				matrices.SetStorage(matrix_array, 3);
				Assert::IsTrue(glm::mat4(3) == matrices.Back<glm::mat4>());
			}
			{
				Datum strings;
				auto expression = [&] { strings.Back<std::string>(); };
				Assert::ExpectException<std::exception>(expression);
				std::string string_array[] = { "Shaan", "M", "Joshi" };
				strings.SetStorage(string_array, 3);
				Assert::IsTrue("Joshi" == strings.Back<std::string>());
			}
			{
				Datum RTTIs;
				auto expression = [&] { RTTIs.Back<RTTI*>(); };
				Assert::ExpectException<std::exception>(expression);
				Foo a(10);
				Foo b(20);
				Foo c(30);

				Foo** foos = new Foo* [3];
				foos[0] = &a;
				foos[1] = &b;
				foos[2] = &c;
				RTTIs.SetStorage(reinterpret_cast<RTTI**>(foos), 3);
				Assert::IsTrue(RTTIs.Back<RTTI*>()->Equals(reinterpret_cast<RTTI*>(&c)));
				delete foos;
			}
		}

		TEST_METHOD(ConstBack)
		{
			{
				Datum temp;
				const Datum integers = temp;
				auto expression = [&] { integers.Back<int32_t>(); };
				Assert::ExpectException<std::runtime_error>(expression);
				int32_t integer_array[] = { 10,20,30,40,50 };
				temp.SetStorage(integer_array, 5);
				const Datum another_integers = temp;
				Assert::IsTrue(50 == another_integers.Back<int32_t>());
			}

			{
				Datum temp;
				const Datum floats = temp;
				auto expression = [&] { floats.Back<float_t>(); };
				Assert::ExpectException<std::runtime_error>(expression);
				float_t float_array[] = { 1.23f, 2.34f, 3.45f };
				temp.SetStorage(float_array, 3);
				const Datum another_floats = temp;
				Assert::AreEqual(3.45f, another_floats.Back<float_t>());
			}
			{
				Datum temp;
				const Datum vectors = temp;
				auto expression = [&] { vectors.Back<glm::vec4>(); };
				Assert::ExpectException<std::runtime_error>(expression);
				glm::vec4 a(1);
				glm::vec4 b(2);
				glm::vec4 c(3);
				glm::vec4 vector_array[] = { a,b,c };
				temp.SetStorage(vector_array, 3);
				const Datum another_vectors = temp;
				Assert::IsTrue(glm::vec4(3) == another_vectors.Back<glm::vec4>());
			}
			{
				Datum temp;
				const Datum matrices = temp;
				auto expression = [&] { matrices.Back<glm::mat4>(); };
				Assert::ExpectException<std::runtime_error>(expression);
				glm::mat4 a(1);
				glm::mat4 b(2);
				glm::mat4 c(3);
				glm::mat4 matrix_array[] = { a,b,c };
				temp.SetStorage(matrix_array, 3);
				const Datum another_matrices = temp;
				Assert::IsTrue(glm::mat4(3) == another_matrices.Back<glm::mat4>());
			}
			{
				Datum temp;
				const Datum strings = temp;
				auto expression = [&] { strings.Back<std::string>(); };
				Assert::ExpectException<std::exception>(expression);
				std::string string_array[] = { "Shaan", "M", "Joshi" };
				temp.SetStorage(string_array, 3);
				const Datum another_strings = temp;
				Assert::IsTrue("Joshi" == another_strings.Back<std::string>());
			}
			{
				Datum temp;
				const Datum RTTIs = temp;
				auto expression = [&] { RTTIs.Back<RTTI*>(); };
				Assert::ExpectException<std::exception>(expression);
				Foo a(10);
				Foo b(20);
				Foo c(30);

				Foo** foos = new Foo * [3];
				foos[0] = &a;
				foos[1] = &b;
				foos[2] = &c;
				temp.SetStorage(reinterpret_cast<RTTI**>(foos), 3);
				const Datum another_RTTIs = temp;
				Assert::IsTrue(another_RTTIs.Back<RTTI*>()->Equals(reinterpret_cast<RTTI*>(&c)));
				delete foos;
			}
		}

		TEST_METHOD(Set)
		{
			Datum typeless;
			auto expression = [&] {typeless.Set(10); };
			Assert::ExpectException<std::exception>(expression);
			typeless.PushBack(10);
			auto expression2 = [&] {typeless.Set(20,5); };
			Assert::ExpectException<std::exception>(expression2);
			auto expression3 = [&] {typeless.Set(1.23f); };
			Assert::ExpectException<std::exception>(expression3);


			Datum integers;
			integers = 10;
			integers.Set(20);
			Assert::IsTrue(20 == integers.Front<int32_t>());

			Datum floats;
			floats = 1.23f;
			floats.Set(2.34f);
			Assert::IsTrue(2.34f == floats.Front<float_t>());

			Datum vectors;
			vectors = glm::vec4(100);
			vectors.Set(glm::vec4(200));
			Assert::IsTrue(glm::vec4(200) == vectors.Front<glm::vec4>());

			Datum matrices;
			matrices = glm::mat4(100);
			matrices.Set(glm::mat4(200));
			Assert::IsTrue(glm::mat4(200) == matrices.Front<glm::mat4>());

			Datum strings;
			strings = "Shaan";
			strings.Set("Joshi");
			Assert::IsTrue("Joshi" == strings.Front<std::string>());

			Datum rtti;
			Foo a(10);
			Foo b(20);
			rtti = &a;
			rtti.Set(&b);
			Assert::IsTrue(rtti.Front<RTTI*>()->Equals((&b)));
		}

		TEST_METHOD(Resize)
		{
			Datum typeless;
			auto expression = [&] {typeless.Resize(3); };
			Assert::ExpectException<std::exception>(expression);
			

			Datum integers;
			integers = 10;
			integers.Resize(3);
			integers.Set(20, 1);
			integers.Set(30, 2);
			Assert::AreEqual<size_t>(3, integers.Size());
			Assert::AreEqual<size_t>(3, integers.Capacity());
			integers.Resize(1);
			Assert::AreEqual<size_t>(1, integers.Size());
			Assert::AreEqual<size_t>(1, integers.Capacity());

			Datum floats;
			floats = 1.23f;
			floats.Resize(3);
			floats.Set(2.34f, 1);
			floats.Set(3.45f, 2);
			Assert::AreEqual<size_t>(3, floats.Size());
			Assert::AreEqual<size_t>(3, floats.Capacity());
			floats.Resize(1);
			Assert::AreEqual<size_t>(1, floats.Size());
			Assert::AreEqual<size_t>(1, floats.Capacity());

			Datum vectors;
			vectors = glm::vec4(100);
			vectors.Resize(4);
			vectors.Set(glm::vec4(200),1);
			vectors.Set(glm::vec4(300),2);
			vectors.Set(glm::vec4(400),3);
			Assert::AreEqual<size_t>(4, vectors.Size());
			Assert::AreEqual<size_t>(4, vectors.Capacity());
			vectors.Resize(1);
			Assert::AreEqual<size_t>(1, vectors.Size());
			Assert::AreEqual<size_t>(1, vectors.Capacity());

			Datum matrices;
			matrices = glm::mat4(100);
			matrices.Resize(4);
			matrices.Set(glm::mat4(200), 1);
			matrices.Set(glm::mat4(300), 2);
			matrices.Set(glm::mat4(400), 3);
			Assert::AreEqual<size_t>(4, matrices.Size());
			Assert::AreEqual<size_t>(4, matrices.Capacity());
			matrices.Resize(1);
			Assert::AreEqual<size_t>(1, matrices.Size());
			Assert::AreEqual<size_t>(1, matrices.Capacity());

			Datum strings; 
			strings = "Shaan";
			strings.Resize(3);
			strings.Set("M.", 1);
			strings.Set("Joshi", 2);
			Assert::AreEqual<size_t>(3, strings.Size());
			Assert::AreEqual<size_t>(3, strings.Capacity());
			strings.Resize(1);
			Assert::AreEqual<size_t>(1, strings.Size());
			Assert::AreEqual<size_t>(1, strings.Capacity());

			Datum rtti;
			Foo a(10);
			Foo b(20);
			Foo c(30);
			rtti = &a;
			rtti.Resize(3);
			rtti.Set(&b, 1);
			rtti.Set(&c, 2);
			Assert::AreEqual<size_t>(3, rtti.Size());
			Assert::AreEqual<size_t>(3, rtti.Capacity());
			rtti.Resize(1);
			Assert::AreEqual<size_t>(1, rtti.Size());
			Assert::AreEqual<size_t>(1, rtti.Capacity());
		}

		TEST_METHOD(AlteringExternal)
		{
			Datum external;
			int32_t integers[] = { 10, 20, 30, 40, 50 };
			external.SetStorage(integers, 5);
			auto expression = [&] { external.Resize(5); };
			Assert::ExpectException<std::runtime_error>(expression);
			auto expression2 = [&] { external.Reserve(5); };
			Assert::ExpectException<std::runtime_error>(expression2);
		}

		TEST_METHOD(ComparisonOperator)
		{
			Datum integers;
			integers = 10;
			Assert::IsTrue(integers == 10);
			Assert::IsTrue(integers != 5);
			Datum integers_two;
			integers_two = 15;
			Assert::IsTrue(integers != integers_two);

			Datum floats;
			floats = 10.1f;
			Assert::IsTrue(floats == 10.1f);
			Assert::IsTrue(floats != 5.1f);
			Datum floats_two;
			floats_two = 11.1f;
			Assert::IsTrue(floats != floats_two);

			Datum vectors;
			vectors = glm::vec4(100);
			Assert::IsTrue(vectors == glm::vec4(100));
			Assert::IsTrue(vectors != glm::vec4(10));
			Datum vectors_two;
			vectors_two = glm::vec4(200);
			Assert::IsTrue(vectors != vectors_two);

			Datum matrices;
			matrices = glm::mat4x4(100);
			Assert::IsTrue(matrices == glm::mat4x4(100));
			Assert::IsTrue(matrices != glm::mat4x4(10));
			Datum matrices_two;
			matrices_two = glm::mat4x4(200);
			Assert::IsTrue(matrices != matrices_two);

			Datum strings; 
			strings = "Shaan";
			Assert::IsTrue(strings == "Shaan");
			Assert::IsTrue(strings != "Sean");
			Datum strings_two;
			strings_two = "Shaan";
			Assert::IsTrue(strings == strings_two);

			Datum rtti;
			Foo a(10);
			Foo b(20);
			Foo c(30);
			rtti = &a;
			Assert::IsTrue(rtti == &a);
			Assert::IsTrue(rtti != &b);
			Datum rtti_two;
			rtti_two = &a;
			Assert::IsTrue(rtti == rtti_two);
		}

		TEST_METHOD(PopBack)
		{
			Datum datum;
			auto expression = [&] { datum.PopBack(); };
			Assert::ExpectException<std::runtime_error>(expression);

			Datum external;
			int32_t integer_array[] = { 10, 20, 30, 40, 50 };
			external.SetStorage(integer_array, 5);
			auto expression2 = [&] { external.PopBack(); };
			Assert::ExpectException<std::runtime_error>(expression2);

			Datum integers;
			integers.PushBack(10);
			integers.PushBack(20);
			integers.PushBack(30);
			integers.PopBack();
			Assert::AreEqual(10, integers.Front<int32_t>());
			Assert::AreEqual(20, integers.Back<int32_t>());

			Datum floats;
			floats.PushBack(10.1f);
			floats.PushBack(20.2f);
			floats.PushBack(30.3f);
			floats.PopBack();
			Assert::AreEqual(10.1f, floats.Front<float_t>());
			Assert::AreEqual(20.2f, floats.Back<float_t>());

			Datum vectors;
			vectors.PushBack(glm::vec4(100));
			vectors.PushBack(glm::vec4(200));
			vectors.PushBack(glm::vec4(300));
			vectors.PopBack();
			Assert::IsTrue(glm::vec4(100) == vectors.Front<glm::vec4>());
			Assert::IsTrue(glm::vec4(200) == vectors.Back<glm::vec4>());

			Datum matrices;
			matrices.PushBack(glm::mat4x4(100));
			matrices.PushBack(glm::mat4x4(200));
			matrices.PushBack(glm::mat4x4(300));
			matrices.PopBack();
			Assert::IsTrue(glm::mat4x4(100) == matrices.Front<glm::mat4x4>());
			Assert::IsTrue(glm::mat4x4(200) == matrices.Back<glm::mat4x4>());

			Datum strings;
			strings.PushBack("A");
			strings.PushBack("B");
			strings.PushBack("C");
			strings.PopBack();
			Assert::IsTrue("A" == strings.Front<std::string>());
			Assert::IsTrue("B" == strings.Back<std::string>());

			Datum rtti;
			Foo a(10);
			Foo b(20);
			Foo c(30);
			rtti.PushBack(&a);
			rtti.PushBack(&b);
			rtti.PushBack(&c);
			rtti.PopBack();
			Assert::IsTrue(rtti.Front<RTTI*>()->Equals((&a)));
			Assert::IsTrue(rtti.Back<RTTI*>()->Equals((&b)));
		}

		TEST_METHOD(SetType)
		{
			Datum datum;
			datum = 10;
			Assert::IsFalse(datum.IsExternal());
			auto expression = [&] { datum.SetType(Datum::DatumType::FLOAT); };
			Assert::ExpectException<std::exception>(expression);
		}

		TEST_METHOD(SetFromString)
		{
			Datum typeless;
			auto expression = [&] { typeless.SetFromString("TIME TO FAIL", 0); };
			Assert::ExpectException<std::runtime_error>(expression);

			int32_t integer_array[] = { 10, 20, 30, 40, 50 };
			typeless.SetStorage(integer_array, 5);
			auto expression2 = [&] { typeless.SetFromString("TIME TO FAIL", 10); };
			Assert::ExpectException<std::runtime_error>(expression2);

			Datum integers;
			integers.SetType(Datum::DatumType::INTEGER);
			integers.Resize(1);
			integers.SetFromString("100");
			Assert::AreEqual(100, integers.Front<int32_t>());

			Datum floats;
			floats.SetType(Datum::DatumType::FLOAT);
			floats.Resize(1);
			floats.SetFromString("100.1.f");
			Assert::AreEqual(100.1f, floats.Front<float_t>());

			Datum vectors;
			vectors.SetType(Datum::DatumType::VECTOR4);
			vectors.Resize(1);
			vectors.SetFromString("vec4(1, 2, 3, 4)");
			Assert::IsTrue(glm::vec4(1, 2, 3, 4) == vectors.Front<glm::vec4>());

			Datum matrices;
			matrices.SetType(Datum::DatumType::MATRIX4X4);
			matrices.Resize(1);
			matrices.SetFromString("mat4x4((1, 2, 3, 4), (1, 2, 3, 4), (1, 2, 3, 4), (1, 2, 3, 4))");
			Assert::IsTrue(glm::mat4x4(1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4) == matrices.Front<glm::mat4x4>());

			Datum strings; 
			strings.SetType(Datum::DatumType::STRING);
			strings.Resize(1);
			strings.SetFromString("ABCDE");
			Assert::IsTrue("ABCDE" == strings.Front<std::string>());

			Datum rtti;
			rtti.SetType(Datum::DatumType::POINTER);
			Foo a(10);
			auto expression3 = [&] {rtti.SetFromString("&a"); };
			Assert::ExpectException<std::runtime_error>(expression3);
		}

		TEST_METHOD(ToString)
		{
			Datum typeless;
			auto expression = [&] { typeless.ToString(); };
			Assert::ExpectException<std::runtime_error>(expression);

			int32_t integer_array[] = { 10, 20, 30, 40, 50 };
			typeless.SetStorage(integer_array, 5);
			auto expression2 = [&] { typeless.ToString(10); };
			Assert::ExpectException<std::runtime_error>(expression2);

			Datum integers;
			integers = 10;
			Assert::IsTrue("10" == integers.ToString());
		}

		TEST_METHOD(Remove)
		{
			Datum temp_int;
			auto expression = [&] { temp_int.Remove(10); };
			Assert::ExpectException<std::runtime_error>(expression);
			temp_int = 10;
			int32_t integer_array[] = { 10, 20, 30, 40, 50 };
			temp_int.SetStorage(integer_array, 5);
			auto expression2 = [&] { temp_int.Remove(10); };
			Assert::ExpectException<std::runtime_error>(expression2);

			Datum integers;
			integers.PushBack(10);
			integers.PushBack(20);
			integers.PushBack(30);
			integers.Remove(20);
			Assert::AreEqual(10, integers.Get<int32_t>());
			Assert::AreEqual(30, integers.Get<int32_t>(1));
			Assert::AreEqual<size_t>(2, integers.Size());
			Assert::AreEqual<size_t>(3, integers.Capacity());
			Assert::IsFalse(integers.Remove(999));

			Datum temp_floats;
			auto expression3 = [&] { temp_floats.Remove(10.1f); };
			Assert::ExpectException<std::runtime_error>(expression3);
			temp_floats = 10.1f;
			float_t float_array[] = { 10.1f, 20.2f, 30.3f, 40.4f, 50.5f };
			temp_floats.SetStorage(float_array, 5);
			auto expression4 = [&] { temp_floats.Remove(10.1f); };
			Assert::ExpectException<std::runtime_error>(expression4);

			Datum floats;
			floats.PushBack(10.1f);
			floats.PushBack(20.2f);
			floats.PushBack(30.3f);
			floats.Remove(20.2f);
			Assert::AreEqual(10.1f, floats.Get<float_t>());
			Assert::AreEqual(30.3f, floats.Get<float_t>(1));
			Assert::AreEqual<size_t>(2, floats.Size());
			Assert::AreEqual<size_t>(3, floats.Capacity());
			Assert::IsFalse(floats.Remove(1.11f));

			Datum temp_vector;
			glm::vec4 a(10);
			glm::vec4 b(20);
			glm::vec4 c(30);
			glm::vec4 d(40);
			glm::vec4 vector_array[] = { a,b,c };
			auto expression5 = [&] { temp_vector.Remove(a); };
			Assert::ExpectException<std::runtime_error>(expression5);
			temp_vector = a;
			temp_vector.SetStorage(vector_array, 3);
			auto expression6 = [&] { temp_vector.Remove(a); };
			Assert::ExpectException<std::runtime_error>(expression6);

			Datum vectors;
			vectors.PushBack(a);
			vectors.PushBack(b);
			vectors.PushBack(c);
			vectors.Remove(b);
			Assert::IsTrue(a == vectors.Get<glm::vec4>());
			Assert::IsTrue(c == vectors.Get<glm::vec4>(1));
			Assert::AreEqual<size_t>(2, vectors.Size());
			Assert::AreEqual<size_t>(3, vectors.Capacity());
			Assert::IsFalse(vectors.Remove(d));

			Datum temp_matrices;
			glm::mat4x4 w(1);
			glm::mat4x4 x(10);
			glm::mat4x4 y(20);
			glm::mat4x4 z(30);
			glm::mat4x4 matrices_array[] = { x,y,z };
			auto expression7 = [&] { temp_matrices.Remove(x); };
			Assert::ExpectException<std::runtime_error>(expression7);
			temp_matrices = x;
			temp_matrices.SetStorage(matrices_array, 3);
			auto expression8 = [&] { temp_matrices.Remove(x); };
			Assert::ExpectException<std::runtime_error>(expression8);

			Datum matrices;
			matrices.PushBack(x);
			matrices.PushBack(y);
			matrices.PushBack(z);
			matrices.Remove(y);
			Assert::IsTrue(x == matrices.Get<glm::mat4x4>());
			Assert::IsTrue(z == matrices.Get<glm::mat4x4>(1));
			Assert::AreEqual<size_t>(2, matrices.Size());
			Assert::AreEqual<size_t>(3, matrices.Capacity());
			Assert::IsFalse(matrices.Remove(w));

			Datum temp_strings;
			auto expression9 = [&] { temp_strings.Remove("AAA"); };
			Assert::ExpectException<std::runtime_error>(expression9);
			temp_strings = "AAA";
			std::string string_array[] = { "AAA", "BBB", "CCC" };
			temp_strings.SetStorage(string_array, 3);
			auto expression10 = [&] { temp_strings.Remove("AAA"); };
			Assert::ExpectException<std::runtime_error>(expression10);

			Datum strings;
			strings.PushBack("AAA");
			strings.PushBack("BBB");
			strings.PushBack("CCC");
			strings.Remove("BBB");
			Assert::IsTrue("AAA" == strings.Get<std::string>());
			Assert::IsTrue("CCC" == strings.Get<std::string>(1));
			Assert::AreEqual<size_t>(2, strings.Size());
			Assert::AreEqual<size_t>(3, strings.Capacity());
			Assert::IsFalse(strings.Remove("DDD"));

			Foo i(10);
			Foo j(20);
			Foo k(30);
			Foo l(40);
			RTTI* rtti_array[] = { &i, &j, &k };
			Datum temp_rtti;
			auto expression11 = [&] { temp_rtti.Remove(i); };
			Assert::ExpectException<std::runtime_error>(expression11);
			temp_rtti = &i;
			temp_rtti.SetStorage(rtti_array, 3);
			auto expression12 = [&] { temp_rtti.Remove(i); };
			Assert::ExpectException<std::runtime_error>(expression12);

			Datum rttis;
			rttis.PushBack(&i);
			rttis.PushBack(&j);
			rttis.PushBack(&k);
			rttis.Remove(j);
			Assert::IsTrue(&i == rttis.Get<RTTI*>());
			Assert::IsTrue(&k == rttis.Get<RTTI*>(1));
			Assert::AreEqual<size_t>(2, rttis.Size());
			Assert::AreEqual<size_t>(3, rttis.Capacity());
			Assert::IsFalse(rttis.Remove(l));
		}

		TEST_METHOD(RemoveAt)
		{
			Datum temp;
			auto expression = [&] {temp.RemoveAt(0); };
			Assert::ExpectException<std::runtime_error>(expression);
			temp = 10;
			int32_t integer_array[] = { 10, 20, 30, 40, 50 };
			temp.SetStorage(integer_array, 5);
			auto expression2 = [&] {temp.RemoveAt(0); };
			Assert::ExpectException<std::runtime_error>(expression2);

			Datum integers;
			integers.PushBack(10);
			integers.PushBack(20);
			integers.PushBack(30);
			integers.RemoveAt(1);
			Assert::AreEqual(10, integers.Get<int32_t>());
			Assert::AreEqual(30, integers.Get<int32_t>(1));
			Assert::AreEqual<size_t>(2, integers.Size());
			Assert::AreEqual<size_t>(3, integers.Capacity());

			Datum floats;
			floats.PushBack(10.1f);
			floats.PushBack(20.2f);
			floats.PushBack(30.3f);
			floats.RemoveAt(1);
			Assert::AreEqual(10.1f, floats.Get<float_t>());
			Assert::AreEqual(30.3f, floats.Get<float_t>(1));
			Assert::AreEqual<size_t>(2, floats.Size());
			Assert::AreEqual<size_t>(3, floats.Capacity());

			glm::vec4 a(10);
			glm::vec4 b(20);
			glm::vec4 c(30);
			Datum vectors;
			vectors.PushBack(a);
			vectors.PushBack(b);
			vectors.PushBack(c);
			vectors.RemoveAt(1);
			Assert::IsTrue(a == vectors.Get<glm::vec4>());
			Assert::IsTrue(c == vectors.Get<glm::vec4>(1));
			Assert::AreEqual<size_t>(2, vectors.Size());
			Assert::AreEqual<size_t>(3, vectors.Capacity());

			glm::mat4x4 x(10);
			glm::mat4x4 y(20);
			glm::mat4x4 z(30);
			Datum matrices;
			matrices.PushBack(x);
			matrices.PushBack(y);
			matrices.PushBack(z);
			matrices.RemoveAt(1);
			Assert::IsTrue(x == matrices.Get<glm::mat4x4>());
			Assert::IsTrue(z == matrices.Get<glm::mat4x4>(1));
			Assert::AreEqual<size_t>(2, matrices.Size());
			Assert::AreEqual<size_t>(3, matrices.Capacity());

			Datum strings;
			strings.PushBack("AAA");
			strings.PushBack("BBB");
			strings.PushBack("CCC");
			strings.RemoveAt(1);
			Assert::IsTrue("AAA" == strings.Get<std::string>());
			Assert::IsTrue("CCC" == strings.Get<std::string>(1));
			Assert::AreEqual<size_t>(2, strings.Size());
			Assert::AreEqual<size_t>(3, strings.Capacity());

			Foo i(10);
			Foo j(20);
			Foo k(30);
			Datum rttis;
			rttis.PushBack(&i);
			rttis.PushBack(&j);
			rttis.PushBack(&k);
			rttis.RemoveAt(1);
			Assert::IsTrue(&i == rttis.Get<RTTI*>());
			Assert::IsTrue(&k == rttis.Get<RTTI*>(1));
			Assert::AreEqual<size_t>(2, rttis.Size());
			Assert::AreEqual<size_t>(3, rttis.Capacity());
		}

		TEST_METHOD(ConstFind)
		{
			{
				Datum datum1; 
				const Datum const1 = datum1;
				auto expression = [&] {const1.Find(10); };
				Assert::ExpectException<std::runtime_error>(expression);
				datum1.SetType(Datum::DatumType::INTEGER);
				int32_t integer_array[] = { 10, 20, 30, 40, 50 };
				datum1.SetStorage(integer_array, 5);
				const Datum const2 = datum1;
				Assert::IsTrue(0 == const2.Find(10));
				Assert::IsTrue(1 == const2.Find(20));
				Assert::IsTrue(2 == const2.Find(30));
				Assert::IsTrue(5 == const2.Find(999));
			}
			{
				Datum datum1;
				const Datum const1 = datum1;
				auto expression = [&] {const1.Find(10.1f); };
				Assert::ExpectException<std::runtime_error>(expression);
				datum1.SetType(Datum::DatumType::FLOAT);
				float_t float_array[] = { 10.1f, 20.2f, 30.3f, 40.4f, 50.5f };
				datum1.SetStorage(float_array, 5);
				const Datum const2 = datum1;
				Assert::IsTrue(0 == const2.Find(10.1f));
				Assert::IsTrue(1 == const2.Find(20.2f));
				Assert::IsTrue(2 == const2.Find(30.3f));
				Assert::IsTrue(5 == const2.Find(999.9f));
			}
			{
				glm::vec4 a(10);
				glm::vec4 b(20);
				glm::vec4 c(30);
				glm::vec4 d(40);
				glm::vec4 e(999);
				Datum datum1;
				const Datum const1 = datum1;
				auto expression = [&] {const1.Find(a); };
				Assert::ExpectException<std::runtime_error>(expression);
				datum1.SetType(Datum::DatumType::VECTOR4);
				glm::vec4 vector_array[] = { a,b,c,d };
				datum1.SetStorage(vector_array, 4);
				const Datum const2 = datum1;
				Assert::IsTrue(0 == const2.Find(a));
				Assert::IsTrue(1 == const2.Find(b));
				Assert::IsTrue(2 == const2.Find(c));
				Assert::IsTrue(3 == const2.Find(d));
				Assert::IsTrue(4 == const2.Find(e));
			}
			{
				glm::mat4x4 a(10);
				glm::mat4x4 b(20);
				glm::mat4x4 c(30);
				glm::mat4x4 d(40);
				glm::mat4x4 e(999);
				Datum datum1;
				const Datum const1 = datum1;
				auto expression = [&] {const1.Find(a); };
				Assert::ExpectException<std::runtime_error>(expression);
				datum1.SetType(Datum::DatumType::MATRIX4X4);
				glm::mat4x4 matrix_array[] = { a,b,c,d };
				datum1.SetStorage(matrix_array, 4);
				const Datum const2 = datum1;
				Assert::IsTrue(0 == const2.Find(a));
				Assert::IsTrue(1 == const2.Find(b));
				Assert::IsTrue(2 == const2.Find(c));
				Assert::IsTrue(3 == const2.Find(d));
				Assert::IsTrue(4 == const2.Find(e));
			}
			{
				Datum datum1;
				const Datum const1 = datum1;
				auto expression = [&] {const1.Find("AAA"); };
				Assert::ExpectException<std::runtime_error>(expression);
				datum1.SetType(Datum::DatumType::STRING);
				std::string string_array[] = { "AAA", "BBB", "CCC", "DDD" };
				datum1.SetStorage(string_array, 4);
				const Datum const2 = datum1;
				Assert::IsTrue(0 == const2.Find("AAA"));
				Assert::IsTrue(1 == const2.Find("BBB"));
				Assert::IsTrue(2 == const2.Find("CCC"));
				Assert::IsTrue(3 == const2.Find("DDD"));
				Assert::IsTrue(4 == const2.Find("EEE"));
			}
			{
				/*Foo a(10);
				Foo b(20);
				Foo c(30);
				Foo d(40);
				Datum datum1;
				const Datum const1 = datum1;
				auto expression = [&] {const1.Find(a); };
				Assert::ExpectException<std::runtime_error>(expression);
				datum1.SetType(Datum::DatumType::POINTER);
				Foo** foos = new Foo* [3];
				foos[0] = a;
				foos[1] = b;
				foos[2] = c;
				datum1.SetStorage(reinterpret_cast<RTTI**>(foos), 3);
				const Datum const2 = datum1;
				Assert::IsTrue(0 == const2.Find(a));
				Assert::IsTrue(1 == const2.Find(b));
				Assert::IsTrue(2 == const2.Find(c));
				Assert::IsTrue(3 == const2.Find(d));
				delete[] foos;*/
			}
		}
	private:
		static _CrtMemState sStartMemState;
	};
	_CrtMemState DatumTests::sStartMemState;
}