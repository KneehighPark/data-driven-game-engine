#include "pch.h"
#include "JsonParseMaster.h"
#include "IJsonParseHelper.h"


namespace Library
{
	RTTI_DEFINITIONS(JsonParseMaster::SharedData)

#pragma region SharedData

		JsonParseMaster* JsonParseMaster::SharedData::GetJsonParseMaster()
	{
		return mParseMaster;
	}

	void JsonParseMaster::SharedData::SetJsonParseMaster(JsonParseMaster& parseMaster)
	{
		mParseMaster = &parseMaster;
	}

	size_t JsonParseMaster::SharedData::Depth() const
	{
		return depth;
	}

	void JsonParseMaster::SharedData::IncrementDepth()
	{
		++depth;
	}

	void JsonParseMaster::SharedData::DecrementDepth()
	{
		if (depth > 0)
		{
			--depth;
		}
	}

#pragma endregion

#pragma region JsonParseMaster

	Library::JsonParseMaster::JsonParseMaster(SharedData& data) : mSharedData(&data)
	{
		data.SetJsonParseMaster(*this);
	}

	JsonParseMaster::~JsonParseMaster()
	{
		if (mIsClone)
		{
			for (const auto& parseHelper : mParseHelpers)
			{
				delete parseHelper;
			}
			delete mSharedData;
		}
	}

	JsonParseMaster::JsonParseMaster(JsonParseMaster&& rhs) noexcept : 
		mParseHelpers(std::move(rhs.mParseHelpers)), 
		mSharedData(rhs.mSharedData),
		mIsClone(rhs.mIsClone),
		mFileName(std::move(rhs.mFileName))
	{
		mSharedData->SetJsonParseMaster(*this);
		rhs.mSharedData = nullptr;
		rhs.mIsClone = false;
	}

	JsonParseMaster& JsonParseMaster::operator=(JsonParseMaster&& rhs) noexcept
	{
		if (this != &rhs)
		{
			mParseHelpers = std::move(rhs.mParseHelpers);
			mSharedData = rhs.mSharedData;
			mFileName = std::move(mFileName);
			mIsClone = rhs.mIsClone;
			mSharedData->SetJsonParseMaster(*this);
			rhs.mIsClone = false;
			rhs.mSharedData = nullptr;
		}
		return *this;
	}


	const Vector<IJsonParseHelper*>& JsonParseMaster::GetParseHelpers() const
	{
		return mParseHelpers;
	}

	JsonParseMaster::SharedData* JsonParseMaster::GetSharedData()
	{
		return mSharedData;
	}

	void JsonParseMaster::SetSharedData(SharedData& data)
	{
		if (mIsClone)
		{
			delete mSharedData;
		}
		mSharedData = &data;
		mSharedData->SetJsonParseMaster(*this);
	}

	gsl::owner< JsonParseMaster*> JsonParseMaster::Clone()
	{
		gsl::owner<JsonParseMaster*> clone = new JsonParseMaster(*mSharedData->Create());

		for (const auto& helper : mParseHelpers)
		{
			clone->mParseHelpers.PushBack(helper->Create());
		}
		clone->mIsClone = true;
		return clone;
	}

	bool JsonParseMaster::IsClone() const
	{
		return mIsClone;
	}

	void JsonParseMaster::AddHelper(IJsonParseHelper& helper)
	{
		if (mIsClone)
		{
			throw std::exception("Invalid Operation! Cannot add helper to clone");
		}
		
		auto it = std::find_if(mParseHelpers.begin(), mParseHelpers.end(), [&helper](const IJsonParseHelper* h)
			{
				return helper.TypeIdInstance() == h->TypeIdInstance();
			});

		if (it != mParseHelpers.end())
		{
			throw std::runtime_error("This helper, or one of the same type, has already been added to this JsonParseMaster.");
		}
		mParseHelpers.PushBack(&helper);
	}

	bool JsonParseMaster::RemoveHelper(IJsonParseHelper& helper)
	{
		bool result = mParseHelpers.Remove(&helper);
		if (mIsClone && result)
		{
			delete& helper;
		}
		return result;
	}

	void JsonParseMaster::Parse(const std::string& jsonString)
	{
		std::stringstream stream;
		stream << jsonString;
		Parse(stream);
	}

	void JsonParseMaster::Parse(std::istream& jsonInputStream)
	{
		Json::Value value;
		jsonInputStream >> value;
		mSharedData->IncrementDepth();
		Parse(value);
		mSharedData->DecrementDepth();
	}

	const std::string& JsonParseMaster::GetFileName() const
	{
		return mFileName;
	}

	void JsonParseMaster::ParseFromFile(const std::string& filename)
	{
		std::ifstream jsonFile(filename);
		if (jsonFile.bad())
		{
			throw std::runtime_error("Unable to open specified file!");
		}
		mFileName = filename;
		Parse(jsonFile);
	}

	void JsonParseMaster::Initialize()
	{
		mSharedData->Initialize();
		for (auto& helper : mParseHelpers)
		{
			helper->Initialize();
		}
		mFileName.clear();
	}

	void JsonParseMaster::Parse(const Json::Value& value, bool IsArrayElement, size_t index)
	{
		const std::vector<std::string> keys = value.getMemberNames();
		for (const std::string& key : keys)
		{
			ParseKeyValuePair(key, value[key], IsArrayElement, index);
		}
	}

	void JsonParseMaster::ParseKeyValuePair(const std::string& key, const Json::Value& value, bool IsArrayElement, size_t index, size_t arraySize)
	{
		if (value.isArray())
		{
			size_t i = 0;
			for (const auto& element : value)
			{
				if (element.isObject())
				{
					mSharedData->IncrementDepth();
					Parse(element, true, i);
					mSharedData->DecrementDepth();
				}
				else
				{
					ParseKeyValuePair(key, element, true, i, value.size());
				}
				++i;
			}
		}
		else if (value.isObject())
		{
			mSharedData->IncrementDepth();
			for (IJsonParseHelper* helper : mParseHelpers)
			{
				if (helper->StartHandler(*mSharedData, key, value, IsArrayElement, index, arraySize))
				{
					Parse(value);
					helper->EndHandler(*mSharedData, key);
					break;
				}
			}
			mSharedData->DecrementDepth();
		}
		else
		{
			for (IJsonParseHelper* helper : mParseHelpers)
			{
				if (helper->StartHandler(*mSharedData, key, value, IsArrayElement, index, arraySize))
				{
					helper->EndHandler(*mSharedData, key);
					break;
				}
			}
		}
	}

#pragma endregion

}