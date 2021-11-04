#include "pch.h"
#include "Sector.h"

namespace Library
{
	RTTI_DEFINITIONS(World)

	World::World() : Attributed(TypeIdInstance())
	{

	}

	World::World(const std::string& name) : Attributed(TypeIdInstance()), mWorldName(name)
	{

	}

	World::World(const std::string& name, EventQueue& eventQueue) : Attributed(TypeIdClass()), mWorldName(name), mEventQueue(&eventQueue)
	{

	}

	const std::string& World::Name() const
	{
		return mWorldName;
	}

	void World::SetName(const std::string& name)
	{
		mWorldName = name;
	}

	Datum& World::Sectors()
	{
		return mPointersVector[SectorsIndex]->second;
	}

	Sector* World::CreateSector(const std::string& sectorName)
	{
		Sector* sector = new Sector(sectorName);
		sector->SetWorld(this);
		return sector;
	}

	void World::Update(WorldState& worldState)
	{
		worldState.World = this;
		Datum& sectors = Sectors();

		for (size_t i = 0; i < sectors.Size(); ++i)
		{
			Scope& sectorScope = sectors[i];
			assert(sectorScope.Is(Sector::TypeIdClass()));
			Sector& sector = static_cast<Sector&>(sectorScope);
			worldState.Sector = &sector;
			sector.Update(worldState);
		}
		PurgeMarkedScopes();
		worldState.World = nullptr;
	}

	gsl::owner<Scope*> World::Clone() const
	{
		return new World(*this);
	}

	Vector<Signature> World::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name", Datum::DatumType::STRING, 1, offsetof(World, mWorldName)},
			{ "Sectors", Datum::DatumType::TABLE, 0, 0 }
		};
	}

	void World::PurgeMarkedScopes()
	{
		for (auto pntr : mDeletionList)
		{
			delete pntr;
		}
		mDeletionList.Clear();
	}

	void World::MarkForDelete(Scope& scope)
	{
		mDeletionList.PushBack(&scope);
	}

	WorldState& World::GetWorldState()
	{
		assert(mWorldState != nullptr);
		return *mWorldState;
	}

	EventQueue& World::GetEventQueue()
	{
		assert(mEventQueue != nullptr);
		return *mEventQueue;
	}

	Datum& World::Reactions()
	{
		return (*this)["Reactions"];
	}

	Reaction* World::CreateReaction(const std::string& name)
	{
		Reaction* reaction = Factory<Scope>::Create(name)->As<Reaction>();
		if (reaction != nullptr)
		{
			Adopt(*reaction, "Reactions");
		}
		return reaction;
	}
}