#include "pch.h"
#include "Bar.h"


namespace UnitTests
{
	RTTI_DEFINITIONS(Bar);

	Bar::Bar(int data) : mData(data)
	{
	}

	int Bar::Data() const
	{
		return mData;
	}

	void Bar::SetData(int data)
	{
		mData = data;
	}

	bool Bar::operator==(const Bar& t_rhs) const
	{
		return mData == t_rhs.mData;
	}

	bool Bar::operator!=(const Bar& t_rhs) const
	{
		return !(*this == t_rhs);
	}

}