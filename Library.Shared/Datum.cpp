#include "pch.h"
#include "Datum.h"

namespace Library
{
	const HashMap<std::string, Datum::DatumType> Datum::StringDatumTypeHashMap
	{
		{ "unknown", Datum::DatumType::UNKNOWN },
		{ "integer", Datum::DatumType::INTEGER },
		{ "float", Datum::DatumType::FLOAT },
		{ "string", Datum::DatumType::STRING },
		{ "matrix4x4", Datum::DatumType::MATRIX4X4 },
		{ "vector4", Datum::DatumType::VECTOR4 },
		{ "table", Datum::DatumType::TABLE }
	};

	const int8_t Datum::DatumTypeSizes[] =
	{
		0,
		sizeof(int32_t),
		sizeof(float_t),
		sizeof(glm::vec4),
		sizeof(glm::mat4x4),
		sizeof(std::string),
		sizeof(RTTI*),
		sizeof(Scope*)
	};

	Datum::Datum()
	{
		//Default constructor
	}

	Datum::~Datum()
	{
		Clear();
	}

	Datum::Datum(const Datum& rhs)
	{
		operator=(rhs);
	}

	Datum::Datum(Datum&& rhs) : mData(rhs.mData), mType(rhs.mType), mSize(rhs.mSize), mCapacity(rhs.mCapacity), mIsExternal(rhs.mIsExternal)
	{
		rhs.mData.mInt = nullptr;
		rhs.mType = DatumType::UNKNOWN;
		rhs.mSize = 0;
		rhs.mCapacity = 0;
		rhs.mIsExternal = false;
	}



	Datum& Datum::operator=(Datum&& rhs)
	{
		if (this != &rhs)
		{
			Clear();

			mData = rhs.mData;
			mType = rhs.mType;
			mSize = rhs.mSize;
			mCapacity = rhs.mCapacity;
			mIsExternal = rhs.mIsExternal;

			rhs.mData.mInt = nullptr;
			rhs.mType = DatumType::UNKNOWN;
			rhs.mSize = 0;
			rhs.mCapacity = 0;
			mIsExternal = false;
		}
		return *this;
	}

	Datum& Datum::operator= (const Datum& rhs)
	{
		if (this != &rhs)
		{
			Clear();

			mType = rhs.mType;
			mIsExternal = rhs.mIsExternal;

			if (IsExternal())
			{
				mData = rhs.mData;
			}
			else
			{
				Reserve(rhs.mCapacity);
				if (mType == DatumType::STRING)
				{
					for (size_t i = 0; i < rhs.mSize; ++i)
					{
						new (mData.mString + i)std::string(rhs.mData.mString[i]);
					}
				}
				else
				{
					memcpy(mData.vp, rhs.mData.vp, rhs.mSize * DatumTypeSizes[static_cast<std::size_t>(mType)]);
				}
			}

			mSize = rhs.mSize;
			mCapacity = rhs.mCapacity;
		}
		return *this;
	}

		Datum& Datum::operator=(const int32_t & rhs)
		{
			PushBack(rhs);
			return *this;
		}

		Datum& Datum::operator=(const float_t & rhs)
		{
			PushBack(rhs);
			return *this;
		}

		Datum& Datum::operator=(const glm::vec4 & rhs)
		{
			PushBack(rhs);
			return *this;
		}

		Datum& Datum::operator=(const glm::mat4x4 & rhs)
		{
			PushBack(rhs);
			return *this;
		}

		Datum& Datum::operator=(const std::string & rhs)
		{
			PushBack(rhs);
			return *this;
		}

		Datum& Datum::operator=(RTTI* rhs)
		{
			PushBack(rhs);
			return *this;
		}

		Datum& Datum::operator=(Scope& rhs)
		{
			PushBack(rhs);
			return *this;
		}

		Datum::DatumType Datum::Type() const
		{
			return mType;
		}

		void Datum::SetType(DatumType type)
		{
			if (mType != type && mType != DatumType::UNKNOWN)
			{
				throw std::runtime_error("Datum's data type cannot be changed once assigned!");
			}
			mType = type;
		}

		size_t Datum::Size() const
		{
			return mSize;
		}

		size_t Datum::Capacity() const
		{
			return mCapacity;
		}

		void Datum::Resize(size_t newSize)
		{
			if (mIsExternal)
			{
				throw std::runtime_error("Cannot set size for datum with external storage!");
			}
			if (mType == DatumType::UNKNOWN)
			{
				throw std::runtime_error("Cannot set size for datum with unknown type!");
			}

			if (mCapacity < newSize)
			{
				Reserve(newSize);
			}

			while (mSize < newSize)
			{
				switch (mType)
				{
					case DatumType::INTEGER:
						new (mData.mInt + mSize)int32_t();
						mSize++;
						break;
					case DatumType::FLOAT:
						new (mData.mFloat + mSize)float_t();
						mSize++;
						break;
					case DatumType::VECTOR4:
						new (mData.mVec4 + mSize)glm::vec4();
						mSize++;
						break;
					case DatumType::MATRIX4X4:
						new (mData.mMat4x4 + mSize)glm::mat4x4();
						mSize++;
						break;
					case DatumType::STRING:
						new (mData.mString + mSize)std::string();
						mSize++;
						break;
					case DatumType::POINTER:
						new (mData.mRTTI + mSize)RTTI*();
						mSize++;
						break;
					case DatumType::TABLE:
						new (mData.mScope + mSize)Scope* ();
						mSize++;
						break;
					default:
						throw std::runtime_error("Cannot set size for datum with unknown type!");
				}
			}

			while (mSize > newSize)
			{
				switch (mType)
				{
				case DatumType::INTEGER:
					mSize--;
					mData.mInt[mSize].std::int32_t::~int32_t();
					break;
				case DatumType::FLOAT:
					mSize--;
					mData.mFloat[mSize].std::float_t::~float_t();
					break;
				case DatumType::VECTOR4:
					mSize--;
					mData.mVec4[mSize].glm::vec4::~vec4();
					break;
				case DatumType::MATRIX4X4:
					mSize--;
					mData.mMat4x4[mSize].glm::mat4x4::~mat4x4();
					break;
				case DatumType::STRING:
					mSize--;
					mData.mString[mSize].std::string::~string();
					break;
				case DatumType::POINTER:
					mSize--;
					new(mData.mRTTI + mSize)RTTI* (nullptr);
					break;
				case DatumType::TABLE:
					mSize--;
					new(mData.mScope + mSize)Scope* (nullptr);
				default:
					throw std::runtime_error("Cannot set size for datum with unknown type!");
				}
			}
			mCapacity = mSize;
		}

