#include "pch.h"
#include "Entity.h"
#include "Sector.h"
#include "Action.h"

namespace Library
{
	RTTI_DEFINITIONS(Entity)

	Entity::Entity() : Attributed(TypeIdClass())
	{

	}

	Entity::Entity(const std::string& name) : Attributed(TypeIdClass()), mEntityName(name)
	{

	}

	const std::string& Entity::Name() const
	{
		return mEntityName;
	}

	void Entity::SetName(const std::string& name)
	{
		mEntityName = name;
	}

	Sector* Entity::GetSector() const
	{
		assert(GetParent()->Is(Sector::TypeIdClass()));
		return static_cast<Sector*>(GetParent());
	}

	void Entity::SetSector(Sector* sector)
	{
		if (sector != nullptr)
		{
			sector->Adopt(*this, "Entities");
		}
	}

	void Entity::Update(WorldState& worldState)
	{
		Datum& actions = Actions();
		size_t size = actions.Size();
		for (size_t i = 0; i < size; ++i)
		{
			Scope& scope = actions[i];
			assert(scope.Is(Action::TypeIdClass()));
			Action* action = static_cast<Action*>(&scope);
			worldState.Action = action;
			action->Update(worldState);
			worldState.Action = nullptr;
		}
	}

	gsl::owner<Scope*> Entity::Clone() const
	{
		return new Entity(*this);
	}

	Vector<Signature> Entity::Signatures()
	{
		return Vector<Signature>
		{
			{"Name", Datum::DatumType::STRING, 1, offsetof(Entity, mEntityName)},
			{ "Actions", Datum::DatumType::TABLE, 0, 0 }
		};
	}

	Datum& Entity::Actions()
	{
		return (*this)["Actions"];
	}

	Action* Entity::CreateAction(const std::string& className, const std::string& instanceName)
	{
		Scope* scope = Factory<Scope>::Create(className);

		if (scope != nullptr)
		{
			assert(scope->Is(Action::TypeIdClass()));
			Action* action = static_cast<Action*>(scope);
			action->SetName(instanceName);
			action->SetEntity(this);
			return action;
		}
		return nullptr;
	}

	Action* Entity::FindAction(const std::string& actionName)
	{
		Action* action = nullptr;
		auto& actions = Actions();
		for (size_t i = 0; i < actions.Size(); ++i)
		{
			assert(actions[i].Is(Action::TypeIdClass()));
			action = static_cast<Action*>(&actions[i]);
			if (action->Name() == actionName)
			{
				break;
			}
		}
		return action;
	}

	const Action* Entity::FindAction(const std::string& actionName) const
	{
		return const_cast<Entity*>(this)->FindAction(actionName);
	}

}