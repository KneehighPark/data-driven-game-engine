#pragma once
#include "Action.h"
#include "Factory.h"

namespace Library
{
	class ActionDestroyAction final : public Action
	{
		RTTI_DECLARATIONS(ActionDestroyAction, Action)

	public:

		static const std::string ActionInstanceKey;
		static const std::string ActionsKey;


		/// <summary>Default constructor</summary>
		ActionDestroyAction();

		/// <summary>Parameterised Constructor for ActionDestroyAction ( Explicit )</summary>
		/// <param name="name">Const reference to string name</param>
		explicit ActionDestroyAction(const std::string& name);

		/// <summary>Defaulted copy constructor</summary>
		/// <param name="rhs">Const reference to ActionDestroyAction that is to be copied.</param>
		ActionDestroyAction(const ActionDestroyAction& rhs) = default;

		/// <summary>Defaulted move constructor</summary>
		/// <param name="rhs">R-Value reference to ActionDestroyAction that is to be moved.</param>
		ActionDestroyAction(ActionDestroyAction&& rhs) = default;

		/// <summary>Defaulted copy assignment operator</summary>
		/// <param name="rhs">Const reference to ActionDestroyAction that is to be copied.</param>
		/// <returns>Reference to ActionDestroyAction.</returns>
		ActionDestroyAction& operator=(const ActionDestroyAction& rhs) = default;

		/// <summary>Defaulted move assignment operator</summary>
		/// <param name="rhs">R-Value reference to ActionDestroyAction that is to be moved.</param>
		/// <returns>Reference to ActionDestroyAction.</returns>
		ActionDestroyAction& operator=(ActionDestroyAction&& rhs) = default;

		/// <summary>Defaulted destructor</summary>
		virtual ~ActionDestroyAction() = default;

		/// <summary>Getter for action name</summary>
		/// <returns>Const reference to class name, stored as std::string</returns>
		void SetActionName(const std::string& className);
		
		/// <summary>Update method for ActionDestroyAction</summary>
		/// <param name="worldState">WorldState reference</param>
		virtual void Update(WorldState& worldState) override;

		/// <summary>Create clone of an ActionDestroyAction</summary>
		/// <returns>Returns new ActionDestroyAction clone</returns>
		virtual gsl::owner<Scope*> Clone() const override;

		/// <summary>Static method to get Signatures that will be passed to the Type Registry</summary>
		/// <returns>Vector of Signatures</returns>
		static Vector<Signature> Signatures();

	private:
		std::string mActionInstance;
	};

	CONCRETE_FACTORY(ActionDestroyAction, Scope)
}