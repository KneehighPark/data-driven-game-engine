#pragma once
#include "Stack.h"
#include "Scope.h"
#include "IJsonParseHelper.h"

namespace Library
{
	/// <summary>
	/// Table Parse Helper Class - Parses Scopes and Attributed Objects stored as JSON Data
	/// </summary>
	class JsonTableParseHelper final : public IJsonParseHelper
	{
	public:

		/// <summary>
		/// Embedded SharedData Class
		/// </summary>
		class SharedData : public JsonParseMaster::SharedData
		{
			RTTI_DECLARATIONS(SharedData, JsonParseMaster::SharedData)
			
			/// <summary>
			/// Allows JsonTableParseHelper access to private members of SharedData class
			/// </summary>
			friend class JsonTableParseHelper;

		public:

			/// <summary>
			/// Parameterized constructor for Shared Data
			/// </summary>
			/// <param name="scope">Reference to passed Scope</param>
			explicit SharedData(Scope& scope);

			/// <summary>
			/// Default destructor for SharedData
			/// </summary>
			~SharedData() = default;

			/// <summary>
			/// Duplicates a SharedData object
			/// </summary>
			/// <returns>Returns pointer to a JsonParseMaster</returns>
			virtual gsl::owner<SharedData*> Create() const override;

			/// <summary>
			/// SharedData getter 
			/// </summary>
			/// <returns>Scope pointer stored within SharedData</returns>
			Scope* GetSharedData();

			/// <summary>
			/// SharedData setter
			/// </summary>
			/// <param name="rhs">Reference to Scope that will be set</param>
			void SetSharedData(Scope& rhs);

			/// <summary>
			/// Overrided initializer for SharedData
			/// </summary>
			virtual void Initialize() override;

		private:
			Scope* mSharedScope = nullptr;
		};
		/// <summary>
		/// Defaulted constructor
		/// </summary>
		JsonTableParseHelper() = default;

		/// <summary>
		/// Defaulted destructor
		/// </summary>
		virtual ~JsonTableParseHelper() = default;

		/// <summary>
		/// Defaulted copy constructor
		/// </summary>
		/// <param name="rhs">Const reference to JsonTableParseHelper</param>
		JsonTableParseHelper(const JsonTableParseHelper& rhs) = default;

		/// <summary>
		/// Defaulted move constructor
		/// </summary>
		/// <param name="rhs">R-value reference to JsonTableParseHelper</param>
		JsonTableParseHelper(JsonTableParseHelper&& rhs) = default;

		/// <summary>
		/// Defaulted copy assignment operator
		/// </summary>
		/// <param name="rhs">Const reference to JsonTableParseHelper</param>
		/// <returns>Reference to JsonTableParseHelper</returns>
		JsonTableParseHelper& operator=(const JsonTableParseHelper& rhs) = default;

		/// <summary>
		/// Defaulted move assignment operator
		/// </summary>
		/// <param name="rhs">R-value reference to JsonTableParseHelper</param>
		/// <returns>Reference to JsonTableParseHelper</returns>
		JsonTableParseHelper& operator=(JsonTableParseHelper&& rhs) = default;

		/// <summary>
		/// Determines if and how a key/value pair can be handled at the start of the parsing process
		/// </summary>
		/// <param name="sharedData">SharedData reference</param>
		/// <param name="key">Const reference to a key of type string</param>
		/// <param name="value">Json value associated with the aforementioned key</param>
		/// <param name="IsArrayElement">Boolean value that indicates whether the value is in an array or not</param>
		/// <param name="index">Index of an array, if applicable</param>
		/// <param name="arraySize">Size of an array, if applicable</param>
		/// <returns>True if it can be handled, false if not</returns>
		virtual bool StartHandler(JsonParseMaster::SharedData& sharedData, const std::string& key, const Json::Value& value, bool IsArrayElement, size_t index, size_t arraySize) override;
		
		/// <summary>
		/// Determines if and how a key/value pair can be handled at the end of the parsing process
		/// </summary>
		/// <param name="sharedData">SharedData reference</param>
		/// <param name="key">Const reference to a key of type string</param>
		/// <returns>True if it can be handled, false if not</returns>
		virtual bool EndHandler(JsonParseMaster::SharedData& sharedData, const std::string& key) override;
		
		/// <summary>
		/// Creates a clone of a JsonTableParseHelper
		/// </summary>
		/// <returns>Pointer to a IJsonParseHelper</returns>
		virtual gsl::owner<IJsonParseHelper*> Create() const override;

		/// <summary>
		/// Initialize method for JsonTableParseHelper
		/// </summary>
		virtual void Initialize() override;

		/// <summary>
		/// Fetches the size of the stack used to store keys
		/// </summary>
		/// <returns>Size of the stack</returns>
		size_t SizeOfStack() const;

	private:

		struct StackFrame
		{
			const std::string key;
			std::string classname;
			Datum::DatumType type = Datum::DatumType::UNKNOWN;
			Scope* scope = nullptr;
		};

		Stack<StackFrame> mStack;
	};
}