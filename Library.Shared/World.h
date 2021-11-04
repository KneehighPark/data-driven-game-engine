#pragma once
#include "Attributed.h"
#include "TypeRegistry.h"
#include "Sector.h"
#include "EventQueue.h"
#include "Reaction.h"

namespace Library
{
	/// <summary>
	/// Forward declaration of WorldState class
	/// </summary>
	class WorldState;

	/// <summary>
	/// World class, which is a container of Sectors
	/// Defines a global Scope
	/// </summary>
	class World final : public Attributed
	{
		RTTI_DECLARATIONS(World, Attributed)

	public:

		/// <summary> Constructor for World</summary>
		World();

		World(const std::string& name, EventQueue& eventQueue);

		/// <summary>Parameterised constructor for World</summary>
		/// <param name="name">Const reference to name</param>
		explicit World(const std::string& name);

		/// <summary>Copy Constructor</summary>
		/// <param name="rhs">Const reference to passed World.</param>
		World(const World& rhs) = default;

		/// <summary>Move Constructor</summary>
		/// <param name="rhs">R-value reference to passed World.</param>
		World(World && rhs) = default;

		/// <summary>Copy Assignment operator</summary>
		/// <param name="rhs">Const reference to passed World.</param>
		/// <returns>Reference to World.</returns>
		World& operator=(const World & rhs) = default;

		/// <summary>Move Assignment operator</summary>
		/// <param name="rhs">R-value reference to passed World</param>
		/// <returns>Reference to World.</returns>
		World& operator=(World && rhs) = default;

		/// <summary>Destructor</summary>
		virtual ~World() = default;

		/// <summary>
		/// Getter method for name of World
		/// </summary>
		/// <returns>Const reference to World's name</returns>
		const std::string& Name() const;

		/// <summary>
		/// Setter method for name of World
		/// </summary>
		/// <param name="name">Const reference to World's name</param>
		void SetName(const std::string& name);

		/// <summary>
		/// Gets sectors contained in the World
		/// </summary>
		/// <returns>Datum reference storing sectors contained in the World</returns>
		Datum& Sectors();

		Sector* CreateSector(const std::string& sectorName);

		/// <summary>
		/// Calls update methods on the World's contained Sectors
		/// </summary>
		/// <param name="worldState">Reference to WorldState</param>
		void Update(WorldState& worldState);

		/// <summary>
		/// Creates clone of a World
		/// </summary>
		/// <returns>Returns World clone</returns>
		virtual gsl::owner<Scope*> Clone() const override;

		/// <summary>Static method to get Signatures that will be passed to the Type Registry
		/// </summary>
		/// <returns>Vector of Signatures</returns>
		static Vector<Signature> Signatures();

		/// <summary>
		/// Adds a scope to the deletion list
		/// </summary>
		/// <param name="scope">Scope to be deleted</param>
		void MarkForDelete(Scope& scope);

		WorldState& GetWorldState();

		EventQueue& GetEventQueue();

		Datum& Reactions();

		Reaction* CreateReaction(const std::string& name);

	private:
		static const size_t SectorsIndex = 2;

		std::string mWorldName;

		Vector<Scope*> mDeletionList{ 10 };

		void PurgeMarkedScopes();

		WorldState* mWorldState = nullptr;
		EventQueue* mEventQueue = nullptr;
	};
}