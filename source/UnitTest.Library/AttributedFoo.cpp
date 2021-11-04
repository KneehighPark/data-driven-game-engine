#include "pch.h"
#include "AttributedFoo.h"

using namespace std::string_literals;

namespace Library
{
	RTTI_DEFINITIONS(AttributedFoo);

	AttributedFoo::AttributedFoo() : Attributed(AttributedFoo::TypeIdClass())
	{

	}

	gsl::owner<Scope*> AttributedFoo::Clone() const
	{
		return new AttributedFoo(*this);
	}

	bool AttributedFoo::Equals(const RTTI* t_rhs) const
	{
		auto attributed_foo = t_rhs->As<AttributedFoo>();
		if (attributed_foo != nullptr)
		{
			return *attributed_foo == *this;
		}
		return false;
	}

	std::string AttributedFoo::ToString() const
	{
		return "AttributedFoo"s;
	}

	const Vector<Signature> AttributedFoo::Signatures()
	{
		return Vector<Signature>
		{
			{ "ExternalInteger", Datum::DatumType::INTEGER, 1, offsetof(AttributedFoo, external_integer) },
			{ "ExternalFloat", Datum::DatumType::FLOAT, 1, offsetof(AttributedFoo,external_float) },
			{ "ExternalString", Datum::DatumType::STRING, 1, offsetof(AttributedFoo,external_string) },
			{ "ExternalVector", Datum::DatumType::VECTOR4, 1, offsetof(AttributedFoo,external_vector) },
			{ "ExternalMatrix", Datum::DatumType::MATRIX4X4, 1, offsetof(AttributedFoo,external_matrix) },
			{ "ExternalIntegerArray", Datum::DatumType::INTEGER, SIZE_OF_ARRAY, offsetof(AttributedFoo,external_integer_array) },
			{ "ExternalFloatArray", Datum::DatumType::FLOAT, SIZE_OF_ARRAY, offsetof(AttributedFoo,external_float_array) },
			{ "ExternalStringArray", Datum::DatumType::STRING, SIZE_OF_ARRAY, offsetof(AttributedFoo,external_string_array) },
			{ "ExternalVectorArray", Datum::DatumType::VECTOR4, SIZE_OF_ARRAY, offsetof(AttributedFoo,external_vector_array) },
			{ "ExternalMatrixArray", Datum::DatumType::MATRIX4X4, SIZE_OF_ARRAY, offsetof(AttributedFoo,external_matrix_array) },
			{ "NestedScope", Datum::DatumType::TABLE, 0, 0 },
			{ "NestedScopes", Datum::DatumType::TABLE, SIZE_OF_ARRAY, 0 }
		};
	}
}