		void Datum::Reserve(size_t newCapacity)
		{
			if (mIsExternal)
			{
				throw std::runtime_error("Cannot reserve memory for datum with external storage!");
			}

			if (mCapacity < newCapacity)
			{
				switch (mType)
				{
				case DatumType::INTEGER:
					mData.mInt = reinterpret_cast<int32_t*>(realloc(mData.mInt, newCapacity * sizeof(int32_t)));
					break;

				case DatumType::FLOAT:
					mData.mFloat = reinterpret_cast<float_t*>(realloc(mData.mFloat, newCapacity * sizeof(float_t)));
					break;

				case DatumType::VECTOR4:
					mData.mVec4 = reinterpret_cast<glm::vec4*>(realloc(mData.mVec4, newCapacity * sizeof(glm::vec4)));
					break;

				case DatumType::MATRIX4X4:
					mData.mMat4x4 = reinterpret_cast<glm::mat4x4*>(realloc(mData.mMat4x4, newCapacity * sizeof(glm::mat4x4)));
					break;

				case DatumType::STRING:
					mData.mString = reinterpret_cast<std::string*>(realloc(mData.mString, newCapacity * sizeof(std::string)));
					break;

				case DatumType::POINTER:
					mData.mRTTI = reinterpret_cast<RTTI**>(realloc(mData.mRTTI, newCapacity * sizeof(RTTI*)));
					break;
				case DatumType::TABLE:
					mData.mScope = reinterpret_cast<Scope**>(realloc(mData.mScope, newCapacity * sizeof(Scope*)));
					break;
					
				default:
					break;
				}
				mCapacity = newCapacity;
			}
		}

		void Datum::Clear()
		{
			if (mCapacity > 0 && !IsExternal())
			{
				if (mType == DatumType::STRING)
				{
					for (size_t i = 0; i < mSize; ++i)
					{
						mData.mString[i].std::string::~string();
					}
				}
				free(mData.vp);
				mSize = 0;
				mCapacity = 0;
			}
		}
		bool Datum::IsExternal() const
		{
			return mIsExternal;
		}

		void Datum::SetStorage(const DatumType & type, size_t arraySize)
		{
			if (mType != type && mType != DatumType::UNKNOWN)
			{
				throw std::runtime_error("Cannot perform operation on two different datum types!");
			}
			if (!mIsExternal)
			{
				Clear();
			}
			mType = type;
			mSize = arraySize;
			mCapacity = arraySize;
			mIsExternal = true;
		}

		void Datum::SetStorage(int32_t* array, size_t arraySize)
		{
			SetStorage(DatumType::INTEGER, arraySize);
			mData.mInt = array;
		}

		void Datum::SetStorage(float_t* array, size_t arraySize)
		{
			SetStorage(DatumType::FLOAT, arraySize);
			mData.mFloat = array;
		}

		void Datum::SetStorage(glm::vec4* array, size_t arraySize)
		{
			SetStorage(DatumType::VECTOR4, arraySize);
			mData.mVec4 = array;
		}

		void Datum::SetStorage(glm::mat4x4* array, size_t arraySize)
		{
			SetStorage(DatumType::MATRIX4X4, arraySize);
			mData.mMat4x4 = array;
		}

		void Datum::SetStorage(std::string* array, size_t arraySize)
		{
			SetStorage(DatumType::STRING, arraySize);
			mData.mString = array;
		}

		void Datum::SetStorage(RTTI** array, size_t arraySize)
		{
			SetStorage(DatumType::POINTER, arraySize);
			mData.mRTTI = array;
		}

		void Datum::SetStorage(void* array, size_t arraySize)
		{
			mIsExternal = true;
			mSize = arraySize;
			mCapacity = arraySize;
			mData.vp = array;
		}

		bool Datum::operator==(const Datum& rhs) const
		{
			bool equal = true; 
			if ((mType == rhs.mType) && (mSize == rhs.mSize))
			{
				switch (mType)
				{
				case DatumType::STRING:
					for (size_t i = 0; i < mSize; ++i)
					{
						if (mData.mString[i] != rhs.mData.mString[i])
						{
							equal = false;
							break;
						}
					}
					break;
				case DatumType::TABLE:
				case DatumType::POINTER:
					for (size_t i = 0; i < mSize; ++i)
					{
						RTTI* leftPointer = mData.mRTTI[i];
						RTTI* rightPointer = mData.mRTTI[i];
						if (leftPointer != rightPointer && leftPointer != nullptr)
						{
							if (leftPointer->Equals(rightPointer) == false)
							{
								equal = false;
								break;
							}
						}
					}
					break;
				default:
					size_t typeSize = DatumTypeSizes[static_cast<std::size_t>(mType)] * mSize;
					if (memcmp(mData.vp, rhs.mData.vp, typeSize) != 0)
					{
						equal = false;
					}
					break;
				}
			}
			else
			{
				equal = false;
			}
			return equal;
		}

