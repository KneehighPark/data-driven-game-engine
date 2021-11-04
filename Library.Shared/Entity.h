#pragma once
#include "Attributed.h"
#include "TypeRegistry.h"
#include "Factory.h"
#include "WorldState.h"

namespace Library
{
	/// <summary>Forward declaration of Sector</summary>
	class Sector;

	/// <summary>
	/// Entity is a wrapper for Attributed Scope
	/// Entities automatically populate their tables based on information known at compile time
	/// <Summary>
	class Entity : public Attributed
	{
		RTTI_DECLARATIONS(Entity, Attributed)

	public:
		/// <summary>Constructor for Entity</summary>
		Entity();

		/// <summary>Parameterised constructor for Entity</summary>
		/// <param name="name">Const reference to name</param>
		explicit Entity(const std::string& name);

		/// <summary>Copy Constructor</summary>
		/// <param name="rhs">Const reference to passed Entity.</param>
		Entity(const Entity& rhs) = default;

		/// <summary>Move Constructor</summary>
		/// <param name="rhs">R-value reference to passed Entity.</param>
		Entity(Entity&& rhs) = default;

		/// <summary>Copy Assignment operator</summary>
		/// <param name="rhs">Const reference to passed Entity.</param>
		/// <returns>Reference to Entity.</returns>
		Entity& operator=(const Entity& rhs) = default;

		/// <summary>Move Assignment operator</summary>
		/// <param name="rhs">R-value reference to passed Entity</param>
		/// <returns>Reference to Entity.</returns>
		Entity& operator=(Entity&& rhs) = default;

		/// <summary>Destructor</summary>
		virtual ~Entity() = default;

		/// <summary>Gets name of entity</summary>
		/// <returns>Name of entity as const string reference</returns>
		const std::string& Name() const;

		/// <summary>Sets name of entity</summary>
		/// <param name="name">Const reference to name of the entity</param>
		void SetName(const std::string& name);

		/// <summary>Gets sector associated with entity</summary>
		/// <returns>Pointer to the sector</returns>
		Sector* GetSector() const;

		/// <summary>Sets sector associated with entity</summary>
		/// <param name="sector">Pointer to sector associated with entity.</param>
		void SetSector(Sector* sector);

		/// <summary>Takes in a WorldState reference, updates entity</summary>
		/// <param name="worldState">WorldState reference .</param>
		virtual void Update(WorldState& worldState);

		/// <summary>Create clone of an Entity</summary>
		/// <returns>Returns Entity clone</returns>
		virtual gsl::owner<Scope*> Clone() const override;

		/// <summary>Static method to get Signatures that will be passed to the Type Registry</summary>
		/// <returns>Vector of Signatures</returns>
		static Vector<Signature> Signatures();

		/// <summary>
		/// Getter method for an Entity's Actions
		/// </summary>
		/// <returns>Datum reference that stores an Entity's Actions</returns>
		Datum& Actions();

		/// <summary>
		/// Takes a class name and instance name to instantiate a new Action object and adopt it into the Actions datum.
		/// </summary>
		/// <param name="className">Name of class</param>
		/// <param name="instanceName">Name of instance</param>
		/// <returns>Action pointer</returns>
		Action* CreateAction(const std::string& className, const std::string& instanceName);

		/// <summary>
		/// Finds an action belonging to the entity
		/// </summary>
		/// <param name="actionName">Name of action to be found</param>
		/// <returns>Pointer to Action, nullptr if no action is found</returns>
		Action* FindAction(const std::string& actionName);

		/// <summary>
		/// Finds an action belonging to the entity (const version)
		/// </summary>
		/// <param name="actionName">Name of action to be found</param>
		/// <returns>Pointer to Action, nullptr if no action is found</returns>
		const Action* FindAction(const std::string& actionName) const;

	private:
		std::string mEntityName;
	};

	CONCRETE_FACTORY(Entity, Scope);
}