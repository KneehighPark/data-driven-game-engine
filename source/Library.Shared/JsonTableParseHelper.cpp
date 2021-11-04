#include "pch.h"
#include "JsonTableParseHelper.h"
#include "Sector.h"

namespace Library
{
	RTTI_DEFINITIONS(JsonTableParseHelper::SharedData)

#pragma region SharedData

	JsonTableParseHelper::SharedData::SharedData(Scope& scope) : mSharedScope(&scope)
	{

	}

	Scope* JsonTableParseHelper::SharedData::GetSharedData()
	{
		return mSharedScope;
	}

	void JsonTableParseHelper::SharedData::SetSharedData(Scope& rhs)
	{
		mSharedScope = &rhs;
	}

	gsl::owner<JsonTableParseHelper::SharedData*> JsonTableParseHelper::SharedData::Create() const
	{
		Scope* scope = mSharedScope->Clone();
		return new SharedData(*scope);
	}

	void JsonTableParseHelper::SharedData::Initialize()
	{

	}

#pragma endregion

#pragma region JsonTableParseHelper

	gsl::owner<IJsonParseHelper*> JsonTableParseHelper::Create() const
	{
		return new JsonTableParseHelper();
	}

	size_t JsonTableParseHelper::SizeOfStack() const
	{
		return mStack.Size();
	}

	void JsonTableParseHelper::Initialize()
	{
		IJsonParseHelper::Initialize();
	}

	bool JsonTableParseHelper::StartHandler(JsonParseMaster::SharedData& data, const std::string& key, const Json::Value& value, bool IsArrayElement, size_t index, size_t /*arraySize*/)
	{
		JsonTableParseHelper::SharedData* sharedData = data.As<JsonTableParseHelper::SharedData>();

		if (sharedData == nullptr)
		{
			return false;
		}

		if (key == "Type")
		{
			assert(mStack.IsEmpty() == false);
			StackFrame& sf = mStack.Top();
			sf.type = Datum::StringDatumTypeHashMap.At(value.asString());
			Datum& datum = sf.scope->Append(sf.key);
			datum.SetType(sf.type);
		}
		else if (key == "Class")
		{
			assert(mStack.IsEmpty() == false);
			StackFrame& sf = mStack.Top();
			sf.classname = value.asString();
		}
		else if (key == "Value")
		{
			assert(mStack.IsEmpty() == false);
			StackFrame& sf = mStack.Top();

			if (sf.type == Datum::DatumType::TABLE)
			{
				if (value.size() > 0)
				{
					Scope* nestedScope;
					Datum* nestedDatum = sf.scope->Find(sf.key);

					if (nestedDatum != nullptr && nestedDatum->Size() > index)
					{
						nestedScope = &(*nestedDatum)[index];
					}
					else if (sf.classname.empty() == false)
					{
						nestedScope = Factory<Scope>::Create(sf.classname);
						assert(nestedScope != nullptr);
						sf.scope->Adopt(*nestedScope, sf.key);
					}
					else
					{
						nestedScope = &(sf.scope->AppendScope(sf.key));
					}

					if (IsArrayElement)
					{
						mStack.Push({ key,"", Datum::DatumType::TABLE, nestedScope });
					}
					else
					{
						sf.scope = nestedScope;
					}
				}
				
			}
			else
			{
				Datum& datum = sf.scope->At(sf.key);

				if (!datum.IsExternal() && index >= datum.Size())
				{
					datum.Resize(datum.Size() + 1);
				}
				
				switch (datum.Type())
				{
				case Datum::DatumType::INTEGER:
					datum.Set(value.asInt(), index);
					break;
				case Datum::DatumType::FLOAT:
					datum.Set(value.asFloat(), index);
					break;
				case Datum::DatumType::STRING:
					datum.Set(value.asString(), index);
					break;
				case Datum::DatumType::VECTOR4: //same as matrix, so let it fall through
				case Datum::DatumType::MATRIX4X4:
					datum.SetFromString(value.asString(), index);
					break;
				}
			}
		}
		else
		{
			Scope* scope = mStack.IsEmpty() ? sharedData->mSharedScope : mStack.Top().scope;
			assert(scope != nullptr);
			mStack.Push({ key,"", Datum::DatumType::UNKNOWN, scope });
		}
		return true;
	}

	bool JsonTableParseHelper::EndHandler(JsonParseMaster::SharedData& data, const std::string& key)
	{
		JsonTableParseHelper::SharedData* sharedData = data.As<JsonTableParseHelper::SharedData>();

		if (sharedData == nullptr)
		{
			return false;
		}

		auto& sf = mStack.Top();
		if (key == sf.key)
		{
			mStack.Pop();
		}
		return true;
	}

#pragma endregion

}