		bool Datum::operator==(const int32_t & rhs) const
		{
			return ((mSize > 0) && (mType == DatumType::INTEGER) && (*mData.mInt == rhs));
		}

		bool Datum::operator==(const float_t & rhs) const
		{
			return ((mSize > 0) && (mType == DatumType::FLOAT) && (*mData.mFloat == rhs));
		}

		bool Datum::operator==(const glm::vec4 & rhs) const
		{
			return ((mSize > 0) && (mType == DatumType::VECTOR4) && (*mData.mVec4 == rhs));
		}

		bool Datum::operator==(const glm::mat4x4 & rhs) const
		{
			return ((mSize > 0) && (mType == DatumType::MATRIX4X4) && (*mData.mMat4x4 == rhs));
		}

		bool Datum::operator==(const std::string & rhs) const
		{
			return ((mSize > 0) && (mType == DatumType::STRING) && (rhs.compare(*mData.mString) == 0));
		}

		bool Datum::operator==(const RTTI * rhs) const
		{
			return ((mSize > 0) && (mType == DatumType::POINTER) && ((**mData.mRTTI).Equals(rhs)));
		}

		bool Datum::operator!=(const Datum & rhs) const
		{
			return !(operator==(rhs));
		}

		bool Datum::operator!=(const int32_t & rhs) const
		{
			return !(operator==(rhs));
		}

		bool Datum::operator!=(const float_t & rhs) const
		{
			return !(operator==(rhs));
		}

		bool Datum::operator!=(const glm::vec4 & rhs) const
		{
			return !(operator==(rhs));
		}

		bool Datum::operator!=(const glm::mat4x4 & rhs) const
		{
			return !(operator==(rhs));
		}

		bool Datum::operator!=(const std::string & rhs) const
		{
			return !(operator==(rhs));
		}

		bool Datum::operator!=(const RTTI * rhs) const
		{
			return !(operator==(rhs));
		}

		void Datum::Set(const DatumType & type, size_t index)
		{
			if (mType == DatumType::UNKNOWN)
			{
				mType = type;
				mIsExternal = false;
			}

			if (mType != type)
			{
				throw std::runtime_error("Datum type mismatch!");
			}

			if (index >= mSize)
			{
				throw std::exception("Index out of bounds!");
			}
		}

		void Datum::Set(const int32_t& value, size_t index)
		{
			Set(DatumType::INTEGER, index);
			mData.mInt[index] = value;
		}

		void Datum::Set(const float_t& value, size_t index)
		{
			Set(DatumType::FLOAT, index);
			mData.mFloat[index] = value;
		}

		void Datum::Set(const glm::vec4& value, size_t index)
		{
			Set(DatumType::VECTOR4, index);
			mData.mVec4[index] = value;
		}

		void Datum::Set(const glm::mat4x4& value, size_t index)
		{
			Set(DatumType::MATRIX4X4, index);
			mData.mMat4x4[index] = value;
		}

		void Datum::Set(const std::string& value, size_t index)
		{
			Set(DatumType::STRING, index);
			mData.mString[index] = value;
		}

		void Datum::Set(RTTI* value, size_t index)
		{
			Set(DatumType::POINTER, index);
			mData.mRTTI[index] = value;
		}

		void Datum::Set(Scope& value, size_t index)
		{
			Set(DatumType::TABLE, index);
			mData.mScope[index] = &value;
		}

		template<>
		int32_t& Datum::Get(size_t index)
		{
			if (index >= mSize)
			{
				throw std::runtime_error("Index out of bounds!");
			}
			if (mType != DatumType::INTEGER)
			{
				throw std::runtime_error("Type mismatch!");
			}
			return mData.mInt[index];
		}

		template<>
		float_t& Datum::Get(size_t index)
		{
			if (index >= mSize)
			{
				throw std::runtime_error("Index out of bounds!");
			}
			if (mType != DatumType::FLOAT)
			{
				throw std::runtime_error("Type mismatch!");
			}
			return mData.mFloat[index];
		}

		template<>
		glm::vec4& Datum::Get(size_t index)
		{
			if (index >= mSize)
			{
				throw std::runtime_error("Index out of bounds!");
			}
			if (mType != DatumType::VECTOR4)
			{
				throw std::runtime_error("Type mismatch!");
			}
			return mData.mVec4[index];
		}

		template<>
		glm::mat4x4& Datum::Get(size_t index)
		{
			if (index >= mSize)
			{
				throw std::runtime_error("Index out of bounds!");
			}
			if (mType != DatumType::MATRIX4X4)
			{
				throw std::runtime_error("Type mismatch!");
			}
			return mData.mMat4x4[index];
		}

		template<>
		std::string& Datum::Get(size_t index)
		{
			if (index >= mSize)
			{
				throw std::runtime_error("Index out of bounds!");
			}
			if (mType != DatumType::STRING)
			{
				throw std::runtime_error("Type mismatch!");
			}
			return mData.mString[index];
		}

		template<>
		RTTI*& Datum::Get(size_t index)
		{
			if (index >= mSize)
			{
				throw std::runtime_error("Index out of bounds!");
			}
			if (mType != DatumType::POINTER)
			{
				throw std::runtime_error("Type mismatch!");
			}
			return mData.mRTTI[index];
		}

		template<>
		Scope& Datum::Get<Scope>(size_t index)
		{
			if (index >= mSize)
			{
				throw std::runtime_error("Index out of bounds!");
			}
			if (mType != DatumType::TABLE)
			{
				throw std::runtime_error("Type mismatch!");
			}
			return *mData.mScope[index];
		}

