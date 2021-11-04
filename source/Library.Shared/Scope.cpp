#include "pch.h"
#include "Scope.h"

namespace Library
{
	RTTI_DEFINITIONS(Scope)

	Scope::Scope(const size_t size) : mLookupTable(size), mPointersVector(size)
	{
		if (size == 0)
		{
			throw std::runtime_error("Cannot create scope of size 0");
		}
	}

	Scope::Scope(const Scope& rhs) : mLookupTable(rhs.mLookupTable.NumberOfBuckets()), mPointersVector(rhs.mPointersVector.Capacity())
	{
		RecursivelyCopyChilden(rhs);
	}

	Scope::Scope(Scope&& rhs) : mParent(rhs.mParent), mLookupTable(std::move(rhs.mLookupTable)), mPointersVector(std::move(rhs.mPointersVector))
	{
		FixParentPointers(std::move(rhs));
		rhs.mParent = nullptr;
	}

	Scope& Scope::operator=(const Scope& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			RecursivelyCopyChilden(rhs);
		}
		return *this;
	}

	Scope& Scope::operator=(Scope&& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			mParent = rhs.mParent;
			mLookupTable = std::move(rhs.mLookupTable);
			mPointersVector = std::move(rhs.mPointersVector);
			FixParentPointers(std::move(rhs));
			rhs.mParent = nullptr;
		}
		return *this;
	}

	Scope::~Scope()
	{
		Clear();
	}

	void Scope::Clear()
	{
		Orphan();
		
		for (const auto& entry : mPointersVector)
		{
			Datum& datum = entry->second;
			if (datum.Type() == Datum::DatumType::TABLE)
			{
				for (size_t i = 0; i < datum.Size(); ++i)
				{
					datum[i].mParent = nullptr;
					delete &(datum[i]);
				}
			}
		}
		mPointersVector.Clear();
		mLookupTable.Clear();
	}

	bool Scope::operator==(const Scope& rhs) const
	{
		if (mPointersVector.Size() == rhs.mPointersVector.Size())
		{
			auto left = mPointersVector.begin();
			auto right = rhs.mPointersVector.begin();

			for (; left != mPointersVector.end(); ++left, ++right)
			{
				const auto& left_pair = **left;
				const auto& right_pair = **right;

				if (left_pair != right_pair)
				{
					return false;
				}
			}
			return true;
		}
		return false;
	}

	bool Scope::operator!=(const Scope& rhs) const
	{
		return !(operator==(rhs));
	}

	Datum& Scope::operator[](const std::string& name)
	{
		return Append(name);
	}

	const Datum& Scope::operator[](const std::string& name) const
	{
		return At(name);
	}

	Datum& Scope::At(const std::string& name)
	{
		Datum* datum = Find(name);
		if (datum == nullptr)
		{
			throw std::runtime_error("Invalid operation! Entry not found!");
		}
		return *datum;
	}

	const Datum& Scope::At(const std::string& name) const
	{
		const Datum* datum = Find(name);
		if (datum == nullptr)
		{
			throw std::runtime_error("Invalid operation! Entry not found!");
		}
		return *datum;
	}

	Datum& Scope::operator[](const size_t index)
	{
		if (index >= mLookupTable.Size())
		{
			throw std::runtime_error("Invalid operation! Provided index is out of bounds");
		}
		return mPointersVector[index]->second;
	}

	const Datum& Scope::operator[](const size_t index) const
	{
		if (index >= mLookupTable.Size())
		{
			throw std::runtime_error("Invalid operation! Provided index is out of bounds");
		}
		return mPointersVector[index]->second;
	}

	Datum* Scope::Find(const std::string& name)
	{
		if (name.empty())
		{
			return nullptr;
		}

		auto iter = mLookupTable.Find(name);
		if (iter == mLookupTable.end())
		{
			return nullptr;
		}
		return &(iter->second);
	}

	const Datum* Scope::Find(const std::string& name) const
	{
		if (name.empty())
		{
			return nullptr;
		}

		auto iter = mLookupTable.Find(name);
		if (iter == mLookupTable.end())
		{
			return nullptr;
		}
		return &(iter->second);
	}

	Datum* Scope::Search(const std::string& name, Scope** owner)
	{
		Datum* datum = Find(name);

		if (datum != nullptr)
		{
			if (owner != nullptr)
			{
				*owner = this;
			}
			return datum;
		}

		if (mParent == nullptr)
		{
			if (owner != nullptr)
			{
				*owner = nullptr;
			}
			return nullptr;
		}
		return mParent->Search(name, owner);
	}

	Datum& Scope::Append(const std::string& name)
	{
		if (name.empty())
		{
			throw std::runtime_error("Invalid operation! Provided name is empty!");
		}

		bool inserted = false;
		Datum datum;

		auto iter = mLookupTable.Insert(std::make_pair(name, datum), inserted);

		if (inserted)
		{
			mPointersVector.PushBack(&*iter);
		}
		return iter->second;
	}

	Scope& Scope::AppendScope(const std::string& name)
	{
		Datum& datum = Append(name);

		if (datum.Type() == Datum::DatumType::UNKNOWN)
		{
			datum.SetType(Datum::DatumType::TABLE);
		}
		if (datum.Type() != Datum::DatumType::TABLE)
		{
			throw std::runtime_error("Provided name corresponds to a Datum that is not a Scope");
		}

		Scope* scope = new Scope();
		scope->mParent = this;
		datum.PushBack(*scope);
		return *scope;
	}

	void Scope::Adopt(Scope& child, const std::string& name)
	{
		if (this == &child)
		{
			throw std::runtime_error("Invalid operation! Scope cannot adopt itself!");
		}
		if (this->IsAncestorOf(child))
		{
			throw std::runtime_error("Invalid operation! Scope cannot adopt an ancestor!");
		}
		Datum& datum = Append(name);
		datum.SetType(Datum::DatumType::TABLE);
		child.Orphan();
		child.mParent = this;
		datum.PushBack(child);
	}

	std::pair<Datum*, size_t> Scope::FindContainedScope(const Scope& child) const
	{
		for (size_t i = 0; i < mPointersVector.Size(); ++i)
		{
			Datum& datum = mPointersVector[i]->second;

			if (datum.Type() == Datum::DatumType::TABLE)
			{
				for (size_t j = 0; j < datum.Size(); ++j)
				{
					Scope* potential_child = &datum[j];
					if (potential_child == &child)
					{
						return std::make_pair(&datum, j);
					}
				}
			}
		}
		return {};
	}

	void Scope::Orphan()
	{
		if (mParent != nullptr)
		{
			for (auto& iter : mParent->mPointersVector)
			{
				if (iter->second.Type() == Datum::DatumType::TABLE)
				{
					if (iter->second.Remove(*this))
					{
						mParent = nullptr;
						break;
					}
				}
			}
		}
	}




	bool Scope::Equals(const RTTI* rhs) const
	{
		if (rhs == nullptr)
		{
			return false;
		}
		const Scope* scope = rhs->As<Scope>();
		if (scope != nullptr)
		{
			return *scope == *this;
		}
		return false;
	}

	std::string Scope::ToString() const
	{
		return "Scope";
	}

	Scope* Scope::GetParent() const
	{
		return mParent;
	}

	bool Scope::IsDescendentOf(const Scope& scope) const
	{
		return scope.IsAncestorOf(*this);
	}

	bool Scope::IsAncestorOf(const Scope& scope) const
	{
		bool IsAncestor = false;
		Scope* parent = scope.mParent;

		while (parent != nullptr)
		{
			if (parent == this)
			{
				IsAncestor = true;
				break;
			}
			parent = parent->mParent;
		}
		
		return IsAncestor;
	}
	
	void Scope::RecursivelyCopyChilden(const Scope& rhs)
	{
		for (const auto& iter : rhs.mPointersVector)
		{
			const std::string& name = iter->first;
			Datum& existingDatum = iter->second;
			Datum& newDatum = Append(name);

			if (existingDatum.Type() != Datum::DatumType::TABLE)			
			{
				newDatum = existingDatum;
			}
			else
			{
				newDatum.SetType(existingDatum.Type());
				newDatum.Reserve(existingDatum.Size());
				for (size_t i = 0; i < existingDatum.Size(); ++i)
				{
					Scope* sc = existingDatum[i].Clone();
					sc->mParent = this;
					newDatum.PushBack(*sc);
				}
			}
		}
	}

	void Scope::FixParentPointers(Scope&& rhs)
	{
		//Update parent
		if (mParent != nullptr)
		{
			std::pair<Datum*, size_t> container = mParent->FindContainedScope(rhs);
			container.first->Set(this, container.second);
		}

		//Update children
		for (auto& iter : mPointersVector)
		{
			Datum& datum = iter->second;
			if (datum.Type() == Datum::DatumType::TABLE)
			{
				for (size_t i = 0; i < datum.Size(); ++i)
				{
					datum[i].mParent = this;
				}
			}
		}
	}

	size_t Scope::Size() const
	{
		return mPointersVector.Size();
	}

	gsl::owner<Scope*> Scope::Clone() const
	{
		return new Scope(*this);
	}

	const Vector<std::pair<std::string, Datum>*> Scope::GetPointersList() const
	{
		return mPointersVector;
	}
}