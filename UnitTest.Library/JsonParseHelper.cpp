#include "pch.h"
#include "JsonParseHelper.h"

namespace Library
{
#pragma region SharedData

	RTTI_DEFINITIONS(JsonParseHelper::SharedData)

	gsl::owner<JsonParseHelper::SharedData*> JsonParseHelper::SharedData::Create() const
	{
		return new SharedData();
	}

	void JsonParseHelper::SharedData::Initialize()
	{
		maxDepth = 0;
	}
#pragma endregion

#pragma region JsonParseHelper

	RTTI_DEFINITIONS(JsonParseHelper)

	gsl::owner<IJsonParseHelper*> JsonParseHelper::Create() const
	{
		return new JsonParseHelper();
	}

	bool JsonParseHelper::StartHandler(JsonParseMaster::SharedData& data, const std::string& key, const Json::Value& value, bool IsArrayElement, size_t index, size_t arraySize)
	{
		key;
		value;
		index; 
		arraySize; 

		JsonParseHelper::SharedData* sharedData = data.As<JsonParseHelper::SharedData>();

		if (sharedData == nullptr)
		{
			return false;
		}

		assert(sharedData != nullptr);

		++startHandlers;

		if (IsArrayElement)
		{
			++arrayElements;
		}

		if (sharedData->Depth() > sharedData->maxDepth)
		{
			sharedData->maxDepth = sharedData->Depth();
		}
		return true;
	}

	bool JsonParseHelper::EndHandler(JsonParseMaster::SharedData& data, const std::string& key)
	{
		key;
		JsonParseHelper::SharedData* sharedData = data.As<JsonParseHelper::SharedData>();

		if (sharedData == nullptr)
		{
			return false;
		}
		++endHandlers;
		return true;
	}

	void JsonParseHelper::Initialize()
	{
		IJsonParseHelper::Initialize();
		startHandlers = 0;
		endHandlers = 0;
		arrayElements = 0;
		mInitialized = true;
	}
}