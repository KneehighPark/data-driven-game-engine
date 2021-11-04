#pragma once
#include "IJsonParseHelper.h"
#include "RTTI.h"
#include "JsonParseMaster.h"

namespace Library
{
	class JsonParseHelper final : public IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonParseHelper, IJsonParseHelper)

	public:

		class SharedData final : public JsonParseMaster::SharedData
		{
			RTTI_DECLARATIONS(SharedData, JsonParseMaster::SharedData)

		public:

			size_t maxDepth = 0;
			virtual gsl::owner<SharedData*> Create() const override;
			virtual void Initialize() override;
		};

		virtual void JsonParseHelper::Initialize() override;
		virtual gsl::owner<IJsonParseHelper*> JsonParseHelper::Create() const override;
		virtual bool JsonParseHelper::StartHandler(JsonParseMaster::SharedData& data, const std::string& key, const Json::Value& value, bool IsArrayElement, size_t index, size_t arraySize) override;
		virtual bool JsonParseHelper::EndHandler(JsonParseMaster::SharedData& data, const std::string& key) override;

		bool mInitialized = false;
		size_t arrayElements = 0;
		size_t startHandlers = 0;
		size_t endHandlers = 0;
	};
}