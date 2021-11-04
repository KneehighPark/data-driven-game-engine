#pragma once
#include "Action.h"
#include "Factory.h"

namespace Library
{
	class ActionCreateAction final : public Action
	{
		RTTI_DECLARATIONS(ActionCreateAction, Action)
	
	public:
	
		static const std::string PrototypeNameKey;
		static const std::string ActionNameKey;


		/// <summary>Default constructor</summary>
		ActionCreateAction();

		/// <summary>Parameterised Constructor for ActionCreateAction ( Explicit )</summary>
		/// <param name="name">Const reference to string name</param>
		explicit ActionCreateAction(const std::string& name);

		/// <summary>Defaulted copy constructor</summary>
		/// <param name="rhs">Const reference to ActionCreateAction that is to be copied.</param>
		ActionCreateAction(const ActionCreateAction& rhs) = default;

		/// <summary>Defaulted move constructor</summary>
		/// <param name="rhs">R-Value reference to ActionCreateAction that is to be moved.</param>
		ActionCreateAction(ActionCreateAction&& rhs) = default;

		/// <summary>Defaulted copy assignment operator</summary>
		/// <param name="rhs">Const reference to ActionCreateAction that is to be copied.</param>
		/// <returns>Reference to ActionCreateAction.</returns>
		ActionCreateAction& operator=(const ActionCreateAction& rhs) = default;

		/// <summary>Defaulted move assignment operator</summary>
		/// <param name="rhs">R-Value reference to ActionCreateAction that is to be moved.</param>
		/// <returns>Reference to ActionCreateAction.</returns>
		ActionCreateAction& operator=(ActionCreateAction&& rhs) = default;

		/// <summary>Defaulted destructor</summary>
		virtual ~ActionCreateAction() = default;

		/// <summary>Getter for class name</summary>
		/// <returns>Const reference to class name, stored as std::string</returns>
		void SetClassName(const std::string& className);

		/// <summary>Getter for instance name</summary>
		/// <returns>Const reference to instance name, stored as std::string</returns>
		void SetInstanceName(const std::string& instanceName);

		/// <summary>Update method for ActionCreateAction</summary>
		/// <param name="worldState">WorldState reference</param>
		virtual void Update(WorldState& worldState) override;

		/// <summary>Create clone of an ActionCreateAction</summary>
		/// <returns>Returns new ActionCreateAction clone</returns>
		virtual gsl::owner<Scope*> Clone() const override;

		/// <summary>Static method to get Signatures that will be passed to the Type Registry</summary>
		/// <returns>Vector of Signatures</returns>
		static Vector<Signature> Signatures();

	private:
		std::string mClassName;
		std::string mInstanceName;
	};

	CONCRETE_FACTORY(ActionCreateAction, Scope)
}