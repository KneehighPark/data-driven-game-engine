#pragma once
#include "Attributed.h"
#include "Entity.h"
#include "World.h"

namespace Library
{
	/// <summary>
	/// Sector class, which is an artifcat of finite resources
	/// Sector” is loosely comparable to the idea of a “package” or “library”
	/// Sector exists to partition code & data into pieces that we can manage in isolation
	/// </summary>
	class Sector final : public Attributed
	{
		RTTI_DECLARATIONS(Sector, Attributed)

	public:

		/// <summary> Constructor for Sector</summary>
		Sector();

		/// <summary>Parameterised constructor for Sector</summary>
		/// <param name="name">Const reference to name</param>
		explicit Sector(const std::string& name);

		/// <summary>Copy Constructor</summary>
		/// <param name="rhs">Const reference to passed Sector.</param>
		Sector(const Sector& rhs) = default;

		/// <summary>Move Constructor</summary>
		/// <param name="rhs">R-value reference to passed Sector.</param>
		Sector(Sector && rhs) = default;

		/// <summary>Copy Assignment operator</summary>
		/// <param name="rhs">Const reference to passed Sector.</param>
		/// <returns>Reference to Sector.</returns>
		Sector& operator=(const Sector & rhs) = default;

		/// <summary>Move Assignment operator</summary>
		/// <param name="rhs">R-value reference to passed Sector</param>
		/// <returns>Reference to Sector.</returns>
		Sector& operator=(Sector && rhs) = default;

		/// <summary>Destructor</summary>
		virtual ~Sector() = default;

		/// <summary>
		/// Getter method for name of Sector
		/// </summary>
		/// <returns>Const reference to Sector's name</returns>
		const std::string& Name() const;

		/// <summary>
		/// Setter method for name of Sector
		/// </summary>
		/// <param name="name">Const reference to Sector's name</param>
		void SetName(const std::string& name);

		/// <summary>
		/// Gets Entities for a Sector
		/// </summary>
		/// <returns>Reference to datum that stores the entities for a Sector</returns>
		Datum& Entities();

		/// <summary>
		/// Uses the entity factory to make a new object, 
		/// adopts the entity into the sector, 
		/// and returns the address of the new entity.
		/// </summary>
		/// <param name="className">Const reference to a class name as a std::string</param>
		/// <param name="instanceName">Const reference to an instance name as a std::string</param>
		/// <returns>Address of new entity</returns>
		Entity* CreateEntity(const std::string& className, const std::string& instanceName);

		/// <summary>
		/// Getter for an entity's world, as assigned in SetWorld
		/// </summary>
		/// <returns>Address of World</returns>
		World* GetWorld() const;

		/// <summary>
		/// Adopts sector to a World
		/// </summary>
		/// <param name="world">Address of World</param>
		void SetWorld(World* world);

		/// <summary>
		/// Updates entities in the Sector
		/// </summary>
		/// <param name="worldState">Reference to WorldState</param>
		void Update(WorldState& worldState);

		/// <summary>Create clone of a Sector</summary>
		/// <returns>Returns Sector clone</returns>
		virtual gsl::owner<Scope*> Clone() const override;

		/// <summary>Static method to get Signatures that will be passed to the Type Registry</summary>
		/// <returns>Vector of Signatures</returns>
		static Vector<Signature> Signatures();

	private:
		static const size_t EntitiesIndex = 2;

		std::string mSectorName;
	};

	CONCRETE_FACTORY(Sector, Scope);
}