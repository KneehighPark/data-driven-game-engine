#include "pch.h"
#include "Attributed.h"
#include "TypeRegistry.h"

namespace Library
{
	RTTI_DEFINITIONS(Attributed);

	Attributed::Attributed(RTTI::IdType typeID)
	{
		(*this)["this"] = this;
		Populate(typeID);
	}

	Attributed::Attributed(const Attributed& rhs) : Scope(rhs)
	{
		(*this)["this"] = this;
		UpdateExternalStorage(rhs.TypeIdInstance());
	}

	Attributed::Attributed(Attributed&& rhs) : Scope(std::move(rhs))
	{
		(*this)["this"] = this;
		UpdateExternalStorage(rhs.TypeIdInstance());
	}

	Attributed& Attributed::operator=(const Attributed& rhs)
	{
		if (this != &rhs)
		{
			Scope::operator=(rhs);
			(*this)["this"] = this;
			UpdateExternalStorage(rhs.TypeIdInstance());
		}
		return *this;
	}

	Attributed& Attributed::operator=(Attributed&& rhs)
	{
		if (this != &rhs)
		{
			Scope::operator=(std::move(rhs));
			(*this)["this"] = this;
			UpdateExternalStorage(rhs.TypeIdInstance());
		}
		return *this;
	}

	bool Attributed::IsAttribute(const std::string& name) const
	{
		return (Find(name) != nullptr);
	}

	bool Attributed::IsPrescribedAttribute(const std::string& name) const
	{
		if ("this" == name)
		{
			return true;
		}
		const Vector<Signature> signatures = TypeRegistry::GetSignatures(TypeIdInstance());
		for (auto& signature : signatures)
		{
			if (signature.name == name)
			{
				return true;
			}
		}
		return false;
	}

	bool Attributed::IsAuxiliaryAttribute(const std::string& name) const
	{
		return (IsAttribute(name) && !IsPrescribedAttribute(name));
	}

	Datum& Attributed::AppendAuxiliaryAttribute(const std::string& name) 
	{
		if (IsPrescribedAttribute(name))
		{
			throw std::runtime_error("That attribute already exists!");
		}
		return Append(name);
	}

	const Vector<std::pair<std::string, Datum>*> Attributed::GetAttributes() const
	{
		const Vector<std::pair<std::string, Datum>*> attributedPointers = GetPointersList();
		return attributedPointers;
	}

	void Attributed::UpdateExternalStorage(RTTI::IdType typeID)
	{
		const Vector<Signature>& signatures = TypeRegistry::GetSignatures(typeID);

		for (const auto& signature : signatures)
		{
			Datum& datum = At(signature.name);
			datum.SetType(signature.type);
					   
			if (signature.type != Datum::DatumType::TABLE)
			{
				void* offset = reinterpret_cast<uint8_t*>(this) + signature.offset;
				datum.SetStorage(offset, signature.size);
			}
		}
	}

	void Attributed::Populate(RTTI::IdType typeID)
	{
		const Vector<Signature>& signatures = TypeRegistry::GetSignatures(typeID);
		for (const auto& signature : signatures)
		{
			Datum& datum = Append(signature.name);
			datum.SetType(signature.type);

			if (signature.type != Datum::DatumType::TABLE)
			{
				void* offset = reinterpret_cast<uint8_t*>(this) + signature.offset;
				datum.SetStorage(offset, signature.size);
			}
			else
			{
				datum.SetType(Datum::DatumType::TABLE);
				for (size_t i = 0; i < signature.size; ++i)
				{
					AppendScope(signature.name);
				}
			}
		}
	}
}