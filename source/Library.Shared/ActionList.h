#pragma once
#include "Action.h"
#include "Factory.h"

namespace Library
{
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action)
		


	public:

		static const std::string ActionsKey;
		static const size_t ActionsIndex;

		/// <summary>Default constructor </summary>
		ActionList();

		/// <summary>
		/// Parameterized constructor
		/// </summary>
		/// <param name="name">Const reference to string name with which ActionList will be initialized</param>
		explicit ActionList(const std::string& name);

		/// <summary>Defaulted copy constructor</summary>
		/// <param name="rhs">Const reference to ActionList that is to be copied.</param>
		ActionList(const ActionList& rhs) = default;

		/// <summary>Defaulted move constructor</summary>
		/// <param name="rhs">R-Value reference to ActionList that is to be moved.</param>
		ActionList(ActionList&& rhs) = default;

		/// <summary>Defaulted copy assignment operator</summary>
		/// <param name="rhs">Const reference to ActionList that is to be copied.</param>
		/// <returns>Reference to ActionList.</returns>
		ActionList& operator=(const ActionList& rhs) = default;

		/// <summary>Defaulted move assignment operator</summary>
		/// <param name="rhs">R-Value reference to ActionList that is to be moved.</param>
		/// <returns>Reference to ActionList.</returns>
		ActionList& operator=(ActionList&& rhs) = default;

		/// <summary>Defaulted destructor</summary>
		virtual ~ActionList() = default;

		/// <summary>Update method for ActionList</summary>
		/// <param name="worldState"> WorldState reference </param>
		virtual void Update(WorldState& worldState) override;

		/// <summary>
		/// Getter for contained actions
		/// </summary>
		/// <returns>Datum reference that stores an Entity's Actions</returns>
		Datum& Actions();

		/// <summary>Static method to get Signatures that will be passed to the Type Registry</summary>
		/// <returns>Vector of Signatures</returns>
		static Vector<Signature> Signatures();

		/// <summary>Create clone of an ActionList</summary>
		/// <returns>Returns ActionList clone</returns>
		virtual gsl::owner<Scope*> Clone() const override;

	protected:

		/// <summary>
		/// Parameterized constructor 
		/// </summary>
		/// <param name="typeID">Type ID through RTTI</param>
		/// <param name="name">Name</param>
		ActionList(const RTTI::IdType typeID, const std::string& name);
	};

	CONCRETE_FACTORY(ActionList, Scope)
}