		template<>
		const int32_t& Datum::Get(size_t index) const
		{
			if (index >= mSize)
			{
				throw std::runtime_error("Index out of bounds!");
			}
			if (mType != DatumType::INTEGER)
			{
				throw std::runtime_error("Type mismatch!");
			}
			return mData.mInt[index];
		}

		template<>
		const float_t& Datum::Get(size_t index) const
		{
			if (index >= mSize)
			{
				throw std::runtime_error("Index out of bounds!");
			}
			if (mType != DatumType::FLOAT)
			{
				throw std::runtime_error("Type mismatch!");
			}
			return mData.mFloat[index];
		}

		template<>
		const glm::vec4& Datum::Get(size_t index) const
		{
			if (index >= mSize)
			{
				throw std::runtime_error("Index out of bounds!");
			}
			if (mType != DatumType::VECTOR4)
			{
				throw std::runtime_error("Type mismatch!");
			}
			return mData.mVec4[index];
		}

		template<>
		const glm::mat4x4& Datum::Get(size_t index) const
		{
			if (index >= mSize)
			{
				throw std::runtime_error("Index out of bounds!");
			}
			if (mType != DatumType::MATRIX4X4)
			{
				throw std::runtime_error("Type mismatch!");
			}
			return mData.mMat4x4[index];
		}

		template<>
		const std::string& Datum::Get(size_t index) const
		{
			if (index >= mSize)
			{
				throw std::runtime_error("Index out of bounds!");
			}
			if (mType != DatumType::STRING)
			{
				throw std::runtime_error("Type mismatch!");
			}
			return mData.mString[index];
		}

		template<>
		RTTI* const& Datum::Get(size_t index) const
		{
			if (index >= mSize)
			{
				throw std::runtime_error("Index out of bounds!");
			}
			if (mType != DatumType::POINTER)
			{
				throw std::runtime_error("Type mismatch!");
			}
			return mData.mRTTI[index];
		}

		template<>
		Scope const& Datum::Get<Scope>(size_t index) const 
		{
			if (index >= mSize)
			{
				throw std::runtime_error("Index out of bounds!");
			}
			if (mType != DatumType::TABLE)
			{
				throw std::runtime_error("Type mismatch!");
			}
			return *mData.mScope[index];
		}

		void Datum::SetFromString(const std::string& string, size_t index)
		{
			if (mType == DatumType::UNKNOWN)
			{
				throw std::runtime_error("Datum type is not set!");
			}
			if (index >= mSize)
			{
				throw std::runtime_error("Index is out of bounds!");

			}

			std::stringstream input_string(string);

			switch (mType)
			{
			case DatumType::INTEGER:
				int32_t temp_int;
				input_string >> temp_int;
				Set(temp_int, index);
				break;
			case DatumType::FLOAT:
				float_t temp_float;
				input_string >> temp_float;
				Set(temp_float, index);
				break;
			case DatumType::VECTOR4:
				glm::vec4 temp_vec;
				sscanf_s(string.c_str(), "vec4(%f,%f,%f,%f)", &temp_vec.x, &temp_vec.y, &temp_vec.z, &temp_vec.w);
				Set(temp_vec, index);
				break;
			case DatumType::MATRIX4X4:
				glm::mat4 temp_mat;
				sscanf_s(string.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
					&temp_mat[0][0], &temp_mat[0][1], &temp_mat[0][2], &temp_mat[0][3],
					&temp_mat[1][0], &temp_mat[1][1], &temp_mat[1][2], &temp_mat[1][3],
					&temp_mat[2][0], &temp_mat[2][1], &temp_mat[2][2], &temp_mat[2][3],
					&temp_mat[3][0], &temp_mat[3][1], &temp_mat[3][2], &temp_mat[3][3]);
				Set(temp_mat, index);
				break;
			case DatumType::STRING:
				Set(string, index);
				break;
			case DatumType::POINTER:
				throw std::runtime_error("Cannot set RTTI pointer from a string");
				break;
			case DatumType::TABLE:
				throw std::runtime_error("Cannot set Scope pointer from a string");
			default:
				throw std::runtime_error("Invalid data type!");

			}
		}

		std::string Datum::ToString(size_t index)
		{
			if (mType == DatumType::UNKNOWN)
			{
				throw std::runtime_error("Invalid datum type!");
			}
			if (index >= mSize)
			{
				throw std::runtime_error("Index is out of bounds!");
			}
			
			std::string output;

			switch (mType)
			{
			case DatumType::INTEGER:
				output = std::to_string(Get<int32_t>(index));
				break;
			case DatumType::FLOAT:
				output = std::to_string(Get<float_t>(index));
				break;
			case DatumType::VECTOR4:
				output = glm::to_string(Get<glm::vec4>(index));
				break;
			case DatumType::MATRIX4X4:
				output = glm::to_string(Get<glm::mat4x4>(index));
				break;
			case DatumType::STRING:
				output = Get<std::string>(index);
				break;
			case DatumType::POINTER:
				if (mData.mRTTI[index] != nullptr)
					output = (Get<RTTI*>(index))->ToString();
				break;
			case DatumType::TABLE:
				output = "Scope";
			default:
				throw std::exception("Invalid data type!");
			}
			return output;
		}

		void Datum::PushBack(const int32_t & item)
		{
			if (mIsExternal)
			{
				throw std::runtime_error("Cannot write to external storage!");
			}

			if (mType == DatumType::UNKNOWN)
			{
				mType = DatumType::INTEGER;
			}

			if (mType != DatumType::INTEGER)
			{
				throw std::runtime_error("Datum type mismatch!");
			}

			if (mCapacity == mSize)
			{
				Reserve(mCapacity * 2 + 1);
			}

			mData.mInt[mSize] = item;
			mSize++;
		}

