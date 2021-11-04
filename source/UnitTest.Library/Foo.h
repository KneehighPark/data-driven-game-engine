#pragma once
#include "RTTI.h"
#include "Factory.h"

namespace UnitTests
{
	class Foo final : public Library::RTTI
	{
		RTTI_DECLARATIONS(Foo, Library::RTTI)

	public:
		explicit Foo(int data = 0);
		Foo(const Foo& rhs);
		Foo(Foo&& rhs);
		Foo& operator=(const Foo& rhs);
		Foo& operator=(Foo&& rhs);
		~Foo();

		bool operator==(const Foo& rhs) const noexcept;
		bool operator!=(const Foo& rhs) const noexcept;

		int Data() const;
		void SetData(int data);

	private:
		int* mData;
	};

	CONCRETE_FACTORY(Foo, Library::RTTI)
}