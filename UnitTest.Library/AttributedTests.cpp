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
	std::wstring ToString<RTTI>(RTTI* t)
	{
		RETURN_WIDE_STRING(t);
	}

}

namespace UnitTestLibraryDesktop
{
	
	TEST_CLASS(AttributedTests)
	{
		TEST_CLASS_INITIALIZE(InitializeClass)
		{
			TypeRegistry::RegisterType(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());
		}

		TEST_CLASS_CLEANUP(CleanupClass)
		{
			TypeRegistry::UnregisterType(AttributedFoo::TypeIdClass());
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

		TEST_METHOD(Attributes)
		{
			AttributedFoo af;

			Assert::IsTrue(af.IsAttribute("this"));
			Assert::IsTrue(af.IsAttribute("ExternalInteger"));
			Assert::IsTrue(af.IsAttribute("ExternalFloat"));
			Assert::IsTrue(af.IsAttribute("ExternalString"));
			Assert::IsTrue(af.IsAttribute("ExternalVector"));
			Assert::IsTrue(af.IsAttribute("ExternalMatrix"));
			Assert::IsTrue(af.IsAttribute("ExternalIntegerArray"));
			Assert::IsTrue(af.IsAttribute("ExternalFloatArray"));
			Assert::IsTrue(af.IsAttribute("ExternalStringArray"));
			Assert::IsTrue(af.IsAttribute("ExternalVectorArray"));
			Assert::IsTrue(af.IsAttribute("ExternalMatrixArray"));
			Assert::IsTrue(af.IsAttribute("NestedScope"));
			Assert::IsTrue(af.IsAttribute("NestedScopes"));

			Assert::IsTrue(af.IsPrescribedAttribute("this"));
			Assert::IsTrue(af.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(af.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(af.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(af.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(af.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(af.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(af.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(af.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(af.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(af.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(af.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(af.IsPrescribedAttribute("NestedScopes"));

			Assert::IsFalse(af.IsAuxiliaryAttribute("this"));
			Assert::IsFalse(af.IsAuxiliaryAttribute("ExternalInteger"));
			Assert::IsFalse(af.IsAuxiliaryAttribute("ExternalFloat"));
			Assert::IsFalse(af.IsAuxiliaryAttribute("ExternalString"));
			Assert::IsFalse(af.IsAuxiliaryAttribute("ExternalVector"));
			Assert::IsFalse(af.IsAuxiliaryAttribute("ExternalMatrix"));
			Assert::IsFalse(af.IsAuxiliaryAttribute("ExternalIntegerArray"));
			Assert::IsFalse(af.IsAuxiliaryAttribute("ExternalFloatArray"));
			Assert::IsFalse(af.IsAuxiliaryAttribute("ExternalStringArray"));
			Assert::IsFalse(af.IsAuxiliaryAttribute("ExternalVectorArray"));
			Assert::IsFalse(af.IsAuxiliaryAttribute("ExternalMatrixArray"));
			Assert::IsFalse(af.IsAuxiliaryAttribute("NestedScope"));
			Assert::IsFalse(af.IsAuxiliaryAttribute("NestedScopeArray"));

			Assert::AreEqual(static_cast<RTTI*>(&af), af["this"].Get<RTTI*>());
		}

		TEST_METHOD(CopyConstructor)
		{
			AttributedFoo af;
			AttributedFoo another_af = af;

			Assert::IsTrue(another_af.IsAttribute("this"));
			Assert::IsTrue(another_af.IsAttribute("ExternalInteger"));
			Assert::IsTrue(another_af.IsAttribute("ExternalFloat"));
			Assert::IsTrue(another_af.IsAttribute("ExternalString"));
			Assert::IsTrue(another_af.IsAttribute("ExternalVector"));
			Assert::IsTrue(another_af.IsAttribute("ExternalMatrix"));
			Assert::IsTrue(another_af.IsAttribute("ExternalIntegerArray"));
			Assert::IsTrue(another_af.IsAttribute("ExternalFloatArray"));
			Assert::IsTrue(another_af.IsAttribute("ExternalStringArray"));
			Assert::IsTrue(another_af.IsAttribute("ExternalVectorArray"));
			Assert::IsTrue(another_af.IsAttribute("ExternalMatrixArray"));
			Assert::IsTrue(another_af.IsAttribute("NestedScope"));
			Assert::IsTrue(another_af.IsAttribute("NestedScopes"));

			Assert::IsTrue(another_af.IsPrescribedAttribute("this"));
			Assert::IsTrue(another_af.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(another_af.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(another_af.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(another_af.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(another_af.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(another_af.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(another_af.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(another_af.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(another_af.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(another_af.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(another_af.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(another_af.IsPrescribedAttribute("NestedScopes"));

			Assert::IsFalse(another_af.IsAuxiliaryAttribute("this"));
			Assert::IsFalse(another_af.IsAuxiliaryAttribute("ExternalInteger"));
			Assert::IsFalse(another_af.IsAuxiliaryAttribute("ExternalFloat"));
			Assert::IsFalse(another_af.IsAuxiliaryAttribute("ExternalString"));
			Assert::IsFalse(another_af.IsAuxiliaryAttribute("ExternalVector"));
			Assert::IsFalse(another_af.IsAuxiliaryAttribute("ExternalMatrix"));
			Assert::IsFalse(another_af.IsAuxiliaryAttribute("ExternalIntegerArray"));
			Assert::IsFalse(another_af.IsAuxiliaryAttribute("ExternalFloatArray"));
			Assert::IsFalse(another_af.IsAuxiliaryAttribute("ExternalStringArray"));
			Assert::IsFalse(another_af.IsAuxiliaryAttribute("ExternalVectorArray"));
			Assert::IsFalse(another_af.IsAuxiliaryAttribute("ExternalMatrixArray"));
			Assert::IsFalse(another_af.IsAuxiliaryAttribute("NestedScope"));
			Assert::IsFalse(another_af.IsAuxiliaryAttribute("NestedScopeArray"));

			int32_t& af_data = af["ExternalInteger"].Get<int32_t>();
			int32_t& another_af_data = another_af["ExternalInteger"].Get<int32_t>();
			Assert::AreNotEqual(&another_af_data, &af_data);
		}

		TEST_METHOD(CopyAssignment)
		{
			AttributedFoo af;
			AttributedFoo another_af;
			another_af = af;

			Assert::IsTrue(another_af.IsAttribute("this"));
			Assert::IsTrue(another_af.IsAttribute("ExternalInteger"));
			Assert::IsTrue(another_af.IsAttribute("ExternalFloat"));
			Assert::IsTrue(another_af.IsAttribute("ExternalString"));
			Assert::IsTrue(another_af.IsAttribute("ExternalVector"));
			Assert::IsTrue(another_af.IsAttribute("ExternalMatrix"));
			Assert::IsTrue(another_af.IsAttribute("ExternalIntegerArray"));
			Assert::IsTrue(another_af.IsAttribute("ExternalFloatArray"));
			Assert::IsTrue(another_af.IsAttribute("ExternalStringArray"));
			Assert::IsTrue(another_af.IsAttribute("ExternalVectorArray"));
			Assert::IsTrue(another_af.IsAttribute("ExternalMatrixArray"));
			Assert::IsTrue(another_af.IsAttribute("NestedScope"));
			Assert::IsTrue(another_af.IsAttribute("NestedScopes"));

			Assert::IsTrue(another_af.IsPrescribedAttribute("this"));
			Assert::IsTrue(another_af.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(another_af.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(another_af.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(another_af.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(another_af.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(another_af.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(another_af.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(another_af.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(another_af.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(another_af.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(another_af.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(another_af.IsPrescribedAttribute("NestedScopes"));

			Assert::IsFalse(another_af.IsAuxiliaryAttribute("this"));
			Assert::IsFalse(another_af.IsAuxiliaryAttribute("ExternalInteger"));
			Assert::IsFalse(another_af.IsAuxiliaryAttribute("ExternalFloat"));
			Assert::IsFalse(another_af.IsAuxiliaryAttribute("ExternalString"));
			Assert::IsFalse(another_af.IsAuxiliaryAttribute("ExternalVector"));
			Assert::IsFalse(another_af.IsAuxiliaryAttribute("ExternalMatrix"));
			Assert::IsFalse(another_af.IsAuxiliaryAttribute("ExternalIntegerArray"));
			Assert::IsFalse(another_af.IsAuxiliaryAttribute("ExternalFloatArray"));
			Assert::IsFalse(another_af.IsAuxiliaryAttribute("ExternalStringArray"));
			Assert::IsFalse(another_af.IsAuxiliaryAttribute("ExternalVectorArray"));
			Assert::IsFalse(another_af.IsAuxiliaryAttribute("ExternalMatrixArray"));
			Assert::IsFalse(another_af.IsAuxiliaryAttribute("NestedScope"));
			Assert::IsFalse(another_af.IsAuxiliaryAttribute("NestedScopeArray"));
		}

		TEST_METHOD(MoveConstructor)
		{
			AttributedFoo af;

			Assert::IsTrue(af.IsAttribute("this"));
			af["ExternalInteger"].Get<int32_t>() = 123;
			af["ExternalFloat"].Get<float_t>() = 1.23f;

			AttributedFoo another_af(std::move(af));

			Assert::IsTrue(another_af.IsAttribute("this"));
			Assert::AreEqual<size_t>(123, another_af.external_integer);
			Assert::AreEqual(1.23f, another_af.external_float);
		}

		TEST_METHOD(MoveAssignment)
		{
			AttributedFoo af;
			Assert::IsTrue(af.IsAttribute("this"));

			Assert::AreEqual<size_t>(0, af.external_integer);
			Assert::AreEqual(0.0f, af.external_float);

			AttributedFoo another_af;
			another_af["ExternalInteger"].Get<int32_t>() = 123;
			another_af["ExternalFloat"].Get<float_t>() = 1.23f;
			af = std::move(another_af);

			Assert::IsTrue(af.IsAttribute("this"));
			Assert::AreEqual<size_t>(123, af.external_integer);
			Assert::AreEqual(1.23f, af.external_float);
		}

		TEST_METHOD(AppendAuxillaryAttribute)
		{
			AttributedFoo af;
			af.AppendAuxiliaryAttribute("AuxInt");
			Assert::IsTrue(af.IsAttribute("AuxInt"));
			Assert::IsTrue(af.IsAuxiliaryAttribute("AuxInt"));
			Assert::IsFalse(af.IsPrescribedAttribute("AuxInt"));

		
			auto expression = [&] {af.AppendAuxiliaryAttribute("ExternalInteger"); };
			Assert::ExpectException<std::runtime_error>(expression);
		}

		TEST_METHOD(GetAttributes)
		{
			AttributedFoo af;
			af["ExternalInteger"].Get<int32_t>() = 123;
			af["ExternalFloat"].Get<float_t>() = 1.23f;

			Vector<std::pair<std::string, Datum>*> attributes = af.GetAttributes();
			Assert::AreEqual<size_t>(13, attributes.Size());

			Assert::AreEqual("this"s, attributes[0]->first);
			Assert::AreEqual("ExternalInteger"s, attributes[1]->first);
			Assert::AreEqual("ExternalFloat"s, attributes[2]->first);
			Assert::AreEqual("ExternalString"s, attributes[3]->first);
			Assert::AreEqual("ExternalVector"s, attributes[4]->first);
			Assert::AreEqual("ExternalMatrix"s, attributes[5]->first);
			Assert::AreEqual("ExternalIntegerArray"s, attributes[6]->first);
			Assert::AreEqual("ExternalFloatArray"s, attributes[7]->first);
			Assert::AreEqual("ExternalStringArray"s, attributes[8]->first);
			Assert::AreEqual("ExternalVectorArray"s, attributes[9]->first);
			Assert::AreEqual("ExternalMatrixArray"s, attributes[10]->first);
			Assert::AreEqual("NestedScope"s, attributes[11]->first);
			Assert::AreEqual("NestedScopes"s, attributes[12]->first);
		}

	private:
		static _CrtMemState s_start_mem_state;
	};
	_CrtMemState AttributedTests::s_start_mem_state;
}