		void Datum::PushBack(const float_t & item)
		{
			if (mIsExternal)
			{
				throw std::runtime_error("Cannot write to external storage!");
			}

			if (mType == DatumType::UNKNOWN)
			{
				mType = DatumType::FLOAT;
			}

			if (mType != DatumType::FLOAT)
			{
				throw std::runtime_error("Datum type mismatch!");
			}

			if (mCapacity == mSize)
			{
				Reserve(mCapacity * 2 + 1);
			}

			mData.mFloat[mSize] = item;
			mSize++;
		}

		void Datum::PushBack(const glm::vec4 & item)
		{
			if (mIsExternal)
			{
				throw std::runtime_error("Cannot write to external storage!");
			}

			if (mType == DatumType::UNKNOWN)
			{
				mType = DatumType::VECTOR4;
			}

			if (mType != DatumType::VECTOR4)
			{
				throw std::runtime_error("Datum type mismatch!");
			}

			if (mCapacity == mSize)
			{
				Reserve(mCapacity * 2 + 1);
			}

			mData.mVec4[mSize] = item;
			mSize++;
		}

		void Datum::PushBack(const glm::mat4x4 & item)
		{
			if (mIsExternal)
			{
				throw std::runtime_error("Cannot write to external storage!");
			}

			if (mType == DatumType::UNKNOWN)
			{
				mType = DatumType::MATRIX4X4;
			}

			if (mType != DatumType::MATRIX4X4)
			{
				throw std::runtime_error("Datum type mismatch!");
			}

			if (mCapacity == mSize)
			{
				Reserve(mCapacity * 2 + 1);
			}

			mData.mMat4x4[mSize] = item;
			mSize++;
		}

		void Datum::PushBack(const std::string & item)
		{
			if (mIsExternal)
			{
				throw std::runtime_error("Cannot write to external storage!");
			}

			if (mType == DatumType::UNKNOWN)
			{
				mType = DatumType::STRING;
			}

			if (mType != DatumType::STRING)
			{
				throw std::runtime_error("Datum type mismatch!");
			}

			if (mCapacity == mSize)
			{
				Reserve(mCapacity * 2 + 1);
			}
			new (mData.mString + mSize) std::string(item);
			mSize++;
		}

		void Datum::PushBack(RTTI * item)
		{
			if (mIsExternal)
			{
				throw std::runtime_error("Cannot write to external storage!");
			}

			if (mType == DatumType::UNKNOWN)
			{
				mType = DatumType::POINTER;
			}

			if (mType != DatumType::POINTER)
			{
				throw std::runtime_error("Datum type mismatch!");
			}

			if (mCapacity == mSize)
			{
				Reserve(mCapacity * 2 + 1);
			}

			mData.mRTTI[mSize] = item;
			mSize++;
		}

		void Datum::PushBack(Scope& item)
		{
			if (mIsExternal)
			{
				throw std::runtime_error("Cannot write to external storage!");
			}

			if (mType == DatumType::UNKNOWN)
			{
				mType = DatumType::TABLE;
			}

			if (mType != DatumType::TABLE)
			{
				throw std::runtime_error("Datum type mismatch!");
			}

			if (mCapacity == mSize)
			{
				Reserve(mCapacity * 2 + 1);
			}
			mData.mScope[mSize] = &item;
			mSize++;
		}

		void Datum::PopBack()
		{
			if (mSize == 0)
			{
				throw std::runtime_error("Cannot call PopBack on empty datum!");
			}

			if (mIsExternal)
			{
				throw std::runtime_error("Cannot call PopBack on external memory!");
			}

			mSize--;
			switch (mType)
			{
			case DatumType::INTEGER:
				break;
			case DatumType::FLOAT:
				break;
			case DatumType::VECTOR4:
				mData.mVec4[mSize].glm::vec4::~vec4();
				break;
			case DatumType::MATRIX4X4:
				mData.mMat4x4[mSize].glm::mat4x4::~mat4x4();
				break;
			case DatumType::STRING:
				mData.mString[mSize].std::string::~string();
				break;
			case DatumType::POINTER:
				mData.mRTTI[mSize] = nullptr;
				break;
			case DatumType::TABLE:
				mData.mScope[mSize] = nullptr;
			default:
				mSize++;
				throw std::runtime_error("Invalid data type!");
			}
		}

		template<>
		int32_t& Datum::Front()
		{
			if (mSize == 0)
			{
				throw std::runtime_error("Cannot perform Front on an empty array!");
			}
			return Get<int32_t>(0);
		}

		template<>
		float_t& Datum::Front()
		{
			if (mSize == 0)
			{
				throw std::runtime_error("Cannot perform Front on an empty array!");
			}
			return Get<float_t>(0);
		}

		template<>
		glm::vec4& Datum::Front()
		{
			if (mSize == 0)
			{
				throw std::runtime_error("Cannot perform Front on an empty array!");
			}
			return Get<glm::vec4>(0);
		}

		template<>
		glm::mat4x4& Datum::Front()
		{
			if (mSize == 0)
			{
				throw std::runtime_error("Cannot perform Front on an empty array!");
			}
			return Get<glm::mat4x4>(0);
		}

		template<>
		std::string& Datum::Front()
		{
			if (mSize == 0)
			{
				throw std::runtime_error("Cannot perform Front on an empty array!");
			}
			return Get<std::string>(0);
		}

		template<>
		RTTI*& Datum::Front()
		{
			if (mSize == 0)
			{
				throw std::runtime_error("Cannot perform Front on an empty array!");
			}
			return Get<RTTI*>(0);
		}

