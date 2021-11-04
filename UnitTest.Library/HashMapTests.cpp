#include "pch.h"
#include "CppUnitTest.h"
#include "HashFunctions.h"
#include "HashMap.h"
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

namespace Library
{
	template<>
	class HashFunctions<Foo>
	{
	public:
		size_t operator()(const Foo& key) const
		{
			return key.Data();
		}
	};
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(HashMapTests)
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
#pragma region HashFunctionTests

		TEST_METHOD(HashFunctionDefault)
		{
			HashFunctions<size_t> hash;
			size_t a = 10;
			size_t b = 10;
			size_t c = 20;

			Assert::AreEqual(hash(a), hash(b));
			Assert::AreNotEqual(hash(b), hash(c));
		}
		TEST_METHOD(HashFunctionCharPointer)
		{
			HashFunctions<char*> hash;
			char* a = "Pichu";
			char* b = "Eevee";
			char c[6];
			strncpy_s(c, b, strlen(b));
			Assert::AreEqual(hash(b), hash(c));
			Assert::AreNotEqual(hash(b), hash(a));
		}

		TEST_METHOD(HashFunctionFoo)
		{
			HashFunctions<Foo> hash;
			Foo a(10);
			Foo b(10);
			Foo c(20);
			
			Assert::AreEqual(hash(a), hash(b));
			Assert::AreNotEqual(hash(b), hash(c));
		}
#pragma endregion


#pragma region HashMapTests

		TEST_METHOD(Constructor)
		{
			HashMap<Foo, int> hashmap(10);
			Assert::AreEqual<size_t>(0, hashmap.Size());
		}

