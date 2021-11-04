#pragma once
#include "JsonParseMaster.h"

namespace Library
{
	/// <summary>
	/// Abstract base class from which all helpers are derived
	/// </summary>
	class IJsonParseHelper : public RTTI
	{
		RTTI_DECLARATIONS(IJsonParseHelper, RTTI)
	
	public:

		/// <summary>
		/// Default constructor for IJsonParseHelper
		/// </summary>
		IJsonParseHelper() = default;

		/// <summary>
		/// Copy constructor for IJsonParseHelpr
		/// </summary>
		/// <param name="rhs">Const reference to IJsonParseHelper</param>
		IJsonParseHelper(const IJsonParseHelper& rhs) = default;

		/// <summary>
		/// Move constructor for IJsonParseHelper
		/// </summary>
		/// <param name="rhs">R-value reference of IJsonParseHelper</param>
		IJsonParseHelper(IJsonParseHelper&& rhs) = default;

		/// <summary>
		/// Copy assignment operator for IJsonParseHelper
		/// </summary>
		/// <param name="rhs">Const reference to IJsonParseHelper</param>
		/// <returns>Reference to IJsonParseHelper</returns>
		IJsonParseHelper& operator=(const IJsonParseHelper& rhs) = default;

		/// <summary>
		/// Move assignment operator for IJsonParseHelper
		/// </summary>
		/// <param name="rhs">R-value reference of IJsonParseHelper</param>
		/// <returns>Reference to IJsonParseHelper</returns>
		IJsonParseHelper& operator=(IJsonParseHelper && rhs) = default;

		/// <summary>
		/// Virtual destructor for IJsonParseHelper
		/// </summary>
		virtual ~IJsonParseHelper() = default;

		/// <summary>
		/// Initializer for IJsonParseHelper
		/// </summary>
		virtual void Initialize();

		/// <summary>
		/// Checks if a helper can handle provided key/value pair
		/// </summary>
		/// <param name="sharedData">Shared data reference</param>
		/// <param name="key">Const reference to key</param>
		/// <param name="value">JSON value associated with key</param>
		/// <param name="IsArrayElement">Boolean indicating whether or not value is in an array</param>
		/// <returns>True if can be handled, false if not</returns>
		virtual bool StartHandler(JsonParseMaster::SharedData& sharedData, const std::string& key, const Json::Value& value, bool IsArrayElement, size_t index, size_t arraySize) = 0;

		/// <summary>
		/// Attempts to complete handling of element pair
		/// </summary>
		/// <param name="sharedData">Shared data reference</param>
		/// <param name="key">Const reference to key</param>
		/// <returns>True if can be handled, false if not</returns>
		virtual bool EndHandler(JsonParseMaster::SharedData& sharedData, const std::string& key) = 0;

		/// <summary>
		/// Overriden implementation that creates an instance of a helper
		/// </summary>
		/// <returns>Pointer to IJsonParseHelper</returns>
		virtual gsl::owner<IJsonParseHelper*> Create() const = 0;
	};
}