		template<>
		const int32_t& Datum::Front() const
		{
			if (mSize == 0)
			{
				throw std::runtime_error("Cannot perform Front on an empty array!");
			}
			return Get<int32_t>(0);
		}

		template<>
		const float_t& Datum::Front() const
		{
			if (mSize == 0)
			{
				throw std::runtime_error("Cannot perform Front on an empty array!");
			}
			return Get<float_t>(0);
		}

		template<>
		const glm::vec4& Datum::Front() const
		{
			if (mSize == 0)
			{
				throw std::runtime_error("Cannot perform Front on an empty array!");
			}
			return Get<glm::vec4>(0);
		}

		template<>
		const glm::mat4x4& Datum::Front() const
		{
			if (mSize == 0)
			{
				throw std::runtime_error("Cannot perform Front on an empty array!");
			}
			return Get<glm::mat4x4>(0);
		}

		template<>
		const std::string& Datum::Front() const
		{
			if (mSize == 0)
			{
				throw std::runtime_error("Cannot perform Front on an empty array!");
			}
			return Get<std::string>(0);
		}

		template<>
		RTTI* const& Datum::Front() const
		{
			if (mSize == 0)
			{
				throw std::runtime_error("Cannot perform Front on an empty array!");
			}
			return Get<RTTI*>(0);
		}

		template<>
		int32_t& Datum::Back()
		{
			if (mSize == 0)
			{
				throw std::runtime_error("Cannot perform Back on an empty array!");
			}
			return Get<int32_t>(mSize - 1);
		}

		template<>
		float_t& Datum::Back()
		{
			if (mSize == 0)
			{
				throw std::runtime_error("Cannot perform Back on an empty array!");
			}
			return Get<float_t>(mSize - 1);
		}

		template<>
		glm::vec4& Datum::Back()
		{
			if (mSize == 0)
			{
				throw std::runtime_error("Cannot perform Back on an empty array!");
			}
			return Get<glm::vec4>(mSize - 1);
		}

		template<>
		glm::mat4x4& Datum::Back()
		{
			if (mSize == 0)
			{
				throw std::runtime_error("Cannot perform Back on an empty array!");
			}
			return Get<glm::mat4x4>(mSize - 1);
		}

		template<>
		std::string& Datum::Back()
		{
			if (mSize == 0)
			{
				throw std::runtime_error("Cannot perform Back on an empty array!");
			}
			return Get<std::string>(mSize - 1);
		}

		template<>
		RTTI*& Datum::Back()
		{
			if (mSize == 0)
			{
				throw std::runtime_error("Cannot perform Back on an empty array!");
			}
			return Get<RTTI*>(mSize - 1);
		}

		template<>
		const int32_t& Datum::Back() const
		{
			if (mSize == 0)
			{
				throw std::runtime_error("Cannot perform Back on an empty array!");
			}
			return Get<int32_t>(mSize - 1);
		}

		template<>
		const float_t& Datum::Back() const
		{
			if (mSize == 0)
			{
				throw std::runtime_error("Cannot perform Back on an empty array!");
			}
			return Get<float_t>(mSize - 1);
		}

		template<>
		const glm::vec4& Datum::Back() const
		{
			if (mSize == 0)
			{
				throw std::runtime_error("Cannot perform Back on an empty array!");
			}
			return Get<glm::vec4>(mSize - 1);
		}

		template<>
		const glm::mat4x4& Datum::Back() const
		{
			if (mSize == 0)
			{
				throw std::runtime_error("Cannot perform Back on an empty array!");
			}
			return Get<glm::mat4x4>(mSize - 1);
		}

		template<>
		const std::string& Datum::Back() const
		{
			if (mSize == 0)
			{
				throw std::runtime_error("Cannot perform Back on an empty array!");
			}
			return Get<std::string>(mSize - 1);
		}

		template<>
		RTTI* const& Datum::Back() const
		{
			if (mSize == 0)
			{
				throw std::runtime_error("Cannot perform Back on an empty array!");
			}
			return Get<RTTI*>(mSize - 1);
		}

		bool Datum::Remove(const int32_t & item)
		{
			if (mIsExternal)
			{
				throw std::runtime_error("Cannot remove data from external memory!");
			}
			if (mType != DatumType::INTEGER)
			{
				throw std::runtime_error("Type mismatch!");
			}

			size_t index = Find(item);

			if (index < mSize)
			{
				for (; index < mSize - 1; ++index)
				{
					mData.mInt[index] = mData.mInt[index + 1];
				}
				mSize--;
				return true;
			}
			return false;
		}

		bool Datum::Remove(const float_t & item)
		{
			if (mIsExternal)
			{
				throw std::runtime_error("Cannot remove data from external memory!");
			}
			if (mType != DatumType::FLOAT)
			{
				throw std::runtime_error("Type mismatch!");
			}

			size_t index = Find(item);

			if (index < mSize)
			{
				for (; index < mSize - 1; ++index)
				{
					mData.mFloat[index] = mData.mFloat[index + 1];
				}
				mSize--;
				return true;
			}
			return false;
		}

		bool Datum::Remove(const glm::vec4 & item)
		{
			if (mIsExternal)
			{
				throw std::runtime_error("Cannot remove data from external memory!");
			}
			if (mType != DatumType::VECTOR4)
			{
				throw std::runtime_error("Type mismatch!");
			}

			size_t index = Find(item);

			if (index < mSize)
			{
				for (; index < mSize - 1; ++index)
				{
					mData.mVec4[index] = mData.mVec4[index + 1];
				}
				mSize--;
				return true;
			}
			return false;
		}