		TEST_METHOD(Insert)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);

			HashMap<Foo, int> hashmap;
			HashMap<Foo, int>::Iterator iter;
			iter = hashmap.Insert(HashMap<Foo, int>::PairType(a, 10));
			Assert::AreEqual(10, (*iter).second);
			iter = hashmap.Insert(HashMap<Foo, int>::PairType(b, 20));
			Assert::AreEqual(20, (*iter).second);
			iter = hashmap.Insert(HashMap<Foo, int>::PairType(c, 30));
			Assert::AreEqual(30, (*iter).second);
		}

		TEST_METHOD(Find)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);
			Foo d(40);

			HashMap<Foo, int> hashmap;
			
			hashmap.Insert(HashMap<Foo, int>::PairType(a, 10));
			hashmap.Insert(HashMap<Foo, int>::PairType(b, 20));
			hashmap.Insert(HashMap<Foo, int>::PairType(c, 30));

			HashMap<Foo, int>::Iterator iter = hashmap.Find(a);
			Assert::IsTrue(a == (*iter).first);
			iter = hashmap.Find(b);
			Assert::IsTrue(b == (*iter).first);
			iter = hashmap.Find(c);
			Assert::IsTrue(c == (*iter).first);
			iter = hashmap.Find(d);
			Assert::IsTrue(iter == hashmap.end());
		}

		TEST_METHOD(FindConst)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);
			Foo d(40);

			HashMap<Foo, int> hashmap;

			hashmap.Insert(HashMap<Foo, int>::PairType(a, 10));
			hashmap.Insert(HashMap<Foo, int>::PairType(b, 20));
			hashmap.Insert(HashMap<Foo, int>::PairType(c, 30));

			const HashMap<Foo, int> constHashmap = hashmap;
			HashMap<Foo, int>::ConstIterator const_iter = constHashmap.Find(a);
			Assert::IsTrue(a == (*const_iter).first);
			const_iter = constHashmap.Find(d);
			Assert::IsTrue(const_iter == constHashmap.end());
		}

		TEST_METHOD(IndexOperator)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);
			Foo d(40);

			HashMap<Foo, int> hashmap;
			HashMap<Foo, int>::Iterator iter;
			hashmap.Insert(HashMap<Foo, int>::PairType(a, 10));
			hashmap.Insert(HashMap<Foo, int>::PairType(b, 20));
			hashmap.Insert(HashMap<Foo, int>::PairType(c, 30));

			Assert::AreEqual(10, hashmap[a]);
			Assert::AreEqual(20, hashmap[b]);
			Assert::AreEqual(30, hashmap[c]);

			hashmap[a] = 40;
			Assert::AreEqual(40, hashmap[a]);
		}

		TEST_METHOD(At)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);
			Foo d(40);

			HashMap<Foo, int> hashmap;
			HashMap<Foo, int>::Iterator iter;

			hashmap.Insert(HashMap<Foo, int>::PairType(a, 10));
			hashmap.Insert(HashMap<Foo, int>::PairType(b, 20));
			hashmap.Insert(HashMap<Foo, int>::PairType(c, 30));

			Assert::AreEqual(hashmap.At(a), 10);
			Assert::AreEqual(hashmap.At(b), 20);
			Assert::AreEqual(hashmap.At(c), 30);

			auto expression = [&] {hashmap.At(d); };
			Assert::ExpectException<std::runtime_error>(expression);
		}

		TEST_METHOD(AtConst)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);
			Foo d(40);

			HashMap<Foo, int> hashmap;
			HashMap<Foo, int>::Iterator iter;

			hashmap.Insert(HashMap<Foo, int>::PairType(a, 10));
			hashmap.Insert(HashMap<Foo, int>::PairType(b, 20));
			hashmap.Insert(HashMap<Foo, int>::PairType(c, 30));

			const HashMap<Foo, int> constHashmap(hashmap);
			Assert::AreEqual(constHashmap.At(a), 10);
			Assert::AreEqual(constHashmap.At(b), 20);
			Assert::AreEqual(constHashmap.At(c), 30);
		}

		TEST_METHOD(ContainsKey)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);
			Foo d(40);

			HashMap<Foo, int> hashmap;

			hashmap.Insert(HashMap<Foo, int>::PairType(a, 10));
			hashmap.Insert(HashMap<Foo, int>::PairType(b, 20));
			hashmap.Insert(HashMap<Foo, int>::PairType(c, 30));

			Assert::IsTrue(hashmap.ContainsKey(a));
			Assert::IsTrue(hashmap.ContainsKey(b));
			Assert::IsTrue(hashmap.ContainsKey(c));
			Assert::IsFalse(hashmap.ContainsKey(d));

		}

		TEST_METHOD(Clear)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);

			HashMap<Foo, int> hashmap;
			hashmap.Insert(HashMap<Foo, int>::PairType(a, 10));
			hashmap.Insert(HashMap<Foo, int>::PairType(b, 20));
			hashmap.Insert(HashMap<Foo, int>::PairType(c, 30));
			Assert::AreEqual<size_t>(3, hashmap.Size());
			hashmap.Clear();
			Assert::AreEqual<size_t>(0, hashmap.Size());
		}

		TEST_METHOD(Remove)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);

			HashMap<Foo, int> hashmap;
			HashMap<Foo, int>::Iterator iter;

			hashmap.Insert(HashMap<Foo, int>::PairType(a, 10));
			hashmap.Insert(HashMap<Foo, int>::PairType(b, 20));
			hashmap.Insert(HashMap<Foo, int>::PairType(c, 30));
			
			hashmap.Remove(a);
			iter = hashmap.Find(a);
			Assert::IsTrue(iter == hashmap.end());

			hashmap.Remove(b);
			iter = hashmap.Find(b);
			Assert::IsTrue(iter == hashmap.end());

			iter = hashmap.Find(c);
			Assert::IsTrue(c == (*iter).first);
		}

		TEST_METHOD(Rehash)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);

			HashMap<Foo, int> hashmap(13);
			Assert::AreEqual<size_t>(13, hashmap.NumberOfBuckets());

			hashmap.Insert(HashMap<Foo, int>::PairType(a, 10));
			hashmap.Insert(HashMap<Foo, int>::PairType(b, 20));
			hashmap.Insert(HashMap<Foo, int>::PairType(c, 30));

			hashmap.Rehash(27);
			Assert::AreEqual(10, hashmap[a]);
			Assert::AreEqual(20, hashmap[b]);
			Assert::AreEqual(30, hashmap[c]);
			Assert::AreEqual<size_t>(27, hashmap.NumberOfBuckets());
		}

		TEST_METHOD(Begin)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);

			HashMap<Foo, int> hashmap;
			hashmap.Insert(HashMap<Foo, int>::PairType(a, 10));
			hashmap.Insert(HashMap<Foo, int>::PairType(b, 20));
			hashmap.Insert(HashMap<Foo, int>::PairType(c, 30));

			HashMap<Foo, int>::Iterator iter = hashmap.begin();
			Assert::AreEqual((*iter).first, a);
			hashmap.Remove(a);
			iter = hashmap.begin();
			Assert::AreEqual((*iter).first, b);
			hashmap.Remove(b);
			iter = hashmap.begin();
			Assert::AreEqual((*iter).first, c);
		}

		TEST_METHOD(BeginConst)
		{
			
		}

		TEST_METHOD(CBegin)
		{

		}

		TEST_METHOD(End)
		{
			Foo a(10);

			HashMap<Foo, int> hashmap;
			Assert::IsTrue(hashmap.begin() == hashmap.end());
			hashmap.Insert(HashMap<Foo, int>::PairType(a, 10));
			Assert::IsFalse(hashmap.begin() == hashmap.end());
		}

		TEST_METHOD(EndConst)
		{

		}

		TEST_METHOD(CEnd)
		{

		}

#pragma endregion

	private:
		static _CrtMemState sStartMemState;
	};
	_CrtMemState HashMapTests::sStartMemState;
}