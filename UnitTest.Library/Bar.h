#pragma once
#include "RTTI.h"

namespace UnitTests
{
	class Bar : public Library::RTTI
	{
		RTTI_DECLARATIONS(Bar, RTTI);

	public:
		explicit Bar(int data = 0);
		~Bar() = default;
		int Data() const;
		void SetData(int data);
		bool operator==(const Bar& t_rhs) const;
		bool operator!=(const Bar& t_rhs) const;
	private:
		int mData;
	};
}