		bool Datum::Remove(const glm::mat4x4 & item)
		{
			if (mIsExternal)
			{
				throw std::runtime_error("Cannot remove data from external memory!");
			}
			if (mType != DatumType::MATRIX4X4)
			{
				throw std::runtime_error("Type mismatch!");
			}

			size_t index = Find(item);

			if (index < mSize)
			{
				for (; index < mSize - 1; ++index)
				{
					mData.mMat4x4[index] = mData.mMat4x4[index + 1];
				}
				mSize--;
				return true;
			}
			return false;
		}

		bool Datum::Remove(const std::string & item)
		{
			if (mIsExternal)
			{
				throw std::runtime_error("Cannot remove data from external memory!");
			}
			if (mType != DatumType::STRING)
			{
				throw std::runtime_error("Type mismatch!");
			}

			size_t index = Find(item);

			if (index < mSize)
			{
				for (; index < mSize - 1; ++index)
				{
					mData.mString[index] = std::move(mData.mString[index + 1]);
				}
				mData.mString[mSize - 1].~basic_string();
				mSize--;
				return true;
			}
			return false;
		}

		bool Datum::Remove(const RTTI& item)
		{
			if (mIsExternal)
			{
				throw std::runtime_error("Cannot remove data from external memory!");
			}
			if (mType != DatumType::POINTER)
			{
				throw std::runtime_error("Type mismatch!");
			}

			size_t index = Find(item);

			if (index < mSize)
			{
				for (; index < mSize - 1; ++index)
				{
					mData.mRTTI[index] = mData.mRTTI[index + 1];
				}
				mSize--;
				return true;
			}
			return false;
		}

		bool Datum::Remove(const Scope& item)
		{
			if (mIsExternal)
			{
				throw std::runtime_error("Cannot remove data from external memory!");
			}
			if (mType != DatumType::TABLE)
			{
				throw std::runtime_error("Type mismatch!");
			}

			size_t index = Find(item);
			if (index < mSize)
			{
				for (; index < mSize - 1; ++index)
				{
					mData.mScope[index] = mData.mScope[index + 1];
				}
				mSize--;
				return true;
			}
			return false;
		}

		void Datum::RemoveAt(size_t index)
		{
			if (mIsExternal)
			{
				throw std::runtime_error("Cannot alter values in external memory!");
			}
			if (mType == DatumType::UNKNOWN)
			{
				throw std::runtime_error("Datum type is not set!");
			}
			if (index >= mSize)
			{
				throw std::runtime_error("Out of bounds!");
			}

			switch (mType)
			{
			case DatumType::INTEGER:
				for (size_t i = index; i < mSize - 1; ++i)
				{
					mData.mInt[i] = mData.mInt[i + 1];
				}
				break;
			case DatumType::FLOAT:
				for (size_t i = index; i < mSize - 1; ++i)
				{
					mData.mFloat[i] = mData.mFloat[i + 1];
				}
				break;
			case DatumType::VECTOR4:
				for (size_t i = index; i < mSize - 1; ++i)
				{
					mData.mVec4[i] = mData.mVec4[i + 1];
				}
				break;
			case DatumType::MATRIX4X4:
				for (size_t i = index; i < mSize - 1; ++i)
				{
					mData.mMat4x4[i] = mData.mMat4x4[i + 1];
				}
				break;
			case DatumType::STRING:
				for (size_t i = index; i < mSize - 1; ++i)
				{
					mData.mString[i] = mData.mString[i + 1];
				}
				mData.mString[mSize - 1].~basic_string();
				break;
			case DatumType::POINTER:
				for (size_t i = index; i < mSize - 1; ++i)
				{
					mData.mRTTI[i] = mData.mRTTI[i + 1];
				}
				break;
			case DatumType::TABLE:
				for (size_t i = index; i < mSize - 1; ++i)
				{
					mData.mScope[i] = mData.mScope[i + 1];
				}
			default:
				throw std::runtime_error("Datum type is not set!");
			}
			mSize--;
		}

		size_t Datum::Find(const int32_t & item)
		{
			if (mType == DatumType::UNKNOWN || mType != DatumType::INTEGER)
			{
				throw std::runtime_error("Invalid datum types!");
			}

			size_t index = 0;
			bool found = false;

			for (size_t i = 0; i < mSize; ++i)
			{
				index = i;
				if (item == mData.mInt[i])
				{
					found = true;
					break;
				}
			}
			if (found)
			{
				return index;
			}
			else
			{
				return mSize;
			}
		}

		size_t Datum::Find(const float_t & item)
		{
			if (mType == DatumType::UNKNOWN || mType != DatumType::FLOAT)
			{
				throw std::runtime_error("Invalid datum types!");
			}

			size_t index = 0;
			bool found = false;

			for (size_t i = 0; i < mSize; ++i)
			{
				index = i;
				if (item == mData.mFloat[i])
				{
					found = true;
					break;
				}
			}
			if (found)
			{
				return index;
			}
			else
			{
				return mSize;
			}
		}

		size_t Datum::Find(const glm::vec4 & item)
		{
			if (mType == DatumType::UNKNOWN || mType != DatumType::VECTOR4)
			{
				throw std::runtime_error("Invalid datum types!");
			}

			size_t index = 0;
			bool found = false;

			for (size_t i = 0; i < mSize; ++i)
			{
				index = i;
				if (item == mData.mVec4[i])
				{
					found = true;
					break;
				}
			}
			if (found)
			{
				return index;
			}
			else
			{
				return mSize;
			}
		}

