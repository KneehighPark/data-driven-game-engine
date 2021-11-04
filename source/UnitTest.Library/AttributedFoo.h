#pragma once
#include "Attributed.h"
#include "TypeRegistry.h"

namespace Library
{
	class AttributedFoo final : public Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, RTTI);
		static const size_t SIZE_OF_ARRAY = 10;

	public:

		AttributedFoo();
		AttributedFoo(const AttributedFoo& rhs) = default;
		AttributedFoo(AttributedFoo&& rhs) = default;
		AttributedFoo& operator=(const AttributedFoo& rhs) = default;
		AttributedFoo& operator=(AttributedFoo&& rhs) = default;
		~AttributedFoo() = default;

		int32_t external_integer = 0;
		float_t external_float = 0.0f;
		std::string external_string;
		glm::vec4 external_vector;
		glm::mat4x4 external_matrix;

		int32_t external_integer_array[SIZE_OF_ARRAY];
		float_t external_float_array[SIZE_OF_ARRAY];
		std::string external_string_array[SIZE_OF_ARRAY];
		glm::vec4 external_vector_array[SIZE_OF_ARRAY];
		glm::mat4x4 external_matrix_array[SIZE_OF_ARRAY];

		gsl::owner<Scope*> Clone() const override;
		bool Equals(const RTTI* t_rhs) const override;
		std::string ToString() const override;

		static const Library::Vector<Library::Signature> Signatures();

	};
}