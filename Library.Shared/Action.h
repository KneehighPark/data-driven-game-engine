#pragma once
#include "Attributed.h"
#include "TypeRegistry.h"
#include "WorldState.h"


namespace Library
{
	/// <summary>Forward declaration of Entity</summary>
	class Entity;

	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed)

	public:

		static const std::string ActionName;

		/// <summary>Defaulted copy constructor</summary>
		/// <param name="rhs">Const reference to Action that is to be copied.</param>
		Action(const Action& rhs) = default;

		/// <summary>Defaulted move constructor</summary>
		/// <param name="rhs">R-Value reference to Action that is to be moved.</param>
		Action(Action && rhs) = default;

		/// <summary>Defaulted copy assignment operator</summary>
		/// <param name="rhs">Const reference to Action that is to be copied.</param>
		/// <returns>Reference to Action.</returns>
		Action& operator=(const Action& rhs) = default;

		/// <summary>Defaulted move assignment operator</summary>
		/// <param name="rhs">R-Value reference to Action that is to be moved.</param>
		/// <returns>Reference to Action.</returns>
		Action& operator=(Action&& rhs) = default;

		/// <summary>Defaulted destructor</summary>
		virtual ~Action() = default;

		/// <summary>Getter for Action's name</summary>
		/// <returns>Const reference to Action's name, stored as std::string</returns>
		const std::string& Name() const;

		/// <summary>Getter for Action's name</summary>
		/// <param name="name">Const reference to Action's name, stored as std::string</param>
		void SetName(const std::string& name);

		/// <summary>Update method</summary>
		/// <param name="worldState">Reference to world state</param>
		virtual void Update(WorldState& worldState) = 0;

		/// <summary>Getter for Entity associated with an Action</summary>
		/// <returns>Entity pointer associated with Action</returns>
		Entity* GetEntity() const;

		/// <summary>Setter for Entity associated with an Action</summary>
		/// <param name="entity">Entity pointer that is to be assigned to the Action</param>
		void SetEntity(Entity* entity);

		/// <summary>Static method to get Signatures that will be passed to the Type Registry</summary>
		/// <returns>Vector of Signatures</returns>
		static Vector<Signature> Signatures();

		/// <summary>Create clone of an Action</summary>
		/// <returns>Returns Action clone</returns>
		virtual gsl::owner<Scope*> Clone() const = 0;

	protected:

		/// <summary>
		/// Parameterized constructor 
		/// </summary>
		/// <param name="typeID">Type ID through RTTI</param>
		/// <param name="name">Name</param>
		Action(const RTTI::IdType typeID, const std::string& name);

	private:

		std::string mActionName;
	};
}