		size_t Datum::Find(const glm::mat4x4 & item)
		{
			if (mType == DatumType::UNKNOWN || mType != DatumType::MATRIX4X4)
			{
				throw std::runtime_error("Invalid datum types!");
			}

			size_t index = 0;
			bool found = false;

			for (size_t i = 0; i < mSize; ++i)
			{
				index = i;
				if (item == mData.mMat4x4[i])
				{
					found = true;
					break;
				}
			}
			if (found)
			{
				return index;
			}
			else
			{
				return mSize;
			}
		}

		size_t Datum::Find(const std::string & item)
		{
			if (mType == DatumType::UNKNOWN || mType != DatumType::STRING)
			{
				throw std::runtime_error("Invalid datum types!");
			}

			size_t index = 0;
			bool found = false;

			for (size_t i = 0; i < mSize; ++i)
			{
				index = i;
				if (item == mData.mString[i])
				{
					found = true;
					break;
				}
			}
			if (found)
			{
				return index;
			}
			else
			{
				return mSize;
			}
		}

		size_t Datum::Find(const RTTI& item)
		{
			if (mType == DatumType::UNKNOWN || mType != DatumType::POINTER)
			{
				throw std::runtime_error("Invalid datum types!");
			}

			size_t index = 0;
			bool found = false;

			for (size_t i = 0; i < mSize; ++i)
			{
				index = i;
				if (&item == mData.mRTTI[i])
				{
					found = true;
					break;
				}
			}
			if (found)
			{
				return index;
			}
			else
			{
				return mSize;
			}
		}

		size_t Datum::Find(const Scope& item)
		{
			if (mType == DatumType::UNKNOWN || mType != DatumType::TABLE)
			{
				throw std::runtime_error("Invalid datum types!");
			}

			size_t index = 0;
			bool found = false;

			for (size_t i = 0; i < mSize; ++i)
			{
				index = i;
				if (&item == mData.mScope[i])
				{
					found = true;
					break;
				}
			}
			if (found)
			{
				return index;
			}
			else
			{
				return mSize;
			}
		}

		const size_t Datum::Find(const int32_t& item) const
		{
			if (mType == DatumType::UNKNOWN || mType != DatumType::INTEGER)
			{
				throw std::runtime_error("Invalid datum types!");
			}

			size_t index = 0;
			bool found = false;

			for (size_t i = 0; i < mSize; ++i)
			{
				index = i;
				if (item == mData.mInt[i])
				{
					found = true;
					break;
				}
			}
			if (found)
			{
				return index;
			}
			else
			{
				return mSize;
			}
		}

		const size_t Datum::Find(const float_t& item) const
		{
			if (mType == DatumType::UNKNOWN || mType != DatumType::FLOAT)
			{
				throw std::runtime_error("Invalid datum types!");
			}

			size_t index = 0;
			bool found = false;

			for (size_t i = 0; i < mSize; ++i)
			{
				index = i;
				if (item == mData.mFloat[i])
				{
					found = true;
					break;
				}
			}
			if (found)
			{
				return index;
			}
			else
			{
				return mSize;
			}
		}

		const size_t Datum::Find(const glm::vec4& item) const
		{
			if (mType == DatumType::UNKNOWN || mType != DatumType::VECTOR4)
			{
				throw std::runtime_error("Invalid datum types!");
			}

			size_t index = 0;
			bool found = false;

			for (size_t i = 0; i < mSize; ++i)
			{
				index = i;
				if (item == mData.mVec4[i])
				{
					found = true;
					break;
				}
			}
			if (found)
			{
				return index;
			}
			else
			{
				return mSize;
			}
		}

		const size_t Datum::Find(const glm::mat4x4& item) const
		{
			if (mType == DatumType::UNKNOWN || mType != DatumType::MATRIX4X4)
			{
				throw std::runtime_error("Invalid datum types!");
			}

			size_t index = 0;
			bool found = false;

			for (size_t i = 0; i < mSize; ++i)
			{
				index = i;
				if (item == mData.mMat4x4[i])
				{
					found = true;
					break;
				}
			}
			if (found)
			{
				return index;
			}
			else
			{
				return mSize;
			}
		}

		const size_t Datum::Find(const std::string& item) const
		{
			if (mType == DatumType::UNKNOWN || mType != DatumType::STRING)
			{
				throw std::runtime_error("Invalid datum types!");
			}

			size_t index = 0;
			bool found = false;

			for (size_t i = 0; i < mSize; ++i)
			{
				index = i;
				if (item == mData.mString[i])
				{
					found = true;
					break;
				}
			}
			if (found)
			{
				return index;
			}
			else
			{
				return mSize;
			}
		}

		const size_t Datum::Find(const RTTI& item) const
		{
			if (mType == DatumType::UNKNOWN || mType != DatumType::POINTER)
			{
				throw std::runtime_error("Invalid datum types!");
			}

			size_t index = 0;
			bool found = false;

			for (size_t i = 0; i < mSize; ++i)
			{
				index = i;
				if (&item == mData.mRTTI[i])
				{
					found = true;
					break;
				}
			}
			if (found)
			{
				return index;
			}
			else
			{
				return mSize;
			}
		}

		const size_t Datum::Find(const Scope& item) const
		{
			if (mType == DatumType::UNKNOWN || mType != DatumType::TABLE)
			{
				throw std::runtime_error("Invalid datum types!");
			}

			size_t index = 0;
			bool found = false;

			for (size_t i = 0; i < mSize; ++i)
			{
				index = i;
				if (&item == mData.mScope[i])
				{
					found = true;
					break;
				}
			}
			if (found)
			{
				return index;
			}
			else
			{
				return mSize;
			}
		}

		Scope& Datum::operator[](size_t index)
		{
			return Get<Scope>(index);
		}

		const Scope& Datum::operator[](size_t index) const
		{
			return Get<Scope>(index);
		}
	}