#pragma once
#include <string>
#include <json/json.h>
#include <gsl/gsl>
#include "RTTI.h"
#include "Vector.h"

namespace Library
{
	/// <summary>
	/// Forward declaration of IJsonParseHelper
	/// </summary>
	class IJsonParseHelper;

	/// <summary>
	/// Wraps JsonCpp functionality through C++
	/// </summary>
	class JsonParseMaster final
	{
	public:

		/// <summary>
		/// Stores data that all helpers share with each other and parse master
		/// </summary>
		class SharedData : public RTTI
		{
			/// <summary>
			/// Friend declaration, allowing JsonParseMaster access to private members of SharedData
			/// </summary>
			friend class JsonParseMaster;

			RTTI_DECLARATIONS(JsonParseMaster::SharedData, RTTI)


		public:

			/// <summary>
			/// Default constructor for SharedData
			/// </summary>
			SharedData() = default;

			/// <summary>
			/// Virtual destructor for SharedData
			/// </summary>
			virtual ~SharedData() = default;

			/// <summary>
			/// Overriden implementation that creates an instance of SharedData
			/// </summary>
			/// <returns>Pointer to SharedDAta</returns>
			virtual gsl::owner<SharedData*> Create() const = 0;

			/// <summary>
			/// Gets JsonParseMaster 
			/// </summary>
			/// <returns>Pointer to JsonParseMAster</returns>
			JsonParseMaster* GetJsonParseMaster();

			/// <summary>
			/// Gets current nesting depth
			/// </summary>
			/// <returns></returns>
			size_t Depth() const;

			/// <summary>
			/// Initializes SharedData
			/// </summary>
			virtual void Initialize() = 0;

		protected:

			size_t depth = 0;

		private:

			JsonParseMaster* mParseMaster = nullptr;
			void SetJsonParseMaster(JsonParseMaster& parseMaster);

			/// <summary>
			/// IIncrements nesting depth counter
			/// </summary>
			void IncrementDepth();

			/// <summary>
			/// Decrements nesting depth counter
			/// </summary>
			void DecrementDepth();
		};

		/// <summary>
		/// Explicit constructor that takes in a reference to SharedData
		/// </summary>
		/// <param name="data">Reference to SharedData</param>
		explicit JsonParseMaster(SharedData& data);

		/// <summary>
		/// Destructor for JsonParseMaster
		/// </summary>
		~JsonParseMaster();

		/// <summary>
		/// Deleted copy constructor and assignment operator
		/// </summary>
		/// <param name="rhs"></param>
		JsonParseMaster(const JsonParseMaster& rhs) = delete;
		JsonParseMaster& operator=(const JsonParseMaster& rhs) = delete;

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="rhs">R-value reference to JsonParseMaster</param>
		JsonParseMaster(JsonParseMaster&& rhs) noexcept;

		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="rhs">R-value reference to JsonParseMaster</param>
		/// <returns>JsonParseMaster reference</returns>
		JsonParseMaster& operator= (JsonParseMaster&& rhs) noexcept;

		/// <summary>
		/// Duplicates JsonParseMaster object
		/// </summary>
		/// <returns>Pointer to JsonParseMaster</returns>
		gsl::owner<JsonParseMaster*> Clone();

		/// <summary>
		/// Adds helper to the list of helper
		/// </summary>
		/// <param name="helper">Reference to helper that is to be added</param>
		void AddHelper(IJsonParseHelper& helper);

		/// <summary>
		/// Removes helper from the list
		/// </summary>
		/// <param name="helper">Reference to helper that is to be removed</param>
		/// <returns>True if removed, false if not</returns>
		bool RemoveHelper(IJsonParseHelper& helper);

		/// <summary>
		/// Parses a string of JSON data
		/// </summary>
		/// <param name="jsonString">Const reference to std::string of JSON data</param>
		void Parse(const std::string& jsonString);

		/// <summary>
		/// Parses an input stream of JSON data
		/// </summary>
		/// <param name="jsonStream">Input stream of JSON data</param>
		void Parse(std::istream& jsonStream);

		/// <summary>
		/// Parses a JSON file
		/// </summary>
		/// <param name="filename">File name of JSON Data</param>
		void ParseFromFile(const std::string& filename);

		/// <summary>
		/// Retrieves file name of JSON data
		/// </summary>
		/// <returns>Const reference to std::string of file name</returns>
		const std::string& GetFileName() const;

		/// <summary>
		/// Gets SharedData associated with this JsonParseMaster
		/// </summary>
		/// <returns>Pointer to shared data</returns>
		SharedData* GetSharedData();

		/// <summary>
		/// Sets SharedData
		/// </summary>
		/// <param name="data">Reference to shared data</param>
		void SetSharedData(SharedData& data);

		/// <summary>
		/// Checks whether JsonParseMaster is a clone or not
		/// </summary>
		/// <returns>True if a clone, false if not</returns>
		bool IsClone() const;

		/// <summary>
		/// Gets list of helpers
		/// </summary>
		/// <returns>Const reference to a vector of IJsonParseHelper pointers</returns>
		const Vector<IJsonParseHelper*>& GetParseHelpers() const;

		/// <summary>
		/// Initializes JsonParseMaster and SharedData
		/// </summary>
		void Initialize();

	private:

		SharedData* mSharedData = nullptr;
		Vector<IJsonParseHelper*> mParseHelpers;

		bool mIsClone = false;
		std::string mFileName;

		void Parse(const Json::Value& value, bool IsArrayElement = false, size_t index = 0);
		void ParseKeyValuePair(const std::string& key, const Json::Value& value, bool IsArrayElement = false, size_t index = 0, size_t arraySize = 0);

	};
}