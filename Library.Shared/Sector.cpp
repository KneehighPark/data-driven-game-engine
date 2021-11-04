#include "pch.h"
#include "Sector.h"
#include "Factory.h"

namespace Library
{
	RTTI_DEFINITIONS(Sector)

	Sector::Sector() : Attributed(TypeIdInstance())
	{

	}

	Sector::Sector(const std::string& name) : Attributed(TypeIdInstance()), mSectorName(name)
	{

	}

	const std::string& Sector::Name() const
	{
		return mSectorName;
	}

	void Sector::SetName(const std::string& name)
	{
		mSectorName = name;
	}

	Datum& Sector::Entities()
	{
		return mPointersVector[EntitiesIndex]->second;
	}

	Entity* Sector::CreateEntity(const std::string& className, const std::string& instanceName)
	{
		Entity* entity = Factory<Scope>::Create(className)->As<Entity>();
		if (entity != nullptr)
		{
			entity->SetName(instanceName);
			entity->SetSector(this);
			return entity;
		}
		return nullptr;
	}

	World* Sector::GetWorld() const
	{
		assert(GetParent()->Is(World::TypeIdClass()));
		return static_cast<World*>(GetParent());
	}

	void Sector::SetWorld(World* world)
	{
		if (world != nullptr)
		{
			world->Adopt(*this, "Sectors");
		}
	}

	void Sector::Update(WorldState& worldState)
	{
		worldState.Sector = this;
		Datum& entities = Entities();
		for (size_t i = 0; i < entities.Size(); ++i)
		{
			Scope& entityScope = entities[i];
			assert(entityScope.Is(Entity::TypeIdClass()));
			Entity& entity = static_cast<Entity&>(entityScope);
			worldState.Entity = &entity;
			entity.Update(worldState);
		}
		worldState.Entity = nullptr;
	}

	gsl::owner<Scope*> Sector::Clone() const
	{
		return new Sector(*this);
	}

	Vector<Signature> Sector::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name", Datum::DatumType::STRING, 1, offsetof(Sector, mSectorName) },
			{ "Entities", Datum::DatumType::TABLE, 0, 0 }
		};